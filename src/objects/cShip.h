#ifndef CSHIP_H_
#define CSHIP_H_

#include "iObject.h"
#include <cmath>

class cShip : public iObject {
public:
	cShip(c3DSModel* shipmodel, unsigned bpm);
	virtual ~cShip();
	
	GLfloat bank;
	GLfloat xpos;
	GLfloat bob;
	GLfloat bobvel;
	Uint32 respawntime, respawnstart;
	bool respawning;
	c3DSModel* model;
	unsigned bpm;
	
	void update();
	void draw();
	void bump();
	void respawn();
};

#endif /*CSHIP_H_*/
