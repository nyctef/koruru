#ifndef __IOBJECT_H__
#define __IOBJECT_H__

#include "../utils/includes.h"

/** \brief Provides a basic object interface.
 * 
 * Creates an interface for all objects that need to be updated and drawn.
 * Used for lists of objects.
 */
class iObject {
public:
	/** \brief Delete flag
	 * 
	 * Set to true for object to be cleaned up by whatever controls it.
	 */
	bool deleteme; 
	/** \brief The current modelview transformation for the object.
	 * 
	 * Used when the object's global coordinates are needed, for 
	 * example in collisions and such. Calculated when the object is drawn, 
	 * since it will be under the modelview transformation by the time the 
	 * control gets to the object's draw function.
	 */
	GLfloat* modelviewmatrix; 
	virtual void update() = 0;
	virtual void draw() = 0;
	iObject();
	virtual ~iObject();
};

#endif 
