#ifndef CSCORE_H_
#define CSCORE_H_

#include <cmath>
#include "SDL/SDL.h" // for Uint32 and SDL_GetTicks();
#include <iostream>
using namespace std;



class cScore {
public:
	cScore();
	virtual ~cScore();
	
	void reset();
	void add(long amount);
	void subtract(long amount);
	unsigned long get();
	
	void update();
	void draw();
	
	void operator+=(long rhs);
	void operator-=(long rhs);
	bool operator< (long rhs);
	void operator= (long rhs);
	
private:
	/** \brief The currently-displayed score */
	long score;
	/** \brief The score we're going for */
	long target;
	/** \brief How many score changes are happening (ie our speed) */
	int speed;
};

#endif /*CSCORE_H_*/
