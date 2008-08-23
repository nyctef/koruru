#ifndef MDRAWTEXT_H_
#define MDRAWTEXT_H_

#include "cFont.h"
#include "../iBindable.h"

class mDrawText {
public:
	mDrawText();
	virtual ~mDrawText();
	
	void render_to_bindable(string text, cFont* font, int x_pos, int y_pos, iBindable* buffer);
};

#endif /*MDRAWTEXT_H_*/
