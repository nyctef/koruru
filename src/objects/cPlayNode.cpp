#include "cPlayNode.h"

void cPlayNode::update() {
	
	if (SDL_GetTicks() >= timerstart + timertime) {
		switch (timertrigger) {
			case EXPLODE:
				unstable = 1;
				break;
		}
	}
}

void cPlayNode::light(Uint32 time) {
	
	if ( !((colour == WHITE) && lit) ) { //whites can't get reset.
	 timerstart = SDL_GetTicks();
	 timertrigger = EXPLODE;
	 timertime = time/speed;
	 lit = 1;
	}	
}

cPlayNode::cPlayNode() {
	colour = timerstart = timertime = timertrigger = unstable = 0;
	marked = lit = false;
}

cPlayNode::cPlayNode(int i) {
	timerstart = timertime = timertrigger = unstable = 0;
	colour = i;
	marked = lit = false;
}

cPlayNode::~cPlayNode() {

}

ostream& operator<< (ostream& os, cPlayNode& node) {
		os << node.colour;
		return os;
}
