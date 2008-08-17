#ifndef AUDIOFILESTATE_H_
#define AUDIOFILESTATE_H_

#include "ffmpeg/avformat.h"
#include "cPacketQueue.h"
#include "cAudioBuffer.h"
#include "SDL/SDL.h"

#include <cmath>
#include <cstdio>
#include <iostream>
using namespace std;

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

/** \brief Some debugging info to use with cout or dout */
#define DEBUG_INFO __FILE__ << ":" << __LINE__ << " "

#if defined( DEBUG ) 
/** \brief A DEBUG-dependant cout 
 * 
 * use as "dout << debugging_info << endl;"
 */
# define dout if(0); else cout << DEBUG_INFO
#else
# define dout if(1); else cout
#endif


#define FF_QUIT_EVENT (SDL_USEREVENT + 1)
#define AUDIO_DIFF_AVG_NB 20
/** \brief number of samples we want in our buffer */
#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)

#define AV_NOSYNC_THRESHOLD 10.0

#define SAMPLE_CORRECTION_PERCENT_MAX 10

extern SDL_AudioSpec* wanted_spec,* spec;

/** \brief The current audio state
 */
class cAudioFileState {
public:

	/** \brief Data about the audio format */
    AVFormatContext *pFormatCtx;
    /** \brief The position of the first audio stream in the file */
    int             audio_stream;
    
	/** \brief The current real-time audio position (of the currently-decoded packet) 
	 * Use get_audio_clock() to find what the currently-playing audio clock is.  */
    double          audio_clock;
    /** \brief Data about the audio stream */
    AVStream*       audio_st;
    /** \brief Where we store extracted packets before decoding them */ 
    cPacketQueue     queue;
    /** \brief Where we store decoded samples before sending them to SDL */
    cAudioBuffer		buffer;
    
    /** \brief current packet */
    AVPacket        audio_packet;
    /** \brief Pointer to where in the current packet we're currently prodding. */
    uint8_t*        audio_packet_data; 
    /** \brief Distance from audio_packet_data to the end of the packet. */
    int             audio_packet_size;
    
    /** \brief used for AV buffer sync computation */ 
    double          audio_diff_cum; 
    double          audio_diff_avg_coef;
    double          audio_diff_threshold;
    int             audio_diff_avg_count;

	/** \brief A reference to the decoding thread */
    SDL_Thread      *parse_tid;
	
	/** \brief The filename */
    char            filename[1024];
    
    /** \brief have we reached the end of the file */
    bool			eof_reached;
    
    /** \brief Signals various threads and things to quit once the file has finished playing. */
    bool            quit;
    
    /** \brief An SDL audio conversion struct that SDL uses to convert audio 
     * samples from the file format to the hardware format
     */
	SDL_AudioCVT*   ffmpeg_to_sdl;

	/** \brief Constructor
	 * \param filename the file to load
	 */
	cAudioFileState(const char* filename);
    
    /** \brief Get the pts of the currently playing packet.
	 */
    double get_audio_clock();
	/** \brief Resize the buffer if we are out of sync
	 * 
	 * \param samples_size Size to resync
	 * \param pts Current pts.
	 * \return the new sample size to use
 	*/ 
	int synchronize_audio( int samples_size, double pts);
	/** \brief Decode a frame of audio
	 * \param pts_ptr Pointer to where we store the current audio pts
	 * \return data size: how much data was gathered, -1 for error.
	 */
	int audio_decode_frame(double *pts_ptr);
	
	/** \brief End early */
	void end(){quit=true;}
};

/** \brief Get the master clock value from ffmpeg (in seconds)
 */
double get_master_clock();

/** \brief Where most of the action happens
 * 
 * Runs in a separate thread, initialises stuff then runs the main loop.
 * 
 * Data: file -> cPacketQueue -> (decoding) -> cAudioBuffer -> SDL's buffer -> hardware
 * \param arg An cAudioFileState pointer
 * \return 0
 */
int decode_thread(void *arg);

#endif /*AUDIOFILESTATE_H_*/
