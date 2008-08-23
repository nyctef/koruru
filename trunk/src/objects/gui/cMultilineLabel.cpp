#include "cMultilineLabel.h"

cMultilineLabel::cMultilineLabel():cWindow("",0,0,0,0){}

cMultilineLabel::cMultilineLabel(int x_pos, int y_pos, int width, int lines, cFont* font ) 
	: cWindow("multiline_label_", x_pos, y_pos, width*font->char_width, lines*font->char_height), font(font),
	num_lines(lines) {
}

cMultilineLabel::~cMultilineLabel() {
}

void cMultilineLabel::clear() {
	
	lines.clear();
	dirty = true;
	
}

void cMultilineLabel::trim_lines() {

	vector<string>::iterator it;
	
	while (lines.size() > num_lines) {
		it = lines.begin();
		lines.erase(it);
	}
}

void cMultilineLabel::draw() {

		glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	 glPushMatrix();
	 glLoadIdentity();
	 glOrtho(0,SCREEN_WIDTH,0,SCREEN_HEIGHT,-1, 1);
	glMatrixMode(GL_MODELVIEW);
	 glPushMatrix();
	 glLoadIdentity();
	
	 glTranslatef(x_position, y_position, 0);
	 glColor3f(0.5,0.5,0.5);
	 	for (unsigned i=0; i<lines.size(); i++) {
	 		glPushMatrix();
	   		 font->draw(lines[i]);
	   		glPopMatrix();
	   		glTranslatef(0, -font->char_height, 0);
		 }
	  
	glMatrixMode(GL_PROJECTION);
	 glPopMatrix();   
	glMatrixMode(GL_MODELVIEW);
	 glPopMatrix();
	
}
