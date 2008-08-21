#ifndef CLABEL_H_
#define CLABEL_H_

#include "cWindow.h"

class cLabel : public cWindow {
public:
	cLabel():cWindow("",0,0,0,0){};
	cLabel(string text, cFont* font, int x_pos, int y_pos);
	virtual ~cLabel();
};

#endif /*CLABEL_H_*/
