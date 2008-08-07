#include "cMonoMode.h"

cMonoMode::cMonoMode(const char* name) : cPlayMode(name) {
}

cMonoMode::~cMonoMode() {
}

int cMonoMode::init () {
	InitMode(RESET_MODE | GAME_MODE);
	score = 0;
	speed = 2*bpm/60;
	if (createpickup_thread == NULL) createpickup_thread = SDL_CreateThread(generate_pickups_thread, (void*)this);
	return 0;
}

void cMonoMode::drawship() {
	glTranslatef( (-0.5+ship.xpos)*MONO_TRACK_WIDTH, 0, 0);
	ship.draw();
}

void cMonoMode::drawtrack() {
	glPushMatrix();
		//glColor3fv(color[BLUE][0]);
		
		// give the illusion of a continually-moving track
		trackdisp += 0.06*dtime*speed;
		if (trackdisp > 18) trackdisp -= 18;
		glTranslatef(0, 0, 20+trackdisp);
		for (int i=0; i<100; i++) {
		  	track->Draw();
		  	glTranslatef(0, 0, -18);
		}
	glPopMatrix();
	
	changemonocolour();
}

void cMonoMode::CreatePickups() {
	Uint32 newobjecttimer = SDL_GetTicks();
	int pickupcount = 0;
	while (1) {
		if (quit_createpickup_thread) break;
		
		if (SDL_GetTicks() - newobjecttimer >= 250/speed) {
			if (pickupcount == 0) {
				if (GenerateRandomNumber(1, 3) == 1) { 
					currentposition = GenerateRandomNumber(0,2);
					currentcolour = GenerateRandomNumber(1,2);
						currentcolour = (currentcolour == 1) ? MONO : WHITE;
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

void cMonoMode::changemonocolour() {

	static long monofactor = 400000;

	for (int i=0; i<3; i++) {
		color[MONO][0][i] = 0.5+0.5*sin(  (float)(SDL_GetTicks() + i*monofactor)  *  bpm  /
		                                 /*------------------------------------------------*/
										  (float)(3*monofactor)                             );
		color[MONO][1][i] = color[MONO][0][i]*0.8;
		color[MONO][2][i] = color[MONO][1][i]*0.8;
	}
}
