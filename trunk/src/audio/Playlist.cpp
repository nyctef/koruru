#include "Playlist.h"

Playlist::Playlist()
{
}

Playlist::~Playlist()
{
}

unsigned find_value(string input, const char* tag) {
	if (input.compare(0, strlen(tag), tag) == 0) {
		// seperate out the number after 'tag' then convert it to an integer.
		return atoi(input.substr(strlen(tag)).c_str());
	} else {
		return 0;
	}
		
}

string remove_first(string input, const char* token) {
	size_t pos = input.find(token, 0);
	if (pos == string::npos) return input;
	
	string result = input.substr(0, pos) + input.substr(pos+strlen(token));
	return result;
}

void Playlist::parse_plain(char* file) {
	
	cout << "parsing plain file " << file << endl;
	
	string current_line;
	
	ifstream file_stream(file);
	if (!file_stream) throw "Error opening file";
	
	items.clear();
	
	PlaylistItem* tmpitem; 
	while (getline(file_stream, current_line)) {
		tmpitem = new PlaylistItem;
		tmpitem->path = current_line;
		items.push_back(tmpitem);
	}
	
}

void Playlist::parse_pls(char* file) {
	/** 
	 * Plan:
	 * -----
	 * 
	 * load file into vector of strings, separated (but not including) newlines
	 * 
	 * loop through lines and erase comments, find the [Playlist]/[playlist] 
	 * line and erase it, find the NumberOfEntries= line and store the value. 
	 * Find the version= line and remove it (maybe check for =2). Remove blank lines. 
	 * 
	 * sort lines (so that song entries are now grouped). Note that the numbers will be 
	 * in alphabetical, not numerical order.
	 * 
	 * create a vector of playlistitems equal to the size of the playlist. Initialise them.
	 * 
	 * loop through lines. foreach line:
	 *   check if we've moved on an entry (the number will change)
	 *   edit the playlistitem for that number with the filename, length or title respectively.
	 */
	
	cout << "parsing .pls file " << file << endl;
	
	vector<string> filev;
	string current_line;
	
	ifstream file_stream(file);
	if (!file_stream) throw "Error opening file";
	
	while (getline(file_stream, current_line)) {
		filev.push_back(current_line);
	}
	
	// first loop
	
	unsigned num_entries = 0;
	int version = 0;
	bool playlist_found = false;
	for (unsigned i=0; i<filev.size(); i++) {
		// strip all the comments out of the file (from ; symbol to end of line)
		if (filev[i].find_first_of(';') != string::npos) {
			filev[i] = filev[i].substr(0, filev[i].find_first_of(';'));
		}
		
		// TODO: make the following tests case-independant
		
		// remove the [playlist] line - ignores whitespace by only comparing the first n characters,
		// where n = strlen("[playlist]")
		if ((filev[i].compare(0, strlen("[playlist]"), "[playlist]") == 0) 
		 || (filev[i].compare(0, strlen("[Playlist]"), "[Playlist]") == 0)) {
		 	filev.erase(filev.begin() + i);
			playlist_found = true;
			i--; // vector has now shrunk
			continue;
		}
		
		// find the number of entries
		if (find_value(filev[i], "NumberOfEntries=") != 0) num_entries = find_value(filev[i], "NumberOfEntries=");
		
		// find the version tag
		if (find_value(filev[i], "version=") != 0) version = find_value(filev[i], "version=");
		if (find_value(filev[i], "Version=") != 0) version = find_value(filev[i], "Version=");
		
		// clear empty lines
		if(filev[i].compare("") == 0) {
			filev.erase(filev.begin() + i);
			i--;
			continue;
		}
	}
	
	if (!playlist_found) throw "probable malformed .pls file : [playlist] or [Playlist] not found";
	if (num_entries == 0) throw "probable malformed .pls file : NumberOfEntries is 0 or nonexistant";
	
	items.resize(num_entries, NULL);
	
	for (unsigned i=0; i<items.size(); i++) {
		items[i] = new PlaylistItem;
	}
	
	int current_item = 0;
	size_t equals_pos; // position of the equals sign in the line.
	
	// actually parse the file now. All that should be left are Length#, File# and Title# fields.
	
	string tmpstr;
	int mode;
	for (unsigned i=0; i<filev.size(); i++) {
		tmpstr = filev[i];
		
		//isolate the current item (ie # from File#=blah)
		if (tmpstr.compare(0, strlen("File"), "File") == 0) {
			tmpstr = remove_first(tmpstr, "File");
			mode = 1;
		} else if (tmpstr.compare(0, strlen("Title"), "Title") == 0) {
			tmpstr = remove_first(tmpstr, "Title");
			mode = 2;
		} else if (tmpstr.compare(0, strlen("Length"), "Length") == 0) {
			tmpstr = remove_first(tmpstr, "Length");
			mode = 3;
		} else {
			throw "Probable malformed pls file : key not one of Length#, File# or Title#";
		}
		
		equals_pos = tmpstr.find_first_of('=');
		if (equals_pos == string::npos) throw "Probable malformed pls file : can't find equals sign in key-value pair";
		
		current_item = atoi(tmpstr.substr(0, equals_pos+1).c_str());
		tmpstr = tmpstr.substr(equals_pos+1);
		
		// set the appropriate variable
		
		switch (mode) { // current_item-1 to convert to array index (the playlist starts counting from 1)
			case 1:
				items[current_item-1]->path = tmpstr.c_str(); 
				break;
			case 2:
				items[current_item-1]->full_title = tmpstr.c_str(); 
				break;
			case 3:
				items[current_item-1]->length = atoi(tmpstr.c_str()); 
				break;
		}
	}
}

void Playlist::parse_m3u(char* file) {

	/** 
	 * Plan:
	 * -----
	 * 
	 * load file into vector of strings, separated by (but not including) newlines
	 * 
	 * check for #EXTM3U line
	 * 
	 * for each line:
	 *  line begins with #EXTINF?
	 *   yes - parse extinf, then parse next line for filename
	 *   no - parse line for filename
	 *  create playlistitem for line 
	 */
	
	cout << "parsing .m3u file " << file << endl;
	
	items.clear();
	
	vector<string> filev;
	string current_line, tmpstr;
	
	ifstream file_stream(file);
	if (!file_stream) throw "Error opening file";
	
	// load file
	
	while (getline(file_stream, current_line)) {
		filev.push_back(current_line);
	}
	
	// check for extm3u
	
	if ((filev[0].compare("#EXTM3U") != 0)) throw "possible malformed .m3u file: #EXTM3U not found";
	filev.erase(filev.begin());
	
	unsigned comma_pos;
	PlaylistItem* item = NULL;
	for (unsigned i=0; i<filev.size(); i++) {
		
		item = new PlaylistItem;
		
		if ((filev[i].compare(0, strlen("#EXTINF:"), "#EXTINF:")) == 0) {
			tmpstr = remove_first(filev[i], "#EXTINF:");
			
			//isolate the length
			comma_pos = tmpstr.find_first_of(',');
			if (comma_pos == string::npos) throw "possible malformed .m3u file: comma not found in #EXTINF line";
			
			item->length = atoi(tmpstr.substr(0, comma_pos+1).c_str());
			
			// get the title
			item->full_title = tmpstr.substr(comma_pos+1);
			
			// get the filename from the next line
			i++;
			item->path = filev[i]; 
		} else {
			item->path = filev[i];
		}
		
		items.push_back(item);
	}
}

void Playlist::writeto_plain(char* file) {

	ofstream out;
	out.open(file);
	
	for (unsigned i=0; i<items.size(); i++) {
		out << items[i]->path << endl;
	}
	
	out.close();
	
}

void Playlist::writeto_pls(char* file) {

	ofstream out;
	out.open(file);
	
	out << "[playlist]" << endl;
	out << "NumberOfItems=" << items.size() << endl;
	
	for (unsigned i=0; i<items.size(); i++) {
		out << "Title" << i << "=" << items[i]->path << endl;
		out << "File" << i << "=" << items[i]->path << endl;
		out << "Length" << i << "=" << items[i]->path << endl;
	}
	out << "Version=2" << endl;
	
	out.close();
}

void Playlist::writeto_m3u(char* file) {

	ofstream out;
	out.open(file);
	
	out << "#EXTM2U" << endl;
	
	for (unsigned i=0; i<items.size(); i++) {
		out << "#EXTINF:" << items[i]->length << "," << items[i]->full_title << endl;
		out << items[i]->path << endl;
	}
	
	out.close();
}
