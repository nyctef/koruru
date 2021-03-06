#ifndef CWINDOW_H_
#define CWINDOW_H_

#include "../cFrameBuffer.h"
#include "cEvent.h"

#include <vector>
using namespace std;

class cWindow {
public:
	cWindow(string name, int x_pos=0, int y_pos=0, int width=0, int height=0);
	virtual ~cWindow();
	
	string name;
	vector<cWindow*> sub_windows;
	int x_position;
	int y_position;
	int width;
	int height;
	
	bool dirty;
	bool focus;
	bool highlighted;
	
	cFrameBuffer* buffer;
	
	virtual string handle_event(cEvent event);
	
	/** \brief Draw the object/window to the specified buffer.
	 * 
	 * At the moment, each window has its own buffer, whether or not it is made up of subwindows.
	 * Each subwindow draws itself to the window, before the window draws itself to a parent window
	 * (or the screen, which is handled outside of this class)
	 * 
	 * TODO: think about only having framebuffers for primitive types, so that the draw function could
	 * pass down the framebuffer reference to the primitive types, so they draw directly onto the 
	 * base window/panel/screen thingie.
	 */
	virtual void draw(cFrameBuffer* buffer);
	
	/** \brief Draw as a normal OpenGL textured quad. Used for the highest-level window to draw to the screen.
	 */
	void draw();
	
};

extern Uint32 SCREEN_WIDTH;
extern Uint32 SCREEN_HEIGHT;

#endif /*CWINDOW_H_*/
