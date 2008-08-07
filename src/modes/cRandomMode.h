#ifndef CRANDOMMODE_H_
#define CRANDOMMODE_H_

#include "cPlayMode.h"

class cRandomMode : public cPlayMode {
public:
	cRandomMode(const char* name);
	virtual ~cRandomMode();
	
	int init();
};

#endif /*CRANDOMMODE_H_*/
