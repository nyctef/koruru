#include "PlaylistItem.h"

PlaylistItem::PlaylistItem() {
}

PlaylistItem::~PlaylistItem() {
}

void PlaylistItem::fill_from_file() {

}

void PlaylistItem::fill_from_id3() {

    ID3_Tag* tag = new ID3_Tag(path.c_str());

    if (tag != NULL) {
        ID3_Frame *frame;

        if ((frame = tag->Find(ID3FID_TITLE)) != NULL) {
            ID3_Field *field;
            if ((field = &frame->Field(ID3FN_TEXT)) != NULL && field->GetRawText() != NULL) {
                song = string(field->GetRawText());
                //cout << "Title: " << field->GetRawText() << endl;
            }
        }

        if ((frame = tag->Find(ID3FID_LEADARTIST)) != NULL) {
            ID3_Field *field;
            if ((field = &frame->Field(ID3FN_TEXT)) != NULL && field->GetRawText() != NULL) {
                artist = string(field->GetRawText());
                //cout << "Artist: " << field->GetRawText() << endl;
            }
        }

        if ((frame = tag->Find(ID3FID_ALBUM)) != NULL) {
            ID3_Field *field;
            if ((field = &frame->Field(ID3FN_TEXT)) != NULL && field->GetRawText() != NULL) {
                album = field->GetRawText();
                //cout << "Album: " << field->GetRawText() << endl;
            }
        }
    }
}

string PlaylistItem::get_title() {

    if (song.compare("") == 0) {
        if (full_title.compare("") == 0) {
            string result("Unknown");
            return result;
        } else {
            return full_title;
        }
    } else {
        string result(song);
        result += " - " + artist + " - " + album;
        return result;
    }
}

void PlaylistItem::show_known_info() {
   ID3_FrameInfo myFrameInfo;
   for (int cur = ID3FID_NOFRAME+1; cur < myFrameInfo.MaxFrameID(); cur ++) { 
     cout << "Short ID: " << 	 myFrameInfo.ShortName(ID3_FrameID(cur)) <<\
        	 " Long ID: " << 	 myFrameInfo.LongName(ID3_FrameID(cur)) <<\
       		 " Description: " << myFrameInfo.Description(ID3_FrameID(cur)) << endl;
     cout << "ping" << endl;
   }
} 

