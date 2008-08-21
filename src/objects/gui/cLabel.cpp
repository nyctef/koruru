#include "cLabel.h"

cLabel::cLabel(string text, cFont* font, int x_pos, int y_pos) 
 : cWindow("label_" + text, x_pos, y_pos, font->string_width(text), font->char_height) {
	
	buffer->bind();
	
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,font->string_width(text), font->char_height);
	
	
		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);
	
		glMatrixMode(GL_PROJECTION);
		 glPushMatrix();
		 glLoadIdentity();
		 glOrtho(0, font->string_width(text), 0, font->char_height, -1, 1);
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
	
	cout << gluErrorString(glGetError());
	
}

cLabel::~cLabel() {
}
