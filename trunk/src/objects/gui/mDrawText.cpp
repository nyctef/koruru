#include "mDrawText.h"

mDrawText::mDrawText()
{
}

mDrawText::~mDrawText()
{
}

void mDrawText::render_to_bindable(string text, cFont* font, int x_pos, int y_pos, iBindable* buffer) {

	buffer->bind();

	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, buffer->width, buffer->height);
	
	
		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);
	
		glMatrixMode(GL_PROJECTION);
		 glPushMatrix();
		 glLoadIdentity();
		 glOrtho(0, buffer->width, 0, buffer->height, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		 glPushMatrix();
		 glLoadIdentity();
		 
		 	glTranslatef(x_pos, y_pos, 0);
		
			font->draw(text);
			
		glMatrixMode(GL_PROJECTION);
		 glPopMatrix();   
		glMatrixMode(GL_MODELVIEW);
		 glPopMatrix();
		 
	glPopAttrib();
	
	buffer->unbind();
	
}
