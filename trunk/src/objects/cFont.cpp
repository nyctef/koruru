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
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    texture->bind();

	float xoff = 0;  /// x offset.
	GLfloat off = 0; /// screen offset
	
	glBegin(GL_QUADS);
  	  for (unsigned i=0; i<text.size(); ++i){
 
 		if (characters.find(text[i]) != string::npos) {
	 		xoff = characters.find(text[i])*char_width;
	 		
	 		//cout << text[i] << " " << characters.find(text[i]) << " " << xoff << " " << xoff/texture->width << " " << (xoff+char_width) << endl;
	 		
	 		// define our vertices and texcoords, starting from the bottom left-hand corner and going anticlockwise
	    	glTexCoord2f(xoff/texture->width, 1);
	    	 glVertex2f(off, 0);
	    	
	    	glTexCoord2f((xoff+char_width)/texture->width, 1);
	    	 glVertex2f(off+char_width, 0);
	    	
	    	glTexCoord2f((xoff+char_width)/texture->width, 0);
	    	 glVertex2f(off+char_width, char_height);
	    	
	    	glTexCoord2f((xoff)/texture->width, 0);
	    	 glVertex2f(off, char_height);
	    	
 		} else if (strcmp(text.substr(i, 1).c_str(), " ") != 0) {
 			cout << "char " << text[i] << " not found in " << characters << endl;
 		}
 	   off += char_width;
 	 }
  	glEnd();
 	glPopMatrix();
	
}

void cFont::Test() {

	glPushMatrix();
    texture->bind();
	glBegin(GL_QUADS);
    	glTexCoord2f(0,0);
    	 glVertex2f (0,0);
    	 
    	glTexCoord2f(1,0);
    	 glVertex2f (texture->width,0);
    	 
    	glTexCoord2f(1,1);
    	 glVertex2f (texture->width,texture->height);
    	 
    	glTexCoord2f(0,1);
    	 glVertex2f (0,texture->height);
  	glEnd();
  	glPopMatrix();
	
}


unsigned cFont::string_width(string str) {
	return char_width*str.length();
}
