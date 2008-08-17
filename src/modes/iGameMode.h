#ifndef CGAMEMODE_H_
#define CGAMEMODE_H_

#include "SDL/SDL.h"
#include <string>
using namespace std;

class iGameMode {
public:
	iGameMode(string name);
	virtual ~iGameMode();
	
	virtual string mainloop() = 0;
	
	string name;
	string parent;
	
	SDL_Event event;
};

#endif /*CGAMEMODE_H_*/
