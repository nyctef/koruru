#ifndef __UTILS_H__
#define __UTILS_H__

#include "includes.h"

//#include "cObjectManager.height"
#include "SDL/SDL_image.h"

#include <vector>

SDL_Rect* CreateRect(int xpos, int ypos, int height, int width);
SDL_Color* CreateColour(Uint8 r, Uint8 g, Uint8 b);
SDL_Surface* SubSurface(SDL_Surface* metaSurface, int x, int y, int width, int height);
void dump(SDL_Surface* surface);
void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
Uint32 GetPixel(SDL_Surface *surface, int x, int y);
GLfloat* TransformPointf(GLfloat* matrix, GLfloat* point); // takes a 4x4 matrix and a 4x1 vector.  
GLfloat* TransformPoint3fv(GLfloat* m, GLfloat p[3]); // takes a 4x4 matrix and a 3x1 vector.
void PrintMatrix(GLfloat* m); // 4x4 float matrix

extern Uint32 dtime; // the time (in ms) at the beginning of the frame.
extern float deltatime; // the fraction of a second since the beginning of the frame.


#endif
