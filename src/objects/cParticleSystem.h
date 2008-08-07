#ifndef CPARTICLESYSTEM_H_
#define CPARTICLESYSTEM_H_

#include "iObject.h"
#include "cParticle.h"
#include "cTexturedParticle.h"

#include <time.h>

#include <list>
using namespace std;

class cParticleSystem : public iObject {
public:
	cParticleSystem(unsigned numberofparticles); 
	cParticleSystem(unsigned numberofparticles, GLfloat startposition[3], GLfloat nvelocity[3], 
                    GLfloat velocityrandomness, GLfloat liferandomness, GLfloat nforce[3], GLfloat colour[3]);
	virtual ~cParticleSystem();
	
	void init();
	void init(unsigned numberofparticles, GLfloat startposition[3], GLfloat nvelocity[3], 
              GLfloat velocityrandomness, GLfloat liferandomness, GLfloat nforce[3], GLfloat colour[3]);
	
	list< cParticle* > particles;
	float velocityrandomness;
	float liferandomness;
	GLfloat position[3];
	GLfloat velocity[3];
	GLfloat force[3];
	
	GLfloat colour[3];
	
	using iObject::deleteme;
	//using iObject::modelviewmatrix;
	
	void update();
	void draw();
	
	//temporary
	cTexture* defaulttexture;

protected:
	void initparticle(list< cParticle* >::iterator it);
};

#endif /*CPARTICLESYSTEM_H_*/

// notes: particles are fairly smart, but we can exert a force on them by changing their velocity vector - 
// they don't update that, and only use it to change their position by a constant amount each frame.
// if we change the velocity by a constant amount each frame the particles will follow a parabola.
