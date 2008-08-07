#ifndef CMENUMODE_H_
#define CMENUMODE_H_

#include "iGameMode.h"
#include "../objects/cMenuItem.h"
#include "../objects/cMenuSpacer.h"
//#include "cTestMode.height"
#include "../utils/includes.h"

#include <vector>
using namespace std;

/** \brief A basic menu mode.
 * 
 * Displays a list of items, with target modes and titles for each.
 * 
 * We build a menu by first constructing a menu object and then calling additem() 
 * for each menu item we want, before calling init() then mainloop().
 * 
 * TODO: more transitions and stuff.
 */
class cMenuMode : public iGameMode {
public:
	cMenuMode(string title, string name);
	virtual ~cMenuMode();
	
	void additem(cMenuItem*);
	void additem(string title, string target);
	
	//int init();
	string mainloop();
	
protected:
	string title;
	vector< cMenuItem* > menuitems;
	vector< cMenuItem* >::iterator it;
	vector< cMenuItem* >::iterator currentitem;

	void moveup();
	void movedown();
	string select();
	
	int loadmode(iGameMode* newmode);
	
	string handleevents();
	void draw();
	void draw_cursor(unsigned x, unsigned y);
	unsigned mousex, mousey;
	
	using iGameMode::event;
};

#endif /*CMENUMODE_H_*/
