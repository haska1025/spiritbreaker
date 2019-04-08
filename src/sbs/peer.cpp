#include "peer.h"
#include "room.h"

Peer::Peer(uint32_t id, std::shared_ptr<Room> r):id_(id), room_(r)
{
}
Peer::~Peer()
{
}

 
