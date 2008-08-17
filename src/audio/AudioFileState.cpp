#include "AudioFileState.h"

/** 
 * Add or subtract samples to get a better sync, return new
 * audio buffer size. Uses a complex weighted mean of the past
 * few async values to calculate how out of sync we are.
 */ 
int AudioFileState::synchronize_audio(int samples_size, double pts) {
	
	uint8_t* samples = buffer.buffer;
    int num_channels_by2 = 2 * audio_st->codec->channels; // this assumes 16-bit audio: 2 bytes per sample per channel. 
                                                          // May not be portable, need to check
                                                          // Note: apparently ffmpeg gives us 16-bit samples, so this might be fine here.

    double diff, avg_diff;
    int wanted_size, min_size, max_size;

    diff = get_audio_clock() - get_master_clock();

    if (diff < AV_NOSYNC_THRESHOLD) {
        audio_diff_cum = diff + audio_diff_avg_coef * audio_diff_cum;
        
        if (audio_diff_avg_count < AUDIO_DIFF_AVG_NB) { // accumulate diffs
            audio_diff_avg_count++;
        } else { // start calculating avg diffs.
            avg_diff = audio_diff_cum * (1.0 - audio_diff_avg_coef);
            if (fabs(avg_diff) >= audio_diff_threshold) { // re-calc sample size: this is where the action happens
            	
            	// what the target samples size is
                wanted_size = samples_size + ((int)(diff * audio_st->codec->sample_rate) * num_channels_by2);
                
                // calculate maximum change we can make
                min_size = samples_size * ((100 - SAMPLE_CORRECTION_PERCENT_MAX) / 100);
                max_size = samples_size * ((100 + SAMPLE_CORRECTION_PERCENT_MAX) / 100);
                
                // adjust the target size if we're over the maximum change
                if (wanted_size < min_size) {
                    wanted_size = min_size;
                } else if (wanted_size > max_size) {
                    wanted_size = max_size;
                }
                
                //make the change
                if (wanted_size < samples_size) {
                    // remove samples 
                    samples_size = wanted_size;
                } else if (wanted_size > samples_size) {
                    uint8_t* samples_end,* samples_dst;
                    int num_samples; // The number of samples we need to fill

                    // add samples by copying final sample
                    num_samples = (samples_size - wanted_size); // the number of samples we need to create
                    samples_end = (uint8_t *)samples + samples_size - num_channels_by2; // sets samples_end to the beginning of the last set of samples
                    samples_dst = samples_end + num_channels_by2; // sets samples_dst to the location of the first blank sample we need to fill
                    
                    // while we have samples left to fill, copy the last real sample to the next space that we need to fill.
                    while (num_samples > 0) {
                        memcpy(samples_dst, samples_end, num_channels_by2);
                        samples_dst += num_channels_by2;
                        num_samples -= num_channels_by2;
                    }
                    samples_size = wanted_size;
                }
            }
        }
    } else {
        /* difference is TOO big; reset diff stuff */
        audio_diff_avg_count = 0;
        audio_diff_cum = 0;
    }
    
    return samples_size;
}



/** 
 * From the tutorial:
 * 
 * This whole process actually starts towards the end of the function, 
 * where we call packet_queue_get(). We pick the packet up off the queue, 
 * and save its information. Then, once we have a packet to work with, we 
 * call avcodec_decode_audio2(), which acts a lot like its sister function, 
 * avcodec_decode_video(), except in this case, a packet might have more 
 * than one frame, so you may have to call it several times to get all the 
 * data out of the packet. Also, note the audio_buf cast -- SDL gives an 
 * 8 bit int buffer, and ffmpeg gives us data in a 16 bit int buffer. You 
 * should also notice the difference between len1 and data_size. len1 is 
 * how much of the packet we've used, and data_size is the amount of raw 
 * data returned. <br>
 * When we've got some data, we immediately return to see if we still need 
 * to get more data from the queue, or if we are done. If we still had more 
 * of the packet to process, we save it for later. If we finish up a packet,
 * we finally get to free that packet. 
 */
int AudioFileState::audio_decode_frame(double *pts_ptr) {

    int len1, data_size, num_channels_by2;
    AVPacket *pkt = &audio_packet;
    while (1) {
        while (audio_packet_size > 0) {
            data_size = sizeof(buffer.buffer);
            len1 = avcodec_decode_audio2(audio_st->codec, // codec context
                                         (int16_t *)buffer.buffer, &data_size, // output buffer and its size
                                         audio_packet_data, audio_packet_size); // input buffer and its size
                                         
            if (len1 <= 0) { // len1 is negative on error, else the number of bytes used from audio_packet_data
                audio_packet_size = 0;
                dout << "Error, skipping frame" << endl;
                break;
            }
            
            audio_packet_data += len1; // advance the pointer past the used data
            audio_packet_size -= len1; // decrease the size so we don't overshoot
            
            if (data_size <= 0) { // happens when no frame could be decompressed
                dout << "no data yet, get more frames" << endl;
                continue;
            }

            *pts_ptr = audio_clock;
            num_channels_by2 = 2 * audio_st->codec->channels;
            audio_clock += (double)data_size / (double)(num_channels_by2 * audio_st->codec->sample_rate);

            /* We have data, return it and come back for more later */
            return data_size;
        }
        
        if (pkt->data) av_free_packet(pkt);
        
        if (quit) {
        	dout << "quitting adf from is->quit" << endl;
            return -1;
        }
        /* next packet */
        if (queue.get(pkt, 0) < 0) { // get packet, check for errors
        	dout << "packet error " << endl;
            return -1;
        }
        
        // set our state to the current packet
        audio_packet_data = pkt->data;
        audio_packet_size = pkt->size;

        /* if update, update the audio clock w/pts */
        if (pkt->pts != (unsigned)AV_NOPTS_VALUE) {
            audio_clock = av_q2d(audio_st->time_base)*pkt->pts;
        }
    }
}

int decode_interrupt_cb(void) { 
	dout << "dicb" << endl; 
    //return (global_video_state && global_video_state->quit);
    return 0;
}

int decode_thread(void *arg) {

	AudioFileState* is = (AudioFileState *)arg;
    
    AVFormatContext *pFormatCtx;
    AVPacket* packet = new AVPacket;
    
	//
    // main decode loop
	//
	
    while (1) {
    	//cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << av_gettime();
        if (is->quit) {
            break;
        }
        
        if (is->queue.size > MAX_AUDIOQ_SIZE) {
            SDL_Delay(10);
            continue;
        }
        
        if (av_read_frame(is->pFormatCtx, packet) < 0) {
            if (url_ferror(&pFormatCtx->pb) == 0) {
                SDL_Delay(100); /* no error; wait for user input */
                continue;
            } else {
                break;
            }
        }
        
        if (packet->stream_index == is->audio_stream) {
            is->queue.put(packet);
        } else {
            av_free_packet(packet);
        }
    }
    
    /* all done - wait for it */
    dout << is->filename << " : " << is << " : waiting for quit" << endl;
    is->eof_reached = true;
    while (!is->quit) {
        SDL_Delay(100);
    }

    return 0;
}

AudioFileState::AudioFileState(const char* nfilename) {
	
	if (!boost::filesystem::exists(nfilename)) throw "File doesn't exist.";
    
    strcpy(filename, nfilename);

    unsigned i;
	quit = eof_reached = false;
    audio_stream = -1;

    // will interrupt blocking functions if we quit!
    // TODO: I don't think this seems to do anything; it doesn't seem to get called at all. Find out more.
    url_set_interrupt_cb(decode_interrupt_cb);

    if (av_open_input_file(&pFormatCtx, filename, NULL, 0, NULL) != 0) throw "Couldn't open file";
    if (av_find_stream_info(pFormatCtx) < 0) throw "Couldn't find stream information";

    // Dump information about file onto standard error
#    ifdef DEBUG
     dump_format(pFormatCtx, 0, filename, 0);
#    endif

    // Find the first audio stream
    for (i=0; i<pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO) {
        	audio_stream = i; 
        	break;  	
        }
    }
	if (audio_stream < 0) throw "No audio found in file.";

    AVCodecContext* codecCtx;
    AVCodec* codec;

    // Get a pointer to the codec context for the audio stream
    codecCtx = pFormatCtx->streams[audio_stream]->codec;

	while (spec->freq == 0) {
		SDL_Delay(10); // wait for the audio format we're getting to be determined.
		               // happens in the main function.
		dout << "waiting for spec->samples" << endl;
	}

	// create our sdl conversion struct.
	// TODO: AUDIO_S16SYS might not be portable: see if we might need something else.
	ffmpeg_to_sdl = new SDL_AudioCVT;
	if(SDL_BuildAudioCVT(ffmpeg_to_sdl, 
			AUDIO_S16SYS, codecCtx->channels, codecCtx->sample_rate,
			AUDIO_S16SYS, spec->channels,     spec->freq             ) < 0) {
		throw "Could not build audio converter";
	}

    codec = avcodec_find_decoder(codecCtx->codec_id);
    if (!codec || (avcodec_open(codecCtx, codec) < 0)) throw "Unsupported codec!";

    audio_st = pFormatCtx->streams[audio_stream];
    buffer.size = 0;
    buffer.index = 0;

    /* averaging filter for audio sync */
    audio_diff_avg_coef = 0.01 / AUDIO_DIFF_AVG_NB;
    audio_diff_avg_count = 0;
    /* Correct audio only if larger error than this */
    audio_diff_threshold = 2.0 * SDL_AUDIO_BUFFER_SIZE / codecCtx->sample_rate;
    
    dout << "Creating new thread for " << filename << " with pointer " << this << endl;
    parse_tid = SDL_CreateThread(decode_thread, this);
    if (parse_tid == NULL) throw "Cannot create new thread";

}

/**
 * Uses the pts of the currently decoding packet, then adds an offset
 * based on the audio buffer
 */
double AudioFileState::get_audio_clock() {
    double pts;
    int hw_buf_size, bytes_per_sec, n;

    pts = audio_clock; /* maintained in the audio thread */
    hw_buf_size = buffer.size - buffer.index;
    bytes_per_sec = 0;
    n = audio_st->codec->channels * 2;
    if (audio_st) {
        bytes_per_sec = audio_st->codec->sample_rate * n;
    }
    if (bytes_per_sec) {
        pts -= (double)hw_buf_size / bytes_per_sec;
    }
    return pts;
}
	
