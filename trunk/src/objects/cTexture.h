#ifndef CTEXTURE_H_
#define CTEXTURE_H_

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_opengl.h"

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
	/** \brief Load a blank texture */
	cTexture();
	virtual ~cTexture();
	
	/** \brief Bind the texture so that it will be used for drawing polygons. */
	void bind();

protected:
	/** \brief Utility function to load an SDL texture/surface from memory into an OpenGL texture. */
	GLuint load_texture_from_surface(SDL_Surface* surface);
	/** \brief The texid that OpenGL gives us to work with.*/
	GLuint texid;
};

#endif /*CTEXTURE_H_*/
