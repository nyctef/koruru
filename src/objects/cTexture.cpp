#include "cTexture.h"

cTexture::cTexture(string file) {
	
	SDL_Surface* surface = IMG_Load(file.c_str());
	if (!surface) throw "loading file failed with IMG_Load()";
	
	texid = load_texture_from_surface(surface);
	
	SDL_FreeSurface(surface);
}

cTexture::cTexture() {
	
	texid = 0;
	
}

cTexture::~cTexture() {
	
	glDeleteTextures(1, &texid);
	texid = 0;
	
}

void cTexture::bind() {
	
	glBindTexture(GL_TEXTURE_2D, texid);

}

GLuint cTexture::load_texture_from_surface(SDL_Surface* surface) {
	
	GLuint new_texid;
	
	glGenTextures(1, &new_texid);
    glBindTexture(GL_TEXTURE_2D, new_texid);
    
    SDL_Surface* tex;
    Uint32 rmask, gmask, bmask, amask;
	
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
    	rmask = 0xff000000;
    	gmask = 0x00ff0000;
    	bmask = 0x0000ff00;
    	amask = 0x000000ff;
	} else {
    	rmask = 0x000000ff;
    	gmask = 0x0000ff00;
    	bmask = 0x00ff0000;
    	amask = 0xff000000;
	}
	
    tex = SDL_CreateRGBSurface(SDL_HWSURFACE, surface->w, surface->h, 32, rmask, gmask, bmask, amask);
    
    SDL_BlitSurface(surface, NULL, tex, NULL);
	 		
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tex->w, tex->h, GL_RGBA, GL_UNSIGNED_BYTE, tex->pixels);
	   					
	SDL_FreeSurface(tex);
	
	width = surface->w;
	height = surface->h;
	
	return new_texid;
}

