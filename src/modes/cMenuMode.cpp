#include "cMenuMode.h"
	
cMenuMode::cMenuMode(string ntitle, string name) : iGameMode(name), title(ntitle) {
	
	mousex = mousey = 0;
	parent = "quit";
}

cMenuMode::~cMenuMode() {
}

void cMenuMode::additem(string title, string target) {

	cMenuItem* tmpmenu = new cMenuItem(title, NULL, target);
	menuitems.push_back(tmpmenu);
	
}

void cMenuMode::additem(cMenuItem* tmpmenu) {

	menuitems.push_back(tmpmenu);
		
}

string cMenuMode::mainloop() {
	debug_pane << "Loading menu ... \n";
	draw_frame(); 
	
	string tmpmode;
	
	init_mode(RESET_MODE | MENU_MODE );
	currentitem = menuitems.begin();
	(*currentitem)->active = true;
	
	for (unsigned i=1; i<menuitems.size(); i++) {
		menuitems[i]->active = false;
	}
	
	while (1) {
		
		if ((tmpmode = handleevents()) != "continue") { 
			return tmpmode; 
		}
		draw();
		draw_frame();
		
		//frames++; if (SDL_GetTicks()-dtime >= 1000) {cout << frames << " fps." << endl; frames = 0; dtime = SDL_GetTicks();} // calc fps.
	}
	return "quit";
}

void cMenuMode::moveup() {
	if (currentitem != menuitems.begin()) {
		(*currentitem)->active = false;
		currentitem--;
		if ((*currentitem)->name == "") moveup(); // skip blank entries. Note that this will fail if the beginning or end of a menu is blank.
		(*currentitem)->active = true;
	}
}

void cMenuMode::movedown() {
	if (currentitem+1 != menuitems.end()) { // remember, .end() points just *beyond* the end of the vector.
		(*currentitem)->active = false;
		currentitem++;
		if ((*currentitem)->name == "") movedown(); // skip blank entries. Note that this will fail if the beginning or end of a menu is blank.
		(*currentitem)->active = true;
	}
}

string cMenuMode::select() {
	
	(*currentitem)->clicked = true;
	draw();
	SDL_Delay(100);
	(*currentitem)->clicked = false;
	if ((*currentitem)->target != "") {
		// load up the target mode, run it
		return (*currentitem)->target;
	} else {
		return "continue";
	}
}

string cMenuMode::handleevents() {

	while (SDL_PollEvent (&event)) {
        switch (event.type) {
          case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:
                	return parent;
                    break;
                case SDLK_UP:
                	moveup();
                	break;
                case SDLK_DOWN:
                	movedown();
                	break;
                case SDLK_SPACE:
                	return select();
                	break;
                default:
                	break;
            }
	      case SDL_MOUSEMOTION:
	      		mousex = event.motion.x; mousey = event.motion.y;
	            break; 
          case SDL_QUIT:
          	return "quit";
		    break;
          default:
		    break;
        }
	}
	return "continue";
}

void cMenuMode::draw() {
	
	glLoadIdentity();
	gluLookAt(0,0,20, 0,0,0, 0,1,0);
	
	//draw_cursor(mousex, mousey);
	
	glRotatef(30, 0, 1, 0); 
	
	glTranslatef(-7, 5, 0);

	for (it=menuitems.begin(); it!=menuitems.end(); it++) {
		glPushMatrix();
		 (*it)->draw();
		glPopMatrix();
		glTranslatef(0, -2, 0);
		
	}
}

void cMenuMode::draw_cursor(unsigned x, unsigned y) {
	glPushMatrix();
	GLdouble mvmatrix[16] = {0};
	GLdouble pjmatrix[16] = {0};
	GLint    viewport[4] = {0};
	
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,pjmatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	GLdouble result[3] = {0};
	
	gluUnProject(x, y, 0.01, 
	             mvmatrix, pjmatrix, viewport,
	             &result[0], &result[1], &result[2]);
	             
	//if (result[2] < 10)
	
	
	//cout << result[0] << " " << -result[1] << " " << result[2] << endl;
	
	glScalef (0.2,0.2,0.2);
	glScalef (0.2,0.2,0.2);
	
	glBegin(GL_LINES);
	
		glVertex3f(0, 0, -10);
		glVertex3f(0, 0, 10);
		
	glEnd();
	glPopMatrix();
	
}
