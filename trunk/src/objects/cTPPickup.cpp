#include "cTPPickup.h"

cTPPickup::cTPPickup(int nlane, int height, int ncolour, cPlayTable* npt) : cPickup() {
	
	lane = nlane;
	colour = ncolour;
	pt = npt;
	mode = TOPLAYTABLE;
	
	x = orig_pos[0] = 0.7*(-3+(nlane*3));
	y = orig_pos[1] = 0.9;
	z = orig_pos[2] = -8;
	
	target_pos[0] = -3+(nlane*3);
	target_pos[1] = 0;
	target_pos[2] = -height*3;
	
	start_time = SDL_GetTicks();
	
	result = -1;
}

cTPPickup::~cTPPickup() { 
	
}

void cTPPickup::update() {

	if (SDL_GetTicks() - start_time >= 500/speed) {
		result = pt->addpickup(lane, colour);
		deleteme = true;	
	}
	
	float u = (SDL_GetTicks() - start_time)/(500/speed);
	x = (1-u)*(orig_pos[0]) + (u)*(target_pos[0]);
	y = (1-u)*(orig_pos[1]) + (u)*(target_pos[1]);
	z = (1-u)*(orig_pos[2]) + (u)*(target_pos[2]);
	
}

void cTPPickup::draw() {
	
	glPushMatrix();
	 glScalef(0.7, 0.7, 0.7);
	 cPickup::draw();
	glPopMatrix();
}
