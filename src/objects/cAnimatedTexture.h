#ifndef CANIMATEDTEXTURE_H_
#define CANIMATEDTEXTURE_H_

#include "cTexture.h"
#include "../utils/utils.h" // for SubSurface

#include <vector>
using namespace std;

/** \brief Provides an animated texture with the same interface
 * as cTexture.
 */
class cAnimatedTexture : public cTexture {
public:
	/** \brief Default constructor. Acts the same as cTexture */
	cAnimatedTexture();
	/** \brief cTexture constructor. Acts the same as cTexture*/
	cAnimatedTexture(const char* file);
	/** \brief Load a series of textures from one image, where (num_frames) frames are stored horizontally. 
	 * Provides an animated texture, as opposed to the other two constructors
	 */ 
	cAnimatedTexture(const char* file, int num_frames, Uint32 frame_time);
	
	virtual ~cAnimatedTexture();
	
	/** \brief Bind the texture. Used the same as cTexture */
	void bind();
	/** \brief Restart the animation */
	void restart();
	
protected:
	void init();
	Uint32 frame_start_time;
	unsigned num_frames;
	vector<GLint> frames;
	unsigned current_frame;
	Uint32 frame_time;
};

#endif /*CANIMATEDTEXTURE_H_*/
