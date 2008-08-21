#include "main.h"

// these are pretty much global. 
cScore score;
extern Uint32 dtime;
GLuint dlDrawQuad;
float speed;
GLuint bpm;
cTextPane debug_pane;
cLabel version_label;

SDL_AudioSpec* spec,* wanted_spec;

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
		
	    //Initialise
	    if( Init() == false ) {
	        return 1;    
	    }
		
		debug_pane << "Loading modes ... \n"; draw_frame();
		
		map<string, iGameMode*> modes;
		
		modes["quit"] = new cQuitMode("quit"); 									ddot();
		modes["default_random_game"] = new cRandomMode("default_random_game"); 	ddot();
		modes["default_mono_game"] = new cMonoMode("default_mono_game"); 		ddot();
		cMenuMode* main_menu = new cTitleMenuMode("Main menu", "main_menu"); 	ddot();
		modes["main_menu"] = main_menu;											ddot();
			main_menu->additem("endurance", "default_random_game"); 			ddot();
			main_menu->additem("mono", "default_mono_game"); 					ddot();
			main_menu->additem("gametype 3", "continue"); 						ddot();
			main_menu->additem(new cMenuSpacer()); 								ddot();
			main_menu->additem("credits", "continue"); 							ddot();
			main_menu->additem("quit", "quit"); 								ddot();
		
		debug_pane << "done\n"; draw_frame();
		
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
		 cout << gluErrorString(glGetError());
		return 1;
	} catch (const GLubyte* e) {
		
		cout << (const char*)e << endl;
	   cout << gluErrorString(glGetError());
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
    
    cFont* small_font = new cFont("data/font2.png", 6, 16, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789{}[]()<>*+-=/#_%^@\\&|~?!'\".,;:");
   
    debug_pane = *(new cTextPane(small_font, 15, SCREEN_HEIGHT - 25, 0, 0));
							 
	version_label = *(new cLabel("Koruru version 0.1alpha", small_font, SCREEN_WIDTH - 150, SCREEN_HEIGHT - 50));
	
	debug_pane << "Initialising .. \nChecking GLEW ...\n";
	
	debug_pane << "OpenGL 1.1" << (GLEW_VERSION_1_1 ? " " : " not " ) << "supported\n";
	debug_pane << "OpenGL 2.0 (shaders)" << (GLEW_VERSION_2_0 ? " " : " not " ) << "supported\n";
	debug_pane << "framebuffer objects" << (GLEW_EXT_framebuffer_object ? " " : " not " ) << "supported\n";
	debug_pane << "\n....................\n\n";
	
	debug_pane.draw();
    draw_frame();
    
    // load audio
    
    av_register_all();
    
    wanted_spec = new SDL_AudioSpec;
	spec = new SDL_AudioSpec;
    
	// Set audio settings
   	wanted_spec->freq = 44100;
   	wanted_spec->format = AUDIO_S16SYS;
   	wanted_spec->channels = 1;
   	wanted_spec->samples = SDL_AUDIO_BUFFER_SIZE;
    
    spec->freq = 0;
    spec->format = 0;
    spec->channels = 0;
    spec->samples = 0;
    
	debug_pane << "ffmpeg/SDL wrapper [" << __DATE__ << " " << __TIME__ << " build] by jtg\n\n";
	
    debug_pane << "Wanted spec: " << wanted_spec->freq << "Hz " << get_audio_format(wanted_spec->format) << " over " 
		<< (int)wanted_spec->channels << " channels with " << wanted_spec->samples 
		<< " as the buffer size.\n";

    if (Mix_OpenAudio(wanted_spec->freq, wanted_spec->format, wanted_spec->channels, wanted_spec->samples) == -1) throw SDL_GetError();
    Mix_QuerySpec(&spec->freq, &spec->format, (int*)&spec->channels);
    
   	debug_pane << "Got:         " 
		<< spec->freq << "Hz " << get_audio_format(spec->format) << " over " 
		<< (int)spec->channels << " channels\n";
	
	// start the audio running (unpause it)
	SDL_PauseAudio(0);
		
    // load models
	debug_pane << "loading models ..";
	ship_model = new c3DSModel("data/ship1.3ds"); ddot();
	block = new c3DSModel("data/block1.3ds"); ddot();
	track = new c3DSModel("data/track1.3ds"); ddot();
    
    //set some global variables
    dtime = 0;
	bpm = 120;
	score = 0; 
    
    debug_pane << "done and done\n";
	draw_frame();
    
    return true;    
}

bool InitGL() {	/// Init tasks related to OpenGL. Called only by Init().
	
	//debug_pane << "GL init ... \n";
	
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
	
	glewInit();
	
	check_errors();

    return true;
}

void draw_frame() { // declaration in includes.h

//#ifdef DEBUG

	// calc fps.
    static Uint32 frametime = SDL_GetTicks(); static Uint32 frames = 0;                 
    frames++; if (SDL_GetTicks()-frametime >= 5000) {debug_pane << frames << " fps.\n"; frames = 0; frametime = SDL_GetTicks();}
     
	debug_pane.draw();
	
	version_label.draw();
	
//#endif // def DEBUG

	SDL_GL_SwapBuffers();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
}
