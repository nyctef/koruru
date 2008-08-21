#ifndef CFRAMEBUFFER_H_
#define CFRAMEBUFFER_H_

#include "../utils/includes.h"
#define GL_GLEXT_PROTOTYPES
#include "GL/glext.h"

class cFrameBuffer {
public: 
	cFrameBuffer(int width, int height);
	virtual ~cFrameBuffer();
	
	GLuint fbo;
	cTexture* texture;
	
	void bind();
	void copy_from_buffer(cFrameBuffer* other_buffer, int x_pos, int y_pos);
	static void unbind();
	
	void bind_texture();
};

#endif /*CFRAMEBUFFER_H_*/
