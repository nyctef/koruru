#include "cScorePickup.h"

cScorePickup::cScorePickup(int nlane, int height, int ncolour) : cPickup() {
	
	colour = ncolour;
	mode = SCORE;
	lane = nlane;
	
	x = -2+(nlane*2);
	z = -3*height;
	y = 0;
	
	zvel = yvel = y2vel = 0;
}

cScorePickup::~cScorePickup() { 
	
}

void cScorePickup::update() {

	zvel += -0.0015*dtime*speed; z+= zvel;
	y2vel += 0.0000002*dtime*speed;
	yvel +=  y2vel*dtime*speed; y+= yvel;
	
}
