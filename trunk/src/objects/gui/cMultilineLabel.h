#ifndef CMULTILINELABEL_H_
#define CMULTILINELABEL_H_

#include "cWindow.h"
#include "cFont.h"

#include <vector>
#include <string>
#include <sstream> 
#include <iostream>

using namespace std;


/** \brief A label that spans multiple lines. Used for debug output at the moment.
 * 
 * FIXME: this class could bake the font rendering to the buffer; would probably
 * speed it up a bit and use it as intended at least.
 */
class cMultilineLabel : public cWindow/*, public mDrawText*/ {
public:
	/** \brief Empty constructor */
	cMultilineLabel();
	cMultilineLabel(int x_pos, int y_pos, int width, int num_lines, cFont* font );
	virtual ~cMultilineLabel();
	
	 template <class T>
	void set(T t);
	 template <class T>
	void add(T t);
	void clear();
	
	void draw();
	
	vector<string> lines;
	cFont* font;
	unsigned num_lines;
	
protected:
	void trim_lines();
	
};

// these functions added here because templates need them here

 template <class T>
cMultilineLabel& operator<<(cMultilineLabel& lhs, T rhs) {lhs.add(rhs); return lhs;}

/* * * * * * * * * * end interface * * * * * * * * * * * * */

 template <class T>
void cMultilineLabel::set(T t) {
	clear();
	add(t);
}

 template <class T>
void cMultilineLabel::add(T t) {

	// convert whatever it is to a string
	stringstream ss;
	ss << t; 

	// getline will ignore a newline at the end of a string, so if there is one, we need to deal with it at the end.
	bool add_newline_at_end = false;
	string::iterator it(--ss.str().end());
	if (*it == '\n') add_newline_at_end = true;

	// here, we add the lines to our data. If there is just one line with no newlines being added, then that just gets
	// appended to the end of the current line and skips the while loop, since the stringstream is now empty. If, however,
	// the string contains several lines, then new lines are added to the label in the while loop.
	string token;
	getline(ss, token, '\n');
	if (lines.empty()) { // since if the vector is empty, lines.size()-1 == -1, so segfaults all round. 
		lines.push_back(token);
	} else {
		lines[lines.size()-1] += token; 
	}
	
	while( getline(ss, token, '\n')) {
		lines.push_back(token);
	}
	
	if (add_newline_at_end) {
		lines.push_back("");
	}
	
	trim_lines(); 
	
	// now render
	//draw_text(text, font);
	dirty = true;
	
}

extern cMultilineLabel debug_pane;

#endif /*CMULTILINELABEL_H_*/
