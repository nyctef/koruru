#include "includes.h"

//int numcolours = 8;
GLfloat color[9][3][3] = { {{0.3, 0.3, 0.3},  {0, 0, 0},  {0, 0, 0}},					// none
						   {{1, 0, 0},  {0.8, 0, 0},  {0.5, 0, 0}},						// red
	                       {{1, 1, 0},  {0.8, 0.8, 0},  {0.5, 0.5, 0}},					// yellow
	                       {{0, 1, 0},  {0, 0.8, 0},  {0, 0.5, 0}},						// green
	                       {{0, 0, 1},  {0, 0, 0.8},  {0, 0, 0.5}},						// blue
	                       {{0.8, 0.1, 0.9},  {0.7, 0.075, 0.8},  {0.4, 0.05, 0.5}},	// purple
	                       {{1, 1, 1},  {0.8, 0.8, 0.8}, {0.5, 0.5, 0.5}},				// white
	                       {{0, 0, 0},  {0, 0, 0},  {0, 0, 0}},  						// black
	                       {{1, 0, 0},  {0.8, 0, 0},  {0.5, 0, 0}},						// mono
					     };
					  
GLfloat monocolor[3] = {1, 0, 0};

int DEFAULT_COLOUR = 0;

// apparently this is a nice, fast rng. From http://groups.google.com/group/comp.lang.c/browse_thread/thread/a9915080a4424068/
// not sure if credit needed. Called 'xorshift'. 

unsigned long _rand(void) {
	static unsigned long x=123456789,
	                     y=362436069,
	                     z=521288629,
	                     w=886375123,
	                     v=886756453;
	
	unsigned long t;
 	t=(x^(x>>7)); 
 	x=y; y=z; z=w; w=v;
 	v=(v^(v<<6))^(t^(t<<13)); 
 	return (y+y+1)*v+rand()%2; // +rand%2 since _rand seems to give non-random numbers mod2.
 } 
 
unsigned GenerateRandomNumber( unsigned low, unsigned high) { /// Generate a random integer between the two arguments.
	
	return ((_rand()%(high-low+1))+low);

}

/** \brief Set common GL variables to predetermined sets of values.
 * 
 * Will probably get added to with development - this is a good place to store some boilerplate code.
 *  \param mode 	A bitmask determining what variables to set.
 */
bool InitMode(long long mode) {
	 if (mode & RESET_MODE) {
	    glClearColor( 0, 0, 0, 1 );
	 	glDisable(GL_BLEND);
	 	glDisable(GL_TEXTURE_2D);
	 	glDisable(GL_DEPTH_TEST);
		glDisable(GL_NORMALIZE); 
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0); glDisable(GL_LIGHT1); glDisable(GL_LIGHT2); glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4); glDisable(GL_LIGHT5); glDisable(GL_LIGHT6); glDisable(GL_LIGHT7);
		glDisable(GL_COLOR_MATERIAL);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	 }
	 if (mode & NORMAL_PROJECTION) {
	    glMatrixMode( GL_PROJECTION );
	    glLoadIdentity();
	    gluPerspective(40, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0, 1000.0);
	   
	    glMatrixMode( GL_MODELVIEW );
	    glLoadIdentity();	
		
	}
	if (mode & MENU_MODE) {
		InitMode(NORMAL_PROJECTION);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
	}
	if (mode & GAME_MODE) {
		InitMode(NORMAL_PROJECTION);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_NORMALIZE); 
		
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	    glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	    GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f}; 
	    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
		GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; 
	    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
		GLfloat pos[] = { 0.0, 4.0, -7 };
		glLightfv(GL_LIGHT0, GL_POSITION, pos);	
	}
	CheckErrors();
	return true;
}

bool CheckErrors() {

	GLenum errCode;
	
	if ((errCode = glGetError()) != GL_NO_ERROR) {
	   printf ("Throwing OpenGL Error: %s\n", gluErrorString(errCode));
	   throw gluErrorString(errCode);
	}
	
	return true;
}

