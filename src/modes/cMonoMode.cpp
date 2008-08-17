#include "cMonoMode.h"

cMonoMode::cMonoMode(const char* name) : cPlayMode(name), mPlayAudioFile("data/onemoretime.mp3") {
}

cMonoMode::~cMonoMode() {
}

int cMonoMode::init () {
	
	cPlayMode::init();
	
	speed *= 2;
	
	start_music();
	
	return 0;
}

void cMonoMode::drawship() {
	glTranslatef( (-0.5+ship.xpos)*MONO_TRACK_WIDTH, 0, 0);
	ship.draw();
}

void cMonoMode::drawtrack() {
	glPushMatrix();
		glColor3fv(colours[WHITE][1]);
		
		// give the illusion of a continually-moving track
		trackdisp += 0.06*dtime*speed;
		if (trackdisp > 18) trackdisp -= 18;
		glTranslatef(0, 0, 20+trackdisp);
		for (int i=0; i<100; i++) {
		  	track->Draw();
		  	glTranslatef(0, 0, -18);
		}
	glPopMatrix();
}

void cMonoMode::CreatePickups() {
	Uint32 newobjecttimer = SDL_GetTicks();
	int pickupcount = 0;
	while (1) {
		if (quit_createpickup_thread) break;
		
		if (SDL_GetTicks() - newobjecttimer >= 250/speed) {
			if (pickupcount == 0) {
				if (random_int(1, 3) == 1) { 
					currentposition = random_int(0,2);
					currentcolour = ( (random_int(1,2) == 1) ? MONO : WHITE );
					pickupcount = 1;
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

string cMonoMode::update() {

	cPlayMode::update();

	// change mono colour
	
	static long monofactor = 400000;

	for (int i=0; i<3; i++) {
		colours[MONO][0][i] = 0.5+0.5*sin(  (float)(SDL_GetTicks() + i*monofactor)  *  bpm  /
		                                 /*------------------------------------------------*/
										  (float)(3*monofactor)                             );
		colours[MONO][1][i] = colours[MONO][0][i]*0.8;
		colours[MONO][2][i] = colours[MONO][1][i]*0.8;
	}
	
	// check for end condition	
	if (is_music_finished()) return "main_menu";
	
	return "continue";
	
}

void cMonoMode::exit() {
	
	if(!is_music_finished()) stop_music();
	
}
