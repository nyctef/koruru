#include "cTextPane.h"

cTextPane::cTextPane (cFont* font, int x, int y, int width, int height) 
	: font(font), x(x), y(y), width(width), height(height) {
}

cTextPane::~cTextPane() {
	
}

/* template <class T>
void cTextPane::add(const T& t) */

void cTextPane::clear() {
	text = "";
}

void cTextPane::draw() {

	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	 glPushMatrix();
	 glLoadIdentity();
	 glOrtho(0,SCREEN_WIDTH,0,SCREEN_HEIGHT,-1, 1);
	glMatrixMode(GL_MODELVIEW);
	 glPushMatrix();
	 glLoadIdentity();
	
	 glTranslatef(x, y, 0);
	 glColor3f(0.5,0.5,0.5);
	 
	 // split the text up by newlines and actually draw a new line for each
	 
	 string token;
	 stringstream textstream(text);
	 	while (getline(textstream, token, '\n')) {
	 		glPushMatrix();
	   		 font->draw(token);
	   		glPopMatrix();
	   		glTranslatef(0, -font->char_height, 0);
	 	}
	  
	glMatrixMode(GL_PROJECTION);
	 glPopMatrix();   
	glMatrixMode(GL_MODELVIEW);
	 glPopMatrix();
		
}
