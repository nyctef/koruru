#ifndef MAIN_H_
#define MAIN_H_

#include "GL/glew.h"

#include "utils/includes.h"

#include "SDL/SDL_image.h"
#include "objects/gui/cFont.h"
#include "modes/cRandomMode.h"
#include "modes/cTitleMenuMode.h"
#include "modes/cQuitMode.h"
#include "modes/cMonoMode.h"
#include "objects/cScore.h"
#include "objects/gui/cDynamicLabel.h"
#include "objects/gui/cMultilineLabel.h"

#include "ffmpeg/avformat.h"
#include "SDL/SDL_mixer.h"

#include "audio/cAudioFileState.h"

#include <cmath>
#include <string>
#include <cstdio>
#include <map>

bool Init();
bool InitGL();
//extern GLfloat shipbank;

#endif /*MAIN_H_*/
