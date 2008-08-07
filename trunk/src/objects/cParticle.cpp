#include "cParticle.h"

cParticle::cParticle(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat xvel, GLfloat yvel, GLfloat zvel, Uint32 nlife, GLfloat ncolour[3]) {
	position[0] = xpos;
	position[1] = ypos;
	position[2] = zpos;
	velocity[0] = xvel;
	velocity[1] = yvel;
	velocity[2] = zvel;
	life = nlife;
	birthtime = SDL_GetTicks();
	for (int i=0;i<3;i++) colour[i] = ncolour[i];
}

cParticle::cParticle(GLfloat nposition[3], GLfloat nvelocity[3], Uint32 nlife, GLfloat ncolour[3]) {
	
	for (int i=0; i<3; i++) {
	
		position[i] = nposition[i];
		velocity[i] = nvelocity[i];
		colour[i] = ncolour[i];
		
	} 
	life = nlife;
	birthtime = SDL_GetTicks();

}

cParticle::~cParticle() {
}

void cParticle::update() {
	
	if (SDL_GetTicks() - birthtime > life) deleteme = true;
	
	for (int i=0; i<3; i++) position[i] += velocity[i]; 
	
}

void cParticle::draw() {

	glBegin(GL_POINTS);
	
		glColor3fv(colour);
		glVertex3f(position[0], position[1], position[2]);
	
	glEnd();
	
}

