#include "cPacketQueue.h"

cPacketQueue::cPacketQueue() {
	
	first_pkt = last_pkt = NULL;
	nb_packets = size = 0;
    //memset(this, 0, sizeof(cPacketQueue));
    mutex = SDL_CreateMutex();
    cond = SDL_CreateCond();
}


int cPacketQueue::put(AVPacket* pkt) {
	
    AVPacketList* pkt1;
    if (av_dup_packet(pkt) < 0) {
        return -1;
    }
    pkt1 = new AVPacketList;
    if (!pkt1) return -1;
    
    pkt1->pkt = *pkt;
    pkt1->next = NULL;

    SDL_LockMutex(mutex);

    if (!last_pkt) {
    	first_pkt = pkt1;
    } else {
    	last_pkt->next = pkt1;
    }
    last_pkt = pkt1;
    nb_packets++;
    size += pkt1->pkt.size;
    SDL_CondSignal(cond);

    SDL_UnlockMutex(mutex);
    return 0;
}

int cPacketQueue::get(AVPacket *pkt, int block) {
	
    AVPacketList *pkt1;
    int ret;

    SDL_LockMutex(mutex);

    while (1) {

        pkt1 = first_pkt;
        if (pkt1) {
            first_pkt = pkt1->next;
            if (!first_pkt) last_pkt = NULL;
            nb_packets--;
            size -= pkt1->pkt.size;
            *pkt = pkt1->pkt;
            delete pkt1;
            ret = 1;
            break;
        } else if (!block) {
            ret = -1;
            break;
        } else {
            SDL_CondWait(cond, mutex);
        }
    }
    SDL_UnlockMutex(mutex);
    return ret;
}

void cPacketQueue::clear() {

	if (first_pkt == NULL) return;
	AVPacketList* this_packet = first_pkt;
	AVPacketList* next_packet = this_packet->next;
	while (this_packet != last_pkt) {
	
		av_free_packet(&this_packet->pkt);
		this_packet = next_packet;	
		next_packet = this_packet->next;
	}
	
}

