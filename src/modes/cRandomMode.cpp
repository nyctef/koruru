#include "cRandomMode.h"

cRandomMode::cRandomMode(const char* name) : cPlayMode(name) {
}

cRandomMode::~cRandomMode()
{
}

int cRandomMode::init () {
	
	cPlayMode::init();
	
	speed *= 2;
	
	return 0;
}

void cRandomMode::mode_specific_updates() {}
