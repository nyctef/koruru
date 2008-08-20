#include "cWindow.h"

cWindow::cWindow(string nname, int x_pos, int y_pos, int width, int height) 
	: name(nname), x_position(x_pos), y_position(y_pos), width(width), height(height), buffer(new cFrameBuffer(width, height)) {
	
}

cWindow::~cWindow() {
	
	delete buffer;
	
}

void cWindow::draw(cFrameBuffer* parent_buffer) {

	is_dirty = false;

	for (unsigned i=0; i<sub_windows.size(); i++) {
		
		if (sub_windows[i]->is_dirty) {
			
			is_dirty = true;
			sub_windows[i]->draw(buffer);
			
		}
	} 
	
	if (is_dirty) {
		
		parent_buffer->copy_from_buffer(buffer, x_position, y_position);	
		
	}
	
}

string cWindow::handle_event(cEvent event) {

	SDL_Event& sdl_event = event.event;

	switch (sdl_event.type) {
		
		case SDL_KEYDOWN: 
		case SDL_KEYUP:
			// send the event to the window that has focus
			for (unsigned i=0; i<sub_windows.size(); i++) {
				if (sub_windows[i]->has_focus) {
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
		
		
	}
	return "continue";
}
