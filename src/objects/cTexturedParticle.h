#ifndef CTEXTUREDPARTICLE_H_
#define CTEXTUREDPARTICLE_H_

#include "cParticle.h"
#include "cTexture.h"

class cTexturedParticle : public cParticle {
public:
	cTexturedParticle(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat xvel, GLfloat yvel, GLfloat zvel, Uint32 nlife);
	cTexturedParticle(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat xvel, GLfloat yvel, GLfloat zvel, Uint32 nlife, cTexture* texture);
	cTexturedParticle(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat xvel, GLfloat yvel, GLfloat zvel, Uint32 nlife, const char* file);
	cTexturedParticle(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat xvel, GLfloat yvel, GLfloat zvel, Uint32 nlife, GLfloat colour[3]);
	cTexturedParticle(GLfloat pos[3], GLfloat vel[3], Uint32 nlife, GLfloat colour[3]);
	cTexturedParticle(GLfloat pos[3], GLfloat vel[3], Uint32 nlife, cTexture* texture);
	virtual ~cTexturedParticle();
	void init(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat xvel, GLfloat yvel, GLfloat zvel, Uint32 nlife, cTexture* texture);
	
	cTexture* texture;
	GLfloat size; // factor to scale size by.
	using cParticle::velocity;
	using cParticle::colour;
	void draw();
	
protected:
	using cParticle::life;
	using cParticle::birthtime;
	using cParticle::position;
};


#endif /*CTEXTUREDPARTICLE_H_*/
