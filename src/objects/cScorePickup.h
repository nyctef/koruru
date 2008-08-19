#ifndef CSCOREPICKUP_H_
#define CSCOREPICKUP_H_

#include "cPickup.h"

class cScorePickup : public cPickup {
public:
	cScorePickup(int lane, int height, int colour);
	virtual ~cScorePickup();
	
	float y2vel;
	
	virtual void update();
};

#endif /*CSCOREPICKUP_H_*/
