#ifndef CFONT_H_
#define CFONT_H_

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_image.h"

#include <string>
#include <cstdio>
using namespace std;

#include "cTexture.h"

/** \brief Creates and renders a texture-mapped bitmap fixed-width font.
 */
class cFont {
public:
	/** \brief Default constructor
	 * 
	 * \param file A string constant containing the path to the font file.
	 * \param width The width of one character in pixels.
	 * \param height The height of the font file / one character in pixels.
	 * \param characters A string constant containing the list of characters 
	 * 		  in the font file.  
	 */ 
	cFont(string file, int width, int height, string characters);
	
	/** \brief Draw a string at the current coordinates.
	 * \param str The characters to write.
	 */
	void draw(string str);
	/** \brief Draw all characters at the current coordinates.
	 */
	void Test();
	/** \brief Returns the width in pixels of the given string if using the 
	 * current font.
	 * 
	 * \param str The string to measure
	 */ 
	unsigned string_width(string);

private:	
	/** \brief Width of one block character in the font.*/
	int char_width;    
	/** \brief Height of one block character in the font.*/
	int char_height; 		
	/** \brief The texture handle for our font data.*/
	cTexture* texture; 		
	/** \brief the list of characters in this font.*/
	string characters;  
};

#endif /*CFONT_H_*/
