#include "main.h"

// these are pretty much global. 
cScore score;
extern Uint32 dtime;
GLuint dlDrawQuad;
float speed;
GLuint bpm;
cTextPane debug_pane;

// these shouldn't need to be global, but will be a bit more difficult to integrate (should get done for sake of cleanliness)
c3DSModel* ship_model;
c3DSModel* block;
c3DSModel* track;
GLfloat shipmatrix[16] = {0};
 
Uint32 	SCREEN_HEIGHT = 800;
Uint32 	SCREEN_WIDTH = 1280;
int     SCREEN_BPP = 32;

int main() { 
	try {		
	    //Initialize
	    if( Init() == false ) {
	        return 1;    
	    }
		
		map<string, iGameMode*> modes;
		
		modes["quit"] = new cQuitMode("quit");
		modes["default_random_game"] = new cRandomMode("default_random_game");
		modes["default_mono_game"] = new cMonoMode("default_mono_game");
		cMenuMode* main_menu = new cTitleMenuMode("Main menu", "main_menu");
		modes["main_menu"] = main_menu;
			main_menu->additem("endurance", "default_random_game");
			main_menu->additem("mono", "default_mono_game");
			main_menu->additem("gametype 3", "continue");
			main_menu->additem(new cMenuSpacer());
			main_menu->additem("credits", "continue");
			main_menu->additem("quit", "quit");
		
		
		string nextmode = "main_menu";
		while (nextmode != "quit") {
		
			nextmode = modes[nextmode]->mainloop();	
		}
		
		// deinit
		SDL_WM_GrabInput(SDL_GRAB_OFF);
		SDL_ShowCursor(1);
		SDL_Quit();
		
		return 0;
		
		
	} catch (char* e) {
		cout << e << endl;
		return 1;
	} catch (const GLubyte* e) {
		cout << (const char*)e << endl;	
		return 1;
	}
}

bool Init() { /// Initialises game state.
	
	
    //Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )  return false;
    if( SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL ) == NULL )  return false;
	
    SDL_WM_SetCaption( "Game", NULL );
    //SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(0);
    
    //Initialize OpenGL
    if( InitGL() == false ) return false;
    glClearColor( 0.5, 0.5, 0.5, 1 ); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   
    debug_pane = *(new cTextPane(new cFont("data/font2.png", 6, 16, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}[]()<>*+-=/#_%^@\\&|~?!'\".,;:"),
							 15, SCREEN_HEIGHT - 25, 0, 0)); 
	
	debug_pane << "Initialising .. ";
	debug_pane.draw();
    draw_frame();
    
    //while (1) {}
    
    // load ship model
    try { 
    	ship_model = new c3DSModel("data/ship1.3ds"); 
    	block = new c3DSModel("data/block1.3ds"); 
    	track = new c3DSModel("data/track1.3ds");
    } catch(string error_str) {
		cerr << "Error: " << error_str << std::endl;
		exit(1);
	}
    
    //set the global variables
    dtime = 0;
	bpm = 120;
	score = 0; 
    
    debug_pane << "done\n";
	draw_frame();
    
    return true;    
}

bool InitGL() {	/// Init tasks related to OpenGL. Called only by Init().
	
	init_mode(RESET_MODE|MENU_MODE);
    
    //Set up a display list for drawing a quad
    dlDrawQuad = glGenLists(1); 
    glNewList(dlDrawQuad, GL_COMPILE); 
			glBegin(GL_QUADS);
			  glTexCoord2f(1, 0);
			 glVertex3f( 1,-0.01,-1);
			  glTexCoord2f(1, 1);
			 glVertex3f( 1,-0.01, 1);
			  glTexCoord2f(0, 1);
			 glVertex3f(-1,-0.01, 1);
			  glTexCoord2f(0, 0);
			 glVertex3f(-1,-0.01,-1);
			glEnd();
    glEndList();
    
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	
	check_errors();

    return true;
}

void draw_frame() { // declaration in includes.h

	debug_pane.draw();
	
	SDL_GL_SwapBuffers();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
}
