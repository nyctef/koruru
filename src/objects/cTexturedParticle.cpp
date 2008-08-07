#include "cTexturedParticle.h"

/*cTexturedParticle::cTexturedParticle (GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat xvel, GLfloat yvel, GLfloat zvel, Uint32 nlife)
: cParticle(xpos, ypos, zpos, xvel, yvel, zvel, nlife) {
	
	texture = NULL;
	
}*/
GLfloat white[3] = {1,1,1};
cTexturedParticle::cTexturedParticle (GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat xvel, GLfloat yvel, GLfloat zvel, Uint32 nlife, cTexture* ntexture)
: cParticle(xpos, ypos, zpos, xvel, yvel, zvel, nlife, white) {
	
	texture = ntexture;
	size = 1;
	
}
cTexturedParticle::cTexturedParticle (GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat xvel, GLfloat yvel, GLfloat zvel, Uint32 nlife, GLfloat colour[3])
: cParticle(xpos, ypos, zpos, xvel, yvel, zvel, nlife, colour) {
	
	texture = NULL;
	size = 0.5;
	
}

cTexturedParticle::cTexturedParticle (GLfloat pos[3], GLfloat vel[3], Uint32 nlife, cTexture* ntexture)
: cParticle(pos, vel, nlife, white) {
	
	texture = ntexture;
	size = 1;
	
}
cTexturedParticle::cTexturedParticle (GLfloat pos[3], GLfloat vel[3], Uint32 nlife, GLfloat colour[3])
: cParticle(pos, vel, nlife, colour) {
	
	texture = NULL;
	size = 0.5;
	
}

/*cTexturedParticle::cTexturedParticle (GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat xvel, GLfloat yvel, GLfloat zvel, Uint32 nlife, const char* file)
: cParticle(xpos, ypos, zpos, xvel, yvel, zvel, nlife) {
	
	texture = new cTexture(file);
	
}*/

/*void cTexturedParticle::init(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat xvel, GLfloat yvel, GLfloat zvel, Uint32 nlife, cTexture* texture) {
	
};*/

void cTexturedParticle::draw() {
	
	glColor3fv(colour);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	if (texture != NULL) texture->bind();
	glScalef(size, size, size);
	glBegin(GL_QUADS);
    	glTexCoord2f(0,0);
    	 glVertex3f (position[0],position[1],position[2]);
    	 
    	glTexCoord2f(1,0);
    	 glVertex3f (position[0]+size,position[1],position[2]);
    	 
    	glTexCoord2f(1,1);
    	 glVertex3f (position[0]+size,position[1]+size,position[2]);
    	 
    	glTexCoord2f(0,1);
    	 glVertex3f (position[0],position[1]+size,position[2]);
  	glEnd();
  	glDisable(GL_TEXTURE_2D);
  	glPopMatrix();
}

cTexturedParticle::~cTexturedParticle()
{
}
