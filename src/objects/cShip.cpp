#include "cShip.h"

cShip::cShip(c3DSModel* nmodel, unsigned nbpm) {
	xpos = 0.5;
	bank = bob = bobvel = 0;
	model = nmodel;
	bpm = nbpm;
	respawntime = 5000;
	respawning = false;
}

cShip::~cShip() {
}

void cShip::update() {
	bob += bobvel * dtime * speed;
	bobvel -= 0.0001;
	if (bob < 0) {
		bob = 0;
		bobvel = 0;
	}
	if (respawning && (SDL_GetTicks() - respawnstart >= respawntime)) {
		respawning = false;
	} 
}

void cShip::draw() {
	glPushMatrix();
		glTranslatef(0, 0.5+0.07*sin((float)SDL_GetTicks()*bpm/(float)30000) + bob, -6);
		if (respawning) {
			glTranslatef(0.07*sin((float)SDL_GetTicks()*bpm/(float)3000) + bob, 0.3, 0);
			glColor4f(1,1,1, 0.2);
		} else {
			glColor3fv(colours[WHITE][0]);
		}
		glRotatef(-90,1,0,0);
		glRotatef(90*bank, 0,1,0);
		glScalef(0.15,0.10,0.15);
		glGetFloatv(GL_MODELVIEW_MATRIX, modelviewmatrix);
		model->Draw();
	glPopMatrix();
}

void cShip::bump() {
	bob = 0;
	bobvel = 0.0005;
}

void cShip::respawn() {
	respawnstart = SDL_GetTicks();
	respawning = true;
}
