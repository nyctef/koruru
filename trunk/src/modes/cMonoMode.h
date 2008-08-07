#ifndef CMONOMODE_H_
#define CMONOMODE_H_

#include "cPlayMode.h"

#define MONO_TRACK_WIDTH 5

class cMonoMode : public cPlayMode {
public:
	cMonoMode(const char* name);
	virtual ~cMonoMode();
	
	int init();
	void drawship();
	//void generatepickupsequence();
	void drawtrack();
	void CreatePickups();
	
	void changemonocolour();
	
	using cPlayMode::pickupcount;
};

#endif /*CMONOMODE_H_*/
