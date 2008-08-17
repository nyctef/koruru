#ifndef CTEXTPANE_H_
#define CTEXTPANE_H_

#include "cFont.h"
#include "../utils/includes.h"

#include <sstream>
#include <string>
using namespace std;

class cTextPane {
public:
	cTextPane(){};
	cTextPane(cFont* font, int x, int y, int width, int height);
	virtual ~cTextPane();
	
	// implementation added here because templates are funny.
	 template <class T>
	void add(const T& t) { ostringstream ss;ss << t;text += ss.str(); }
	 template <class T>
	void set(const T& t) { clear(); add(t); }
	
	void clear();
	
	virtual void draw();
	
protected:
	string text;
	cFont* font;
	GLfloat x, y, width, height;
};

 template <class T>
cTextPane operator<<(cTextPane& lhs, T rhs) {lhs.add(rhs); return lhs;}

extern cTextPane debug_pane;

#endif /*CTEXTPANE_H_*/
