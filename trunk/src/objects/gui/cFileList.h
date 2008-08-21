#ifndef CFILELIST_H_
#define CFILELIST_H_

#include "cWindow.h"

#include <vector>
#include <string>
using namespace std;

class cFileList : public cWindow {
public:
	cFileList();
	virtual ~cFileList();
	
	vector<string> filenames;
	string current_directory;
	
	virtual void draw();
	virtual string handle_event(cEvent event);
};

#endif /*CFILELIST_H_*/
