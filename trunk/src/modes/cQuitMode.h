#ifndef CQUITMODE_H_
#define CQUITMODE_H_

#include "iGameMode.h"

class cQuitMode : public iGameMode {
public:
	cQuitMode(const char* name);
	virtual ~cQuitMode();
	
	//int init(){ return CONTINUE;}
	string mainloop(){ return "quit";}
};

#endif /*CQUITMODE_H_*/
