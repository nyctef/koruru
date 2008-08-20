#ifndef CEVENT_H_
#define CEVENT_H_

#include "SDL/SDL.h"

class cEvent {
public:
	cEvent(SDL_Event event);
	virtual ~cEvent();
	
	SDL_Event event;
};

#endif /*CEVENT_H_*/
