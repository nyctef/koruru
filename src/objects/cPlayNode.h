#ifndef CPLAYNODE_H_
#define CPLAYNODE_H_

#include "../utils/includes.h"

enum triggers { EXPLODE = 1 };

/** \brief Controls one section on the playtable.
 */
class cPlayNode {
public:
	
	bool marked;	
	int colour; 
	bool lit; 
	Uint32 timerstart; Uint32 timertime; // |for timers: ~start = SDL_GetTicks() at start and ~time 
    int timertrigger;	 				 // |is the time until the ~trigger happens.
    int unstable;        // if the node is due to explode this frame.  

	void update();
	void light(Uint32 time);
	
	bool operator == (const cPlayNode& node) {return (this->colour == node.colour);}
	bool operator == (const int& i)          {return (this->colour == i);}
	bool operator != (const cPlayNode& node) {return (this->colour != node.colour);}
	bool operator != (const int& i)          {return (this->colour != i);}
	cPlayNode& operator =  (const cPlayNode& node) {this->colour = node.colour; return *this;}
	cPlayNode& operator =  (const int& i)          {this->colour = i; return *this;}
	operator int()              const   {return this->colour;}
	cPlayNode& operator *= (const int& i) {this->colour *= i; return *this;} 
	
	/** \brief Basic constructor
	 * 
	 * \param colour The colour to set the new node to.
	 */
	cPlayNode(int colour);
	/** \brief Default constructor 
	 * 
	 * sets the colour to NONE. 
	 */
	cPlayNode();
	virtual ~cPlayNode();
};

ostream& operator<< (ostream& os, cPlayNode& fraction);


#endif /*CPLAYNODE_H_*/
