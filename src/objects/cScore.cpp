#include "cScore.h"

/** \brief How fast a single bonus will increment the score */ 
#define POINTS_PER_SECOND (300*speed)

cScore::cScore(){ 
	score = 0;
}

cScore::~cScore()
{
}

void cScore::update() {
	static Uint32 dtime = 0;
	static Uint32 atime = SDL_GetTicks();
	
	
	if (score != target) {
		// ((target - score) / abs(target - score)) = 1 if target > score, -1 if target < score
		float increment = (float)speed * ((float)POINTS_PER_SECOND*((float)dtime/1000)) * (float)((target - score) / abs(target - score));
		//cout << score << " " << target << " " << dtime << " " << speed << " " << (POINTS_PER_SECOND*((float)dtime/1000)) << "  " << increment << "\n";
		if (increment > abs(target - score)) {
			score = target;
			speed = 0;
		} else {
			score += increment;
		}
	}
	dtime = SDL_GetTicks() - atime;
	atime = SDL_GetTicks();
}

void cScore::reset(){score = target = 0;}
void cScore::add(long amount){target += amount; speed++;}
void cScore::subtract(long amount){target -= amount; speed++;}
unsigned long cScore::get(){return score;}

void cScore::operator+=(long rhs){add(rhs);}
void cScore::operator-=(long rhs){subtract(rhs);}
bool cScore::operator< (long rhs){return (target < rhs);}
void cScore::operator= (long rhs){target = rhs;}
