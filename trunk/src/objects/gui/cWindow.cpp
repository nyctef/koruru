#include "cWindow.h"

cWindow::cWindow(string nname, int x_pos, int y_pos, int width, int height) 
	: name(nname), x_position(x_pos), y_position(y_pos), width(width), height(height), buffer(new cFrameBuffer(width, height)),
	dirty(false), highlighted(false), focus(false) {
	
}

cWindow::~cWindow() {
	
	delete buffer;
	
}

void cWindow::draw(cFrameBuffer* parent_buffer) {

	for (unsigned i=0; i<sub_windows.size(); i++) {
		
		if (sub_windows[i]->dirty) {
			
			dirty = true;
			sub_windows[i]->draw(buffer);
			
		}
	} 
	
	if (dirty) {
		
		parent_buffer->copy_from_buffer(buffer, x_position, y_position);	
		
	}
	
	dirty = false;
	
}

string cWindow::handle_event(cEvent event) {

	SDL_Event& sdl_event = event.event;

	switch (sdl_event.type) {
		
		case SDL_KEYDOWN: 
		case SDL_KEYUP:
			// send the event to the window that has focus
			for (unsigned i=0; i<sub_windows.size(); i++) {
				if (sub_windows[i]->focus) {
					sub_windows[i]->handle_event(event);
					break;
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			// adjust the x and y positions to local coordinates
			sdl_event.button.x -= x_position;
			sdl_event.button.y -= y_position;
			
			// send the event to any windows that fall under the mouse click.
			for (unsigned i=0; i<sub_windows.size(); i++) {
				if ((sub_windows[i]->x_position <= sdl_event.button.x) &&
					(sub_windows[i]->x_position + width >= sdl_event.button.x) &&
					(sub_windows[i]->y_position <= sdl_event.button.y) &&
					(sub_windows[i]->y_position + height >= sdl_event.button.y)) {
					
					sub_windows[i]->handle_event(event);
				}
			}
			break; 
		case SDL_MOUSEMOTION:
			
			// adjust the x and y positions to local coordinates
			sdl_event.motion.x -= x_position;
			sdl_event.motion.y -= y_position;
			
			// set the highlight to the control underneath the mouse 
			for (unsigned i=0; i<sub_windows.size(); i++) {
				if ((sub_windows[i]->x_position <= sdl_event.motion.x) &&
					(sub_windows[i]->x_position + width >= sdl_event.motion.x) &&
					(sub_windows[i]->y_position <= sdl_event.motion.y) &&
					(sub_windows[i]->y_position + height >= sdl_event.motion.y)) {
			
					sub_windows[i]->highlighted = true;
				}
			}
		default:
			break;
		
		
	}
	return "continue";
}

void cWindow::draw() {

	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	
	glColor3f(1,1,1);

	glMatrixMode(GL_PROJECTION);
	 glPushMatrix();
	 glLoadIdentity();
	 glOrtho(0,SCREEN_WIDTH,0,SCREEN_HEIGHT,-1, 1);
	glMatrixMode(GL_MODELVIEW);
	 glPushMatrix();
	 glLoadIdentity();
	
	 glTranslatef(x_position, y_position, 0);
	 
	 glPushMatrix();
	    buffer->bind_texture();
		glBegin(GL_QUADS);
	    	glTexCoord2f(0, 0);
	    	 glVertex2f (0, 0);
	    	 
	    	glTexCoord2f(1, 0);
	    	 glVertex2f (width, 0);
	    	 
	    	glTexCoord2f(1, 1);
	    	 glVertex2f (width, height);
	    	 
	    	glTexCoord2f(0, 1);
	    	 glVertex2f (0, height);
	  	glEnd();
  	glPopMatrix();
	  
	glMatrixMode(GL_PROJECTION);
	 glPopMatrix();   
	glMatrixMode(GL_MODELVIEW);
	 glPopMatrix();
	
}
