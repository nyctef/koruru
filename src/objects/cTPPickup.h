#ifndef CTPPICKUP_H_
#define CTPPICKUP_H_

#include "cPickup.h"
#include "cPlayTable.h"
/** \brief "ToPlaytable" Pickup - goes from the ship to the table.
 * 
 * Basically, this pickup is different from others in that it interpolates between 
 * two positions over a fixed amount of time. This seems the easiest way for this
 * pickup.
 */
class cTPPickup : public cPickup {
public:
	cTPPickup(int nlane, int height, int ncolour, cPlayTable* pt);
	virtual ~cTPPickup();
	
	GLfloat orig_pos[3];
	GLfloat target_pos[3];
	cPlayTable* pt;
	Uint32 start_time;
	
	virtual void update();
	virtual void draw();
	
	int result;
};

#endif /*CTPPICKUP_H_*/
