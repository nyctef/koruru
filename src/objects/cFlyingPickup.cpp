#include "cFlyingPickup.h"

cPickup::cPickup() {

}

void cPickup::update() {
	 
}

cPickup::~cPickup() {
}

void cPickup::draw() {

	glPushMatrix();
	glTranslatef(x, y, z);
	// remember where we are now
	glGetFloatv(GL_MODELVIEW_MATRIX, modelviewmatrix);
	glColor3fv(colours[this->colour][0]);
	glScalef(0.7,0.7,0.7);
	block->Draw();
	glPopMatrix();
		
}
