#ifndef CRANDOMMODE_H_
#define CRANDOMMODE_H_

#include "cPlayMode.h"

class cRandomMode : public cPlayMode {
public:
	cRandomMode(const char* name);
	virtual ~cRandomMode();
	
	int init();
	void mode_specific_updates();
};

#endif /*CRANDOMMODE_H_*/
