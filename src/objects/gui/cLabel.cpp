#include "cLabel.h"

/**
 * This constructor works by binding the framebuffer and using cFont to render text to it.
 */
cLabel::cLabel(string text, cFont* font, int x_pos, int y_pos) 
 : cWindow("label_" + text, x_pos, y_pos, font->string_width(text), font->char_height) {
	
	render_text(text, font);
	
}

void cLabel::render_text(string text, cFont* font) {
	
	glColor3f(1,1,1);
	
	buffer->bind();
	
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, width, height);
	
	
		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);
	
		glMatrixMode(GL_PROJECTION);
		 glPushMatrix();
		 glLoadIdentity();
		 glOrtho(0, width, 0, height, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		 glPushMatrix();
		 glLoadIdentity();
		
			font->draw(text);
			
		glMatrixMode(GL_PROJECTION);
		 glPopMatrix();   
		glMatrixMode(GL_MODELVIEW);
		 glPopMatrix();
		 
	glPopAttrib();
	buffer->unbind();
}

cLabel::~cLabel() {
}
