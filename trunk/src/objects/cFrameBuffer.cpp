#include "cFrameBuffer.h"

cFrameBuffer::cFrameBuffer(int width, int height) : width(width), height(height) {
	
	if (width == 0 && height == 0) return; // usually this means we're not really using the framebuffer: eg if the object is defined outside of a function.
	
	glGenFramebuffersEXT(1, &fbo);
	bind();
	
	texture = new cTexture(width, height);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture->get_texid(), 0);
	
	GLenum status;
	if ((status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)) != GL_FRAMEBUFFER_COMPLETE_EXT) {
		switch (status) {
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
				cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT" << endl; break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
				cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT" << endl; break;
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				cout << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT" << endl; break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
				cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT" << endl; break;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				cout << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT" << endl; break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
				cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT" << endl; break;
			case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
				cout << "GL_FRAMEBUFFER_UNSUPPORTED_EXT" << endl; break;
			default:
				cout << "ouch" << endl; break;
		} 
				
		cout << width << " " << height << " 0x" << hex << status << dec << endl;
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

void cFrameBuffer::copy_from_buffer(cFrameBuffer* other_buffer, int x_pos = 0, int y_pos = 0) {

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
	
	other_buffer->texture->bind();
	
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

void cFrameBuffer::bind_texture() {

	texture->bind();
	
}
