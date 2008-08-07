#ifndef MAIN_H_
#define MAIN_H_

#include "utils/includes.h"
//#include "objects/cObjectManager.height"
#include "SDL/SDL_image.h"
#include "objects/cFont.h"
#include "modes/cRandomMode.h"
#include "modes/cMenuMode.h"
#include "modes/cQuitMode.h"
#include "modes/cMonoMode.h"
#include "objects/cScore.h"

#include <cmath>
#include <string>
#include <cstdio>
#include <map>

bool Init();
bool InitGL();
//extern GLfloat shipbank;

#endif /*MAIN_H_*/
