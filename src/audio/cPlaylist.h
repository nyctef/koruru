#ifndef PLAYLIST_H_
#define PLAYLIST_H_

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib> // for atoi()
#include <algorithm> // for sort()
using namespace std;

#include "cPlaylistItem.h"

class cPlaylist {
public:
	cPlaylist(char* file);
	cPlaylist(string file);
	cPlaylist();
	
	virtual ~cPlaylist();
	
	void from_file(char* file);
	void add_file(char* file);
	
	vector<cPlaylistItem*> items; 

//private:	
	/** The parse_* functions should only be called on an empty playlist: 
	 * they discard any previous data (memory leaks galore here, folks) */

	/** \brief Parse a playlist that is simply a newline-delimited
	 * list of files
	 * 
	 * Note: playlists with .m3u and .pls extensions can have this format.
	 *
	 * No error checking is done in this function.
	 */
	void parse_plain(char* file);
	/** \brief Parse a PLSv2 playlist */ 
	void parse_pls(char* file);
	/** \brief Parse an extended m3u playlist */
	void parse_m3u(char* file);
	
	void writeto_plain(char* file);
	void writeto_pls(char* file);
	void writeto_m3u(char* file);
};

#endif /*PLAYLIST_H_*/
