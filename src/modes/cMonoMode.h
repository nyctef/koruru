#ifndef CMONOMODE_H_
#define CMONOMODE_H_

#include "cPlayMode.h"
#include "../audio/mPlayAudioFile.h"

#define MONO_TRACK_WIDTH 5

/** \brief Basic 'mono' game 
 */
class cMonoMode : public cPlayMode, public mPlayAudioFile {
public:
	cMonoMode(const char* name);
	virtual ~cMonoMode();
	
	// all of these functions are inherited from cPlayMode
	
	int init();
	void drawship();
	void drawtrack();
	void CreatePickups();
	virtual string update();
	
	virtual void exit();
	
	using cPlayMode::pickupcount;
};

#endif /*CMONOMODE_H_*/
