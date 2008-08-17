#include "cPlayMode.h"

cPlayMode::cPlayMode(const char* name) : iGameMode(name), ship(ship_model, bpm) {
	
	font = new cFont("data/font3.png", 14, 32, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]()^£""';:#");
	trackdisp = pickupcount = 0; 
	
	newobjecttimer = SDL_GetTicks();
	quit_createpickup_thread = false;
	
	createpickup_thread = NULL;
	
	score_pane = new cTextPane(font, SCREEN_WIDTH - font->string_width("          ")/2, 40, 0, 0);
	
}

cPlayMode::~cPlayMode() {
	
}


int cPlayMode::init() {
	
	debug_pane.set("Loading new game .. \n");
	draw_frame();
	
	score = 0;
	speed = 1*bpm/60;
	
	debug_pane.add("creating pickup thread \n");
	if (createpickup_thread == NULL) createpickup_thread = SDL_CreateThread(generate_pickups_thread, (void*)this);
		
	// OH EM GEE MEMORY LEAKS
	objectlist.clear();
	pickuplist.clear();
	particleslist.clear();
	
	playtable = *(new cPlayTable(3,7));
	
	srand(time(NULL));
	
	score.reset();
	
	ship.draw();
	
	start_time = SDL_GetTicks();
	
	debug_pane.add("\n                  .. done\n");
	return 0;
}

string cPlayMode::mainloop() {
	
	init();
	
	atime = SDL_GetTicks();
/*	Uint32 frametime = SDL_GetTicks(), frames = 0;                 // ♫ calc fps*/
    
    string tmpquit;
    
	// into our game loop
	while(1) {
   	  
   	  atime = SDL_GetTicks();
      if ((tmpquit = HandleEvents()) != "continue") {
	      return tmpquit;
      }
      
      if ((tmpquit = update()) != "continue") {
	      return tmpquit;
      }
      
      
       draw_screen();
      draw_frame();
      
      SDL_Delay(20);
      
      dtime = SDL_GetTicks() - atime;
      
      //frames++; if (SDL_GetTicks()-frametime >= 1000) {cout << frames << " fps." << endl; frames = 0; frametime = SDL_GetTicks();} // calc fps.
      
	}
	
	quit_createpickup_thread = true;
	SDL_WaitThread(createpickup_thread, NULL);
}
 
/** \brief Loops through waiting events, then takes keyboard state for arrow keys.
 */
string cPlayMode::HandleEvents() { 
	while (SDL_PollEvent (&event)) {
        switch (event.type) {
          case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    return "main_menu";
                    break;
                default:
                    break;
            }
            break;
          case SDL_MOUSEMOTION:
          	ship.bank = -1/(fabs((event.motion.x-ship.xpos*SCREEN_WIDTH)/100)+1)+1;
          	if (ship.xpos*SCREEN_WIDTH - event.motion.x >= 0) ship.bank = -ship.bank;
            ship.xpos = event.motion.x / (float) SCREEN_WIDTH ;
            break; 
          case SDL_QUIT:
            return "quit";
            break;
          default:
            break;
        }
      }
      // check state of keyboard  
      Uint8 *keys = SDL_GetKeyState(NULL);
      if (keys[SDLK_LEFT])  (ship.xpos <= 0) ? ship.xpos = 0 : ship.xpos -= 0.001*dtime;
      if (keys[SDLK_RIGHT]) (ship.xpos >= 1) ? ship.xpos = 1 : ship.xpos += 0.001*dtime;
      
      return "continue";
}

/** \brief Updates game state 
 */
string cPlayMode::update() {
		
	// update all (generic) objects 
	list< iObject* >::iterator it;	
	for(it=objectlist.begin(); it != objectlist.end(); it++) {
   		(*it)->update();
   	 	if ((*it)->deleteme == true) {
   	 		it=objectlist.erase(it);
		}	
   	}
   	
   	ship.update();
   	score.update();
   
	list< cPickup* >::iterator pt;
	for (pt=pickuplist.begin(); pt != pickuplist.end(); pt++) {
	  
		// collide pickups with the ship .. 
		if (((*pt)->modelviewmatrix != NULL ) && ((*pt)->mode == DOWN) && !(ship.respawning)) {
		
			//PrintMatrix(modelviewmatrix);
			GLfloat* abbmin = TransformPoint3fv((*pt)->modelviewmatrix, block->bbmin);
			GLfloat* abbmax = TransformPoint3fv((*pt)->modelviewmatrix, block->bbmax);
			GLfloat* sbbmin = TransformPoint3fv(ship.modelviewmatrix, ship.model->bbmin); 
			GLfloat* sbbmax = TransformPoint3fv(ship.modelviewmatrix, ship.model->bbmax); 
			
			if (AABBCollision3D(abbmin, abbmax, sbbmin, sbbmax)) {
				
				// set up particles
				/*GLfloat startposition[3] = {-8+(*pt)->lane*8, 5, -100};
				GLfloat velocity[3] = {0, 0.001, 0.002};
				GLfloat force[3] = {0, -0.001, 0};
				cParticleSystem* ps = new cParticleSystem(100, startposition, velocity, 0.3, 0.5, force, colour[(*pt)->colour][0]);
				//cout << colour[(*pt)->colour][0][0] << " " << colour[(*pt)->colour][0][1] << " " << colour[(*pt)->colour][0][2] << endl; 
				addparticles(ps);*/
				
				if ((*pt)->colour == WHITE) score -= 500;
				
				// swap pickup for a new pickup that goes to the playtable.
				cPickup* tmpptr = new cTPPickup((*pt)->lane, playtable.getheight((*pt)->lane), (*pt)->colour, &(playtable));
				delete (*pt);
				
				list< iObject* >::iterator it = find( objectlist.begin(), objectlist.end(), (*pt) );
				(*pt) = tmpptr;
				(*it) = (*pt); 
				
				ship.bump();
			}               
		}
	
   	  // .. and the edges of the screen.
	  GLdouble mvmatrix[16] = {0};
	  GLdouble pjmatrix[16] = {0};
	  GLint viewport[4] = {0};
	  glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	  glGetDoublev(GL_PROJECTION_MATRIX, pjmatrix);
	  glGetIntegerv(GL_VIEWPORT, viewport);
	  GLdouble x, y, z;
	  
	  gluProject((*pt)->x, (*pt)->y, (*pt)->z,
	  			mvmatrix, pjmatrix, viewport,
	  			&x, &y, &z);
	  if ((x < 0) || (x > SCREEN_WIDTH) ||
	  	  (y < 0) || (y > SCREEN_WIDTH)) {
		delete (*pt); list< iObject* >::iterator it = find( objectlist.begin(), objectlist.end(), (*pt) );
		pt = pickuplist.erase(pt); 
		if (it != objectlist.end()) {
			objectlist.erase(it);
		} else {
			debug_pane.add("warning: pickup not found in objectlist\n");
		}
	  }
	}
	
	// update our playnodes
	for (int i=0; i<playtable.width; i++) {
		for (int j=0; j<playtable.height; j++) {
			playtable.positions[i][j]->update();
			
			// check for unstable nodes (release them)
			while (playtable.positions[i][j]->unstable == 1) {
				if (playtable.positions[i][j]->colour != WHITE) { 
					cPickup* newpickup = new cScorePickup(i, j, playtable.positions[i][j]->colour);
					addpickup(newpickup);
				}
				playtable.deletepickup(i, j);
			}
		}
	}
	playtable.update();
	for (int i=0; i<playtable.width; i++) {
		if (playtable.statuses[i] == PT_OVERFILL) {
			ship.respawn();
			playtable.statuses[i] = PT_FINE;
		}	
	}
	if (score < 0) score = 0;
	
	return "continue";
}

/** \brief Performs all the drawing functions.
 */
void cPlayMode::draw_screen() {
	
	init_mode(RESET_MODE | GAME_MODE);
	glDisable(GL_TEXTURE_2D);
	
	// reset position
	glLoadIdentity();
	
	//camera transform
	gluLookAt((ship.xpos-0.5)*2, 4, 10,
	          (ship.xpos-0.5)*2, 0, -30,
	          0, 1, 0);
	
	
	//draw the player
	glPushMatrix();
	 drawship();
	glPopMatrix();
	
	 //draw objects	
	 glPushMatrix();	
	   	if (objectlist.size() != 0) {
		  for(it=objectlist.begin(); it != objectlist.end(); it++) {
			glPushMatrix();
			(*it)->draw();
			glPopMatrix();
		  }
		} 
		playtable.draw();
	 glPopMatrix();
	 
	//draw track
	glPushMatrix();
	 drawtrack();
	glPopMatrix();
	 
	 //draw text
	/*char out[20];
	sprintf(out, "%li", score.get()); // %li - long int 
	DrawText(out, (SCREEN_WIDTH - font->string_width(out))/2, SCREEN_HEIGHT-40);*/
	score_pane->set(score.get());
	score_pane->draw();
	
	debug_pane.draw();
	
	//finally, swap out to the screen. (after the function)
	
}

void cPlayMode::drawship() {
	glTranslatef( (-0.5+ship.xpos)*NORMAL_TRACK_WIDTH, 0, 0); // this is here (outside of cShip) 
										//since the track width is different for different modes.
	ship.draw();
}

void cPlayMode::drawtrack() {
	
		glColor3fv(colours[WHITE][2]);
	
		trackdisp += 0.06*dtime*speed;
		if (trackdisp > 18) trackdisp -= 18;
		glTranslatef(0, 0, 20+trackdisp);
		for (int i=0; i<100; i++) {
		  	track->Draw();
		  	glTranslatef(0, 0, -18);
		}
}

void cPlayMode::DrawText(char* text, int xpos, int ypos) { 
	
	glMatrixMode(GL_PROJECTION);
	 glPushMatrix();
	 glLoadIdentity();
	 glOrtho(0,SCREEN_WIDTH,0,SCREEN_HEIGHT,-1, 1);
	glMatrixMode(GL_MODELVIEW);
	 glPushMatrix();
	 glLoadIdentity();
	 glTranslatef(xpos,ypos,0);
	  glDisable(GL_LIGHTING);
	  glDisable(GL_BLEND);
	   font->draw(text);
	  glEnable(GL_BLEND);
	  glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	 glPopMatrix();   
	glMatrixMode(GL_MODELVIEW);
	 glPopMatrix();
}

void cPlayMode::CreatePickups() {
	
	Uint32 newobjecttimer = SDL_GetTicks();
	int pickupcount = 0;
	while (1) {
		if (quit_createpickup_thread) break;
		
		if (SDL_GetTicks() - newobjecttimer >= 250) {
			if (pickupcount == 0) {
				if (random_int(1, 3) == 1) { 
					currentposition = random_int(0,2);
					currentcolour = random_int(1,5);
					pickupcount = random_int (1,2);
	  			}		
			} else {
				// add pickups
				static int onepickup = 0;
				cPickup* newpickup;
				newpickup = new cDownPickup(currentposition, currentcolour); 
				//newpickup = new cPickup(currentposition, 0, 1, DOWN); // use this for all red pickups. Debugging purposes.
				//if (onepickup==0) // use this for only one pickup. Debugging purposes.
				addpickup(newpickup); 
				onepickup = 1;
				pickupcount--;
			}
			newobjecttimer = SDL_GetTicks();
		} 
		SDL_Delay(20);
	}
}

int generate_pickups_thread(void* playmode) {
	cPlayMode* pm = (cPlayMode*)playmode;
	pm->CreatePickups();
	return 0;
}

void cPlayMode::add(iObject* object) {
	objectlist.push_back(object);
}

void cPlayMode::addpickup(cPickup* pickup) {
	pickuplist.push_back(pickup);
	add(pickup);
}

void cPlayMode::addparticles(cParticleSystem* particles) {
	particleslist.push_back(particles);
	add(particles);	
}

