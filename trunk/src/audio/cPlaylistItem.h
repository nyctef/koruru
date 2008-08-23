#ifndef PLAYLISTITEM_H_
#define PLAYLISTITEM_H_

#include <string>
using namespace std;

#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem; 

#include "id3/tag.h"
#include "id3/id3lib_frame.h"
#include "id3/field.h"

#include "genres.h"

class cPlaylistItem { 
public:
	cPlaylistItem();
	virtual ~cPlaylistItem();
	
	string full_title;
	
	string song;
	string artist;
	string album;
	unsigned length;
	string path;
	int year;
	int genre;
	
	string get_title();
	
	/** \brief check that the file pointed to by path exists */
	bool check_exists(){return bfs::exists(path);}
	
	/** \brief fill empty metadata values from any id3 tags, etc, in the file pointed to by path */
	void fill_from_file();
	
	void fill_from_id3();
	void fill_from_vorbis();
	
	void show_known_info(); // util func
};

#endif /*PLAYLISTITEM_H_*/
