#ifndef CFILELIST_H_
#define CFILELIST_H_

#include <vector>
#include <string>
using namespace std;

class cFileList {
public:
	cFileList();
	virtual ~cFileList();
	
	vector<string> filenames;
	string current_directory;
};

#endif /*CFILELIST_H_*/
