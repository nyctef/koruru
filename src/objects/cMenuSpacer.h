#ifndef CMENUSPACER_H_
#define CMENUSPACER_H_

#include "cMenuItem.h"

/** \brief Blank cMenuItem */
class cMenuSpacer : public cMenuItem {
public:
	cMenuSpacer() : cMenuItem("") {};
	virtual ~cMenuSpacer();
	
	void draw() {}
};

#endif /*CMENUSPACER_H_*/
