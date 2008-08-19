#ifndef CDOWNPICKUP_H_
#define CDOWNPICKUP_H_

#include "cPickup.h"

class cDownPickup : public cPickup {
public:
	cDownPickup(int lane, int colour);
	virtual ~cDownPickup();
	
	virtual void update();
};

#endif /*CDOWNPICKUP_H_*/
