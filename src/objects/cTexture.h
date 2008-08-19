#ifndef CTEXTURE_H_
#define CTEXTURE_H_

//#include "../utils/includes.h"
#include "GL/glew.h"

#include "SDL/SDL_image.h"

#include <iostream>
using namespace std;

/** \brief A simple wrapper for texture functions.
 * 
 * Later (or derivative) versions of this class will probably support
 * multitexturing, animated textures etc.
 */
class cTexture {
public:
	/** \brief Use SDL to load the texture from a file */
	cTexture(string file);
	/** \brief Create a blank texture with the selected dimensions */
	cTexture(int width, int height);
	/** \brief Create a texture object we can attach a texture to later. */
	cTexture();
	virtual ~cTexture();
	
	/** \brief Bind the texture so that it will be used for drawing polygons. */
	void bind();
	
	GLuint get_texid() {return texid;}
	
	long width;
	long height;

protected:
	/** \brief Utility function to load an SDL texture/surface from memory into an OpenGL texture. */
	GLuint load_texture_from_surface(SDL_Surface* surface);
	/** \brief The texid that OpenGL gives us to work with.*/
	GLuint texid;
};

#endif /*CTEXTURE_H_*/
