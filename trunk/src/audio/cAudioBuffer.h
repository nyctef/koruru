#ifndef AUDIOBUFFER_H_
#define AUDIOBUFFER_H_

#include "ffmpeg/avformat.h"

/** \brief Audio buffer for processed samples
 * 
 * Where we store samples after we've decoded them and before they go to SDL.
 */
class cAudioBuffer {
public:
	/** \brief Where the data is stored (not all of it may be in use at once, 
	 * so the next two variables are used to point to the data).
	 */
	uint8_t			buffer[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; // 1.5 times the max buffer size .. we should be safe.
	/** \brief An index to the data array - specifically, the bit we're 
	 * using at the moment.
	 */
	unsigned		index;
	/** \brief Data size (ie, the offset of index to the end of the data. */
	unsigned		size;
	
	/** \brief Reset the buffer to 0s */
	void clear() {
		memset (&buffer[index], 0, size);
		index = size = 0;
	}
};

#endif /*AUDIOBUFFER_H_*/
