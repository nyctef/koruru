#ifndef CGAMEMODE_H_
#define CGAMEMODE_H_

#include "SDL/SDL.h"
#include <string>
using namespace std;
// some constants for functions like handleevents and mainloop
// todo: get rid of these, they're pretty broken. try to implement a proper state machine.
/*#define QUIT_EXIT 1
#define QUIT_TO_MENU 2
#define QUIT_ONE_LEVEL 3
#define CONTINUE 4*/

class iGameMode {
public:
	iGameMode(string name);
	virtual ~iGameMode();
	
	virtual string mainloop() = 0;
	//virtual int init() = 0;
	
	string name;
	string parent;
	
	SDL_Event event;
};

#endif /*CGAMEMODE_H_*/
