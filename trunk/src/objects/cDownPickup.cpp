#include "cDownPickup.h"

cDownPickup::cDownPickup(int nlane, int ncolour) : cPickup() {
	
	colour = ncolour;
	lane = nlane;
	mode = DOWN;
	
	x = -2+(nlane*2);
	z = -400;
	y = 0;
	
	yvel = 0;
	zvel = 0.02;
}

cDownPickup::~cDownPickup() {
	
}

void cDownPickup::update() {

	z += zvel*dtime*speed;
	
}
