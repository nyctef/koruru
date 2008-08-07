#ifndef CDOWNPICKUP_H_
#define CDOWNPICKUP_H_

#include "cFlyingPickup.h"

class cDownPickup : public cPickup {
public:
	cDownPickup(int lane, int colour);
	virtual ~cDownPickup();
	
	virtual void update();
};

#endif /*CDOWNPICKUP_H_*/
