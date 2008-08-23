#ifndef CLABEL_H_
#define CLABEL_H_

#include "cWindow.h"
#include "cFont.h"


/** \brief A simple (static) text-based label class
 * 
 * This label renders its text to the framebuffer in the constructor, then
 * simply draws the attached texture whenever needed.
 */
class cLabel : public cWindow {
public:
	/** \brief Empty constructor */
	cLabel():cWindow("",0,0,0,0){};
	/** \brief Basic constructor. */
	cLabel(string text, cFont* font, int x_pos, int y_pos);
	virtual ~cLabel();
	
	void render_text(string text, cFont* font);
	
	//cFont* font;
};

#endif /*CLABEL_H_*/
