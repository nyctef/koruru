#include "cAnimatedTexture.h"

cAnimatedTexture::cAnimatedTexture() : cTexture() {
	init();
}

cAnimatedTexture::cAnimatedTexture(const char* file) : cTexture(file) {
	init();
}

cAnimatedTexture::cAnimatedTexture(const char* file, int new_num_frames, Uint32 new_frame_time) {
	
	init(); frames.clear();
	
	num_frames = new_num_frames;
	frame_time = new_frame_time;
	
	SDL_Surface* surface = IMG_Load(file);
	if (!surface) throw "loading file failed with IMG_Load()";
	
	unsigned subsurface_width = surface->w / num_frames;
	unsigned subsurface_height = surface->h;
	
	SDL_Surface* subsurface = NULL;
	
	// load up each image into a texture and store the texid.
	for (unsigned i=0; i<num_frames; i++) {
		
		subsurface = SubSurface(surface, i*subsurface_width, 0, subsurface_width, subsurface_height);
		frames.push_back(load_texture_from_surface(subsurface));
		SDL_FreeSurface(subsurface);
	}
	
	frame_start_time = SDL_GetTicks();
	
}

void cAnimatedTexture::init() {

	frame_start_time = SDL_GetTicks();
	num_frames = 1;
	current_frame = 0;
	frames.push_back(texid);
	frame_time = -1; // biggest unsigned value
}

cAnimatedTexture::~cAnimatedTexture() {
}

void cAnimatedTexture::bind() {

	// check if we need to switch to the next frame
	//cout << "tex " << SDL_GetTicks() << " " << frame_start_time << " " << frame_time << " " << bool(SDL_GetTicks() > frame_start_time + frame_time) << endl;
	if (SDL_GetTicks() > frame_start_time + frame_time ) {
		//cout << "ping " << current_frame << " " << frames[current_frame] << " " << (glIsTexture(frames[current_frame])?"true":"false") << endl;
		frame_start_time += frame_time;
		current_frame++;
		if (current_frame == num_frames) current_frame = 0; 	
	} 
	
	glBindTexture(GL_TEXTURE_2D, frames[current_frame]);
}

void cAnimatedTexture::restart() {

	current_frame = 0;	
}
