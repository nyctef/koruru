#ifndef MPLAYAUDIOFILE_H_
#define MPLAYAUDIOFILE_H_

#include "SDL/SDL_mixer.h"

#include "../utils/includes.h"
#include "../audio/cAudioFileState.h"

#define AV_SYNC_THRESHOLD   0.01
#define MAX_AUDIO_ERRORS 3

/** \brief Provides functionality to play an audio file.
 * 
 * This mixin is designed primarily to be used by mode objects.
 */
class mPlayAudioFile {
public:
	/** \brief Basic constructor
	 * 
	 * \param filename The filename of the music file to open
	 */
	mPlayAudioFile(string filename);
	/** \brief Basic destructor
	 * 
	 * Frees the cAudioFileState
	 */
	virtual ~mPlayAudioFile();
	
	/** \brief Start playing the music */
	void start_music();
	/** \brief Check if the music has finished */
	bool is_music_finished();
	/** \brief Stop the music */
	void stop_music();
	
protected:
	/** \brief the audiofilestate. Where all the magic happens. */
	cAudioFileState* audio_file;
	/** \brief the filename of the music we're using. */
	string music_filename;
};

#endif /*MPLAYAUDIOFILE_H_*/
