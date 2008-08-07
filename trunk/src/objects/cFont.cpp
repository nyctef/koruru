#include "cFont.h"

cFont::cFont(string fontfile, int width, int height, string ncharacters) 
: characters (ncharacters) {

	char_width = width;
	char_height = height;
	
	glEnable(GL_TEXTURE_2D);
	texture = new cTexture(fontfile);
    
}

void cFont::draw(string text) {
	
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
    texture->bind();

	float xoff = 0;  /// x offset.
	GLfloat off = 0; /// screen offset
	
	glBegin(GL_QUADS);
  	  for (unsigned i=0; i<text.size(); ++i){
 
 		if (characters.find(text[i]) != string::npos) {
 		xoff = characters.find(text[i])*char_width;
 		
 		// define our vertices and texcoords, starting from the bottom left-hand corner and going clockwise
    	glTexCoord2f(xoff/1024, 1);
    	glVertex2f(off+0, 0);
    	
    	glTexCoord2f((xoff+char_width)/1024, 1);
    	glVertex2f(off+char_width, 0);
    	
    	glTexCoord2f((xoff+char_width)/1024, 0);
    	glVertex2f(off+char_width, char_height);
    	
    	glTexCoord2f((xoff)/1024, 0);
    	glVertex2f(off+0, char_height);
 		}
 	   off += char_width;
 	 }
  	glEnd();
  	glDisable(GL_TEXTURE_2D);
 	glPopMatrix();
	
}

void cFont::Test() {

	glPushMatrix();
    texture->bind();
	glBegin(GL_QUADS);
 		// define our vertices and texcoords, starting from the bottom left-hand corner and going clockwise
 		//glColor3f(1,1,1);
    	glTexCoord2f(0,0);
    	 glVertex2f (0,0);
    	 
    	glTexCoord2f(1,0);
    	 glVertex2f (1024,0);
    	 
    	glTexCoord2f(1,1);
    	 glVertex2f (1024,32);
    	 
    	glTexCoord2f(0,1);
    	 glVertex2f (0,32);
  	glEnd();
  	glPopMatrix();
	
}


unsigned cFont::string_width(string str) {
	return char_width*str.length();
}
