#include "mPlayAudioFile.h"

/**
 * \brief Our callback for SDL.
 *
 * Called by SDL when it needs moar data.
 *  
 * How this function works: we are given a buffer to fill and the amount of data we need to fill it with.
 * First, we check if we have data in the buffer. If we don't, then we create new data or fill the buffer 
 * with 0.
 * 
 * Next, we check how much data is in the buffer, and how big the data is going to be after we convert it 
 * to the format in SDL's stream. We then copy the data over to the buffer and convert it up (if necessary)
 * to fill the buffer. Note that this doesn't work with downsampling as we would overfill the buffer before 
 * we convert it down. 
 * 
 * We probably need to fix this.
 *
 * \param userdata A void pointer to some data we can use (in this case we pass the audiostate)
 * \param stream The buffer we need to fill
 * \param len the length of the buffer
 */
void audio_callback(void *userdata, Uint8 *stream, int len) {
	
	// the audiostate
    cAudioFileState* is = (cAudioFileState*)userdata;
    double pts;
    int len1; 
    int audio_size;
    static int errorcounter = 0;
	
    while (len > 0) { // while we still have to supply more data
        if (is->buffer.index >= is->buffer.size) { // we have no decoded data to work with
            /* We have already sent all our data; get more */
            audio_size = is->audio_decode_frame(&pts);
            if (audio_size < 0) {
            	
                // If error, output silence, quit after MAX_ERRORS, or quit if eof reached. 
                is->buffer.size = 1024;
                memset(is->buffer.buffer, 0, is->buffer.size);
                dout << "Error happened: " << endl;
                if (is->eof_reached) {
                	dout << "eof_reached true. quitting now." << endl;
                	dout << "setting quit /*and pauseaudio */from sdl cb" << endl;
                	//SDL_PauseAudio(1);
			        is->quit = 1;
			        break;
                } else {
                	dout << "filling buffer with silence. Quitting in " << MAX_AUDIO_ERRORS-errorcounter << endl;
                }
                errorcounter++;
                if (errorcounter >= MAX_AUDIO_ERRORS) {
                	dout << "setting quit /*and pauseaudio */from sdl cb" << endl;
                	//SDL_PauseAudio(1);
			        is->quit = 1;
			        break;
                }
                
            	// no error
            } else {
            	errorcounter = 0;
                audio_size =is->synchronize_audio(audio_size, pts);
                is->buffer.size = audio_size;
            }
            is->buffer.index = 0;
        }
        
        // we now have some data to work with in our buffer.
        
        /**
         * Inside the function:
         * 
         * len is the amount of data left to put into SDL's stream
         * 
         * len1 is the amount of data we have to / will give from our buffer.
         * 
         * len2 is the amount of data that will end up in SDL's stream after we have converted it.
         */
        
        // see how much data we have to give
        len1 = is->buffer.size - is->buffer.index;
        
        int len2 = len1 * is->ffmpeg_to_sdl->len_ratio;
        
        // don't overflow sdl's buffer (only works when upsampling)
        if (len2 > len) {
        	len2 = len; 
            len1 = len/is->ffmpeg_to_sdl->len_ratio;
        }
            
        //dout << len << " " << len1 << " " << len2 << " " << ffmpeg_to_sdl->len_ratio << endl;
            
        // copy the samples across
        memcpy(stream, (uint8_t *)is->buffer.buffer + is->buffer.index, len1);
        
        // convert the samples in-place
        // TODO: create a new buffer to convert the samples into, then copy into SDL. Should allow for downsampling.
        is->ffmpeg_to_sdl->buf = stream;
        is->ffmpeg_to_sdl->len = len1;
        SDL_ConvertAudio(is->ffmpeg_to_sdl);
        
        // move through our buffer and stream
        len -= len2;
        stream += len2;
        is->buffer.index += len1;
    }
}

mPlayAudioFile::mPlayAudioFile(string filename) : audio_file(new cAudioFileState(filename.c_str())) {
	music_filename = filename;
}

mPlayAudioFile::~mPlayAudioFile() {
	delete audio_file;
}

void mPlayAudioFile::start_music() {
	debug_pane << "Opening music file: " << music_filename << " .."; ddot();
	//delete audio_file;
	audio_file = new cAudioFileState(music_filename.c_str()); ddot();
	Mix_HookMusic(audio_callback, audio_file); 
	debug_pane << "done \n";
	
}

bool mPlayAudioFile::is_music_finished() {
	return (audio_file->quit);
}

void mPlayAudioFile::stop_music() {
	audio_file->end();
}
