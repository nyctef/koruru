#include "cParticleSystem.h"

cParticleSystem::cParticleSystem(unsigned numberofparticles) : iObject() {
	
	// see init() docs for these variables
	GLfloat sp[3] = {0,0,0};
	GLfloat nv[3] = {0.001,0.001,0.001};
	GLfloat nvr = 0.5;
	GLfloat nlr = 0.3;
	GLfloat nf[3] = {0,0,0};
	GLfloat nc[3] = {1,1,1};
	init(numberofparticles, sp, nv, nvr, nlr, nf, nc);
	
}

cParticleSystem::cParticleSystem(unsigned numberofparticles, GLfloat nposition[3], GLfloat nvelocity[3], 
                                                             GLfloat nvelocityrandomness, GLfloat nliferandomness,
                                                             GLfloat nforce[3], GLfloat ncolour[3] ) : iObject() {
                                                             	                                                        	
	init( numberofparticles, nposition, nvelocity, nvelocityrandomness, nliferandomness, nforce, ncolour );
}

cParticleSystem::~cParticleSystem() {
	
}

void cParticleSystem::init() {
	init(particles.size(), position, velocity, velocityrandomness, liferandomness, force, colour);
	
}

void cParticleSystem::init(unsigned numberofparticles, GLfloat startposition[3], GLfloat nvelocity[3], 
                           GLfloat nvelocityrandomness, GLfloat nliferandomness, GLfloat nforce[3], GLfloat ncolour[3]) {
                           	
    for (int i=0; i<3; i++) { 
		position[i] = startposition[i];
		velocity[i] = nvelocity[i];
		force[i] = nforce[i];
		colour[i] = ncolour[i];
	} 
	
	velocityrandomness = nvelocityrandomness;
	liferandomness = nliferandomness;
    defaulttexture = new cTexture("data/fuzzy1.png");
	
	list<cParticle*>::iterator it = particles.begin();
	
	for (unsigned i=0;i<numberofparticles;i++) {
		
		particles.push_back(NULL); // add a space for a particle
		
		// go to the new particle
		it = particles.begin(); for (unsigned j=0;j<i;j++) it++; // surely there's a better way than this. 
		                                                         // Maybe I need a vector instead of a list - 
		                                                         // fast inserts/deletes aren't as needed as I thought.
		// init the new particle
		initparticle(it);
		
	}
           	
}

void cParticleSystem::update() {
	
	for (list<cParticle*>::iterator it=particles.begin(); it!=particles.end(); it++) {
	
		for (int i=0;i<3;i++) (*it)->velocity[i] += force[i];
		(*it)->update();
		//if ((*it)->deleteme == true) initparticle(it); //reincarnate the particle
		if ((*it)->deleteme == true) it = particles.erase(it); // let particle die.
	}
	
}

void cParticleSystem::draw() {
	
	for (list<cParticle*>::iterator it=particles.begin(); it!=particles.end(); it++) {
		
		(*it)->draw();
		
	}
	
}


void cParticleSystem::initparticle(list< cParticle* >::iterator it) {
	
	cParticle* tmpparticle;
	 cTexture* default_texture = new cTexture("data/fuzzy1.png");
	float randconst;
	Uint32 life = 2000;
	
	GLfloat nvelocity[3];
	
	
	srand(time(NULL)+rand());
	for (int j=0;j<3;j++) {
		
		randconst = ((float)rand()/(float)RAND_MAX) * velocityrandomness - velocityrandomness/2;
	
		nvelocity[j] = velocity[j] + randconst;
	
	}
	
	randconst = ((float)rand()/(float)RAND_MAX) * liferandomness - liferandomness/2;
	life += life*randconst;
	
	tmpparticle = new cTexturedParticle(position, nvelocity, life, default_texture);
	
	(*it) = tmpparticle;
	
}
