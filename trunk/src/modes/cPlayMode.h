#ifndef CPLAYMODE_H_
#define CPLAYMODE_H_

#include "../utils/includes.h"
#include "iGameMode.h"
#include "../objects/cTextPane.h"
#include "../objects/iObject.h"
#include "../objects/cScorePickup.h"
#include "../objects/cDownPickup.h"
#include "../objects/cParticleSystem.h"
#include "../objects/cTPPickup.h"
#include "../objects/cShip.h"

#include <cmath>

#define NORMAL_TRACK_WIDTH 8

class cPlayMode : public iGameMode {
public:
	cPlayMode(const char* name);
	virtual ~cPlayMode();
	
	/** \brief player x-axis position from 0-1 */
	//float playerxpos; 
	Uint32 atime;
	//GLfloat shipbank;
	cFont* font;
	//cObjectManager objects;
	GLfloat trackdisp;
	Uint32 newobjecttimer;
	int pickupcount;
	bool quit_createpickup_thread;
	SDL_Thread* createpickup_thread;
	cTextPane* score_pane;
	
	Uint32 start_time;
	
	cShip ship;
	
	/** \brief List of all objects managed by cPlayMode.
	 * 
	 *  This list doesn't contain the definitive pointer 
	 * to any objects, but is useful when we want to loop 
	 * through all objects - we won't have to make different 
	 * loops for each object type.
	 */
	list< iObject* > objectlist;
	/** \brief List of pickups.
	 * 
	 *  This list has the definitive pointer for all pickups.
	 */ 
	list< cPickup* > pickuplist; 
	/** \brief List of particle systems */
	list< cParticleSystem* > particleslist;
	/** \brief Iterator for objectlist */
	list< iObject* >::iterator it;
	/** \brief Iterator for pickuplist */
	list< cPickup* >::iterator pt;
	/** \brief Iterator for particleslist */
	list< cParticleSystem* >::iterator pst;
	
	/** \brief The current pickup colour
	 * 
	 * Used when generating rows of pickups.
	 */ 
	int currentcolour;
	/** \brief The current pickup (lane) position.
	 * 
	 * Similar to currentcolour. Used when generating pickups.
	 */ 
	int currentposition;
	
	/** \brief The playtable for the game
	 * 
	 * See cPlayTable docs for more details. The table is kept here as 
	 * only one is needed, and it gets updated with everything else.
	 */
	cPlayTable playtable;
	
	/** \brief Add a generic object to the manager. 
	 * 
	 * Probably not used much outside of the class. 
	 * Consider making private.
	 */
	void add(iObject* object);	
	/** \brief Add a pickup to the manager. */
	void addpickup(cPickup* pickup);
	void addparticles(cParticleSystem* particles);
	
	int mode;
	
	// create a new game mode by just overriding these if possible.
	virtual void drawtrack();
	virtual void drawship();
	virtual void CreatePickups();
	virtual int  init();
	
	// these functions can be overridden but remember to call the parent function first.
	virtual string mainloop();
	virtual string HandleEvents();
	virtual string update();
	virtual void draw_screen();
	
	// util func
	void DrawText(char* text, int xpos, int ypos);
};

extern Uint32 dtime;

extern c3DSModel* ship_model;
extern c3DSModel* block;
extern c3DSModel* track;

extern GLuint dlDrawQuad;
//extern GLuint texture;
extern float speed;
extern GLfloat shipmatrix[16];
extern GLuint bpm;
//extern cObjectManager objects;

int generate_pickups_thread(void* a);

#endif /*CPLAYMODE_H_*/
