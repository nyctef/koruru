#ifndef AUDIOBUFFER_H_
#define AUDIOBUFFER_H_

#include "ffmpeg/avformat.h"

/** \brief Audio buffer for processed samples
 * 
 * Where we store samples after we've decoded them and before they go to SDL.
 */
typedef struct AudioBuffer {
	/** \brief The data */
	uint8_t			buffer[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; // 1.5 times the max buffer size .. we should be safe.
	/** \brief Data size */
	unsigned		size;
	/** \brief An index to the data array */
	unsigned		index;

	void clear() {
		memset (&buffer[index], 0, size);
		index = size = 0;
	}
} AudioBuffer;

#endif /*AUDIOBUFFER_H_*/
