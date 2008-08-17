#ifndef INCLUDES_H_
#define INCLUDES_H_

// in here we put everything that only depends on external libs, to avoid dependency problems.

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include "../objects/c3DSModel.h"
#include "../objects/cScore.h"
#include "../objects/cTextPane.h"

#include <iostream>
using namespace std;

extern cScore score;
extern Uint32 dtime;
extern GLuint dlDrawQuad;
extern GLuint texture;
extern float speed;
extern GLfloat shipmatrix[16];

extern c3DSModel* ship;
extern c3DSModel* block;

enum list_colours {NONE=0, RED=1, YELLOW=2, GREEN=3, BLUE=4, PURPLE=5, WHITE=6, BLACK=7, MONO=8};
enum shades {LIGHT=0, MED=1, DARK=2};
extern GLfloat monocolour[3];
extern GLfloat colours[9][3][3];
unsigned random_int(unsigned low, unsigned high);
bool init_mode(long long mode);
bool check_errors();
void draw_frame(); // definition in main.cpp
string get_audio_format(Uint32 format);
void ddot();
/* constants */

extern Uint32 	SCREEN_HEIGHT;
extern Uint32 	SCREEN_WIDTH;
extern int     	SCREEN_BPP;
extern int 		DEFAULT_COLOUR;

//mode constants
#define NORMAL_PROJECTION 			(1<<0)
#define MENU_MODE					(1<<1)
#define	GAME_MODE					(1<<2)
//#define MENU_LIGHTING				(1<<3) // these are a bit redundant at the moment, might use later.
//#define GAME_LIGHTING				(1<<4) //
#define RESET_MODE					(1<<5)

#endif 
