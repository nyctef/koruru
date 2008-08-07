#include "cRandomMode.h"

cRandomMode::cRandomMode(const char* name) : cPlayMode(name) {
}

cRandomMode::~cRandomMode()
{
}

int cRandomMode::init () {
	InitMode(RESET_MODE | GAME_MODE);
	score = 0;
	speed = 1.5*bpm/60;
	if (createpickup_thread == NULL) createpickup_thread = SDL_CreateThread(generate_pickups_thread, (void*)this);
	return 0;
}
