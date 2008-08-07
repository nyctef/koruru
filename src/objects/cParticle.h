#ifndef CPARTICLE_H_
#define CPARTICLE_H_

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <iostream>
using namespace std;

/** \brief Basic particle. 
 */
class cParticle {
public:
	cParticle(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat xvel, GLfloat yvel, GLfloat zvel, Uint32 life, GLfloat colour[3]);
	cParticle(GLfloat pos[3], GLfloat vel[3], Uint32 life, GLfloat colour[3]);
	virtual ~cParticle();
	
	virtual void update();
	virtual void draw();
	
	GLfloat velocity[3];
	GLfloat colour[3];
	bool deleteme;
	
protected:
	Uint32 life;
	Uint32 birthtime;
	GLfloat position[3];
};

#endif /*CPARTICLE_H_*/
