#include "collisions.h"

// all functions return 1 for collision and 0 for no collision

int AABBCollision2D(SDL_Rect A, SDL_Rect B) {
	//check in the x axis for no collisions
	if (A.x+A.w < B.x) return 0; // A x max less than B x min
	if (B.x+B.w < A.x) return 0; // B x max less than A x min
	
	if (A.y+A.h < B.y) return 0; // and same for y-axis
	if (B.y+B.h < A.y) return 0; 
	
	return 1; // they must collide by now.
}

bool AABBCollision3D(GLfloat amin[3], GLfloat amax[3], GLfloat bmin[3], GLfloat bmax[3]) {
	
/*	cout << (amin[0] > bmax[0]) << " "
	     << (amin[1] > bmax[1]) << " "
	     << (amin[2] > bmax[2]) << " "
	     << (bmin[0] > amax[0]) << " "
	     << (bmin[1] > amax[1]) << " "
	     << (bmin[2] > amax[2]) << " ";*/
	
	if (amin[0] > bmax[0] || amax[0] < bmin[0] ||
	    amin[1] > bmax[1] || amax[1] < bmin[1] ||
	    amin[2] > bmax[2] || amax[2] > bmin[2] )
	    return false;
	    
	    return true;
	
}

int CircleCollision(unsigned r1, unsigned x1, unsigned y1, unsigned r2, unsigned x2, unsigned y2) {
	return 0;
}

int CircleAABBCollision(unsigned r1, unsigned x1, unsigned y1, SDL_Rect* B) {
	return 0;
}
