#include "utils.h"
//#include "cPlayArea.height"

Uint32 dtime;
float deltatime;

SDL_Rect* CreateRect(int x, int y, int h, int w){
	SDL_Rect* Result = new SDL_Rect;
	Result->x = x; Result->y = y; 
	Result->w = w; Result->h = h; 
	return( Result);
}

SDL_Color* CreateColour(Uint8 r, Uint8 g, Uint8 b) {
	SDL_Color* Result = new SDL_Color;
	Result->r = r; Result->g = g; Result->b = b;
	return Result;	
}

SDL_Surface* SubSurface(SDL_Surface* sourcesurface, int x, int y, int width, int height) { 
	SDL_Rect* area = CreateRect(x, y, width, height); 
	
	// Set the RGBA mask values.
	Uint32 r, g, b, a;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	r = 0xff000000; g = 0x00ff0000;
	b = 0x0000ff00; a = 0x000000ff;
	#else
	r = 0x000000ff; g = 0x0000ff00;
	b = 0x00ff0000; a = 0xff000000;
	#endif
	
	// this is a bit of a mess since alpha doesn't work ; deferring to 3d version since it'll just get rewritten anyway.
	
	SDL_Surface* subsurface = SDL_DisplayFormat(SDL_CreateRGBSurface(SDL_SRCALPHA, width, height, 32, r, g, b, a));
	SDL_BlitSurface(sourcesurface, area, subsurface, 0);
	
	// Return the newly created surface!
	return subsurface;
}

void dump(SDL_Surface *x) {
   Uint8 r, g, b, a;
   SDL_GetRGBA(((Uint32*)x->pixels)[0], x->format, &r, &g, &b, &a);
   printf("RGBA %2x %2x %2x %2x\n", r, g, b, a);
}

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 GetPixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

GLfloat* TransformPointf(GLfloat* m, GLfloat* p) {

	GLfloat* result = new GLfloat[4];

	result[0] = m[ 0]*p[0] + m[ 4]*p[1] + m[ 8]*p[2] + m[12]*p[3]; 
	result[1] = m[ 1]*p[0] + m[ 5]*p[1] + m[ 9]*p[2] + m[13]*p[3]; 
	result[2] = m[ 2]*p[0] + m[ 6]*p[1] + m[10]*p[2] + m[14]*p[3]; 
	result[3] = m[ 3]*p[0] + m[ 7]*p[1] + m[11]*p[2] + m[15]*p[3];

	return result;  
	
}

GLfloat* TransformPoint3fv(GLfloat* m, GLfloat p[3]) {
	
	GLfloat* t = new GLfloat[4];
	t[0] = p[0]; t[1] = p[1]; 
	t[2] = p[2]; t[3] = 1; 

	GLfloat* r = TransformPointf(m, t);
	
	delete [] t;
	
	GLfloat* result = new GLfloat[3];
	result[0] = r[0]; result[1] = r[1]; 
	result[2] = r[2];
	
	delete [] r;
	
	return result;
	
}

void PrintMatrix(GLfloat* m) {
	for (int i=0;i<4;i++) {
		cout << "| ";
		for (int j=0;j<4;j++) {
			cout << m[4*j+i] << " ";
		} cout << "|" << endl;
	} cout << endl;
}

