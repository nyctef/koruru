#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

//#include "utils.height"
#include <iostream>
using namespace std;

// all functions return 1 for collision and 0 for no collision
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
int AABBCollision2D(SDL_Rect A, SDL_Rect B); 
bool AABBCollision3D(GLfloat amin[3], GLfloat amax[3], GLfloat bmin[3], GLfloat bmax[3]);
int CircleCollision(unsigned r1, unsigned x1, unsigned y1, unsigned r2, unsigned x2, unsigned y2);
int CircleAABBCollision(unsigned r1, unsigned x1, unsigned y1, SDL_Rect B);

#endif 
