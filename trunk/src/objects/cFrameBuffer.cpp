#include "cFrameBuffer.h"

cFrameBuffer::cFrameBuffer(int width, int height) {
	
	glGenFramebuffersEXT(1, &fbo);
	bind();
	texture = new cTexture(width, height);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture->get_texid(), 0);
	
	GLenum status;
	if ((status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)) != GL_FRAMEBUFFER_COMPLETE_EXT) {
		throw "error creating framebuffer";
	}
}

cFrameBuffer::~cFrameBuffer() {
	delete texture;
	glDeleteFramebuffersEXT(1, &fbo);
}

void cFrameBuffer::bind() {

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	
}

void cFrameBuffer::copy_from_texture(cTexture* other_texture, int x_pos = 0, int y_pos = 0) {

	bind();
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,texture->width, texture->height);

	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	 glPushMatrix();
	 glLoadIdentity();
	 glOrtho(0, texture->width, 0, texture->height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	 glPushMatrix();
	 glLoadIdentity();
	 
	 glTranslatef(x_pos, y_pos, 0);
	
	glBegin(GL_QUADS);
		// define our vertices and texcoords, starting from the bottom left-hand corner and going anticlockwise
    	glTexCoord2f(0, 1);
    	 glVertex2f(0, 0);
    	
    	glTexCoord2f(1, 1);
    	 glVertex2f(texture->width, 0);
    	
    	glTexCoord2f(1, 0);
    	 glVertex2f(texture->width, texture->height);
    	
    	glTexCoord2f(0, 0);
    	 glVertex2f(0, texture->height);
	
	glMatrixMode(GL_PROJECTION);
	 glPopMatrix();   
	glMatrixMode(GL_MODELVIEW);
	 glPopMatrix();
}

void cFrameBuffer::unbind() {

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
