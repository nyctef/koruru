#ifndef CMENUITEM_H_
#define CMENUITEM_H_

#include "../modes/iGameMode.h"
#include "c3DSModel.h"
#include "cFont.h"
#include "../utils/utils.h"

/** \brief Holds data about a particular menu item.
 * 
 * Fairly dumb, struct-y class. 
 * Maybe derive some classes for other controls (sliders, text entry etc).
 * 
 * TODO: add mouse control somewhere.
 */
class cMenuItem {
public:
	cMenuItem(string name);
	cMenuItem(string name, c3DSModel* model, string target);
	virtual ~cMenuItem();
	
	string name;
	bool active;
	bool clicked;
	
	virtual void draw();
	c3DSModel* model;
	string target;
	
//protected:
	void init(string name, c3DSModel* model, string target);
	cFont* font;
};

#endif /*CMENUITEM_H_*/
