#ifndef PACKETQUEUE_H_
#define PACKETQUEUE_H_

#include "ffmpeg/avformat.h"
#include "SDL/SDL_thread.h"

/** \brief A Queue of Packets
 * 
 * Where we keep packets after loading them from the file and before decoding them.
 */
class cPacketQueue {
public:
	/** \brief Default constructor */
    cPacketQueue();
    
	/** \brief pointer to first packet in queue 
	 * 
	 * AVPacketList is actually a linked-list *node*, hence why we have two of them.
	 */
    AVPacketList* first_pkt;
    /** \brief pointer to last packet in queue */
    AVPacketList* last_pkt;
    /** \brief Number of packets in the list */
    int nb_packets;
    /** \brief byte size of the queue's data
     * 
     * A sum of the values from packet->size 
     */
    int size;
    
    /** \brief A mutex since we're using several threads to access this struct */
    SDL_mutex *mutex;
    SDL_cond *cond;
    
    /** \brief Put a packet into the queue 
     * \param packet the packet to put
     */
    int put(AVPacket* packet);
	/** \brief Get a packet from the queue 
	 * \param packet Where to put the packet we get.
	 * \param block Whether to block or not to wait for a packet if the queue is empty.
	 */
	 int get(AVPacket* packet, int block);
	 
	 void clear();
};

#endif /*PACKETQUEUE_H_*/
