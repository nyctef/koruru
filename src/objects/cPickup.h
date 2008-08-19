#ifndef CPICKUP_H_
#define CPICKUP_H_

#include "../utils/includes.h"
#include "iObject.h"
#include "../utils/utils.h"
#include "../utils/collisions.h"

enum flymodes { DOWN = 1, SCORE = 2, DROP = 3, TOPLAYTABLE = 4 };

/** \brief Encapsulates one pickup.
 */
class cPickup : public iObject {
public:
	/** \brief X position */
	GLfloat x; 
	/** \brief Y position */
	GLfloat y;
	/** \brief Z position */
	GLfloat z; 	
	/** \brief Y velocity */					 
	GLfloat yvel; 
	/** \brief Z velocity */
	GLfloat zvel;
	/** \brief Pickup colour */
	int colour; 
	/** \brief What the pickup is doing */
	int mode;
	/** \brief The lane the pickup is in */
	int lane;
	
	/** \brief update the pickup state */
	virtual void update();
	/** \brief Render the pickup */
	virtual void draw();
	
	cPickup();
	virtual ~cPickup();
};

#endif /*CPICKUP_H_*/
