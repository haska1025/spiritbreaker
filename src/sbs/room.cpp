#include "room.h"
#include "peer.h"
#include "room_mgr.h"

Room::Room(uint32_t id):id_(id)
{
    RoomMgr::Instance()->AddRoom(std::shared_ptr<Room>(this));
}
Room::~Room()
{
}

bool Room::AddPeer(std::shared_ptr<Peer> peer)
{
    std::lock_guard<std::mutex> lg(peers_mutex_);

    if (peers_.find(peer->id()) == peers_.end()){
        peers_[peer->id()] = peer;
        return true;
    }
    return false;
}
std::shared_ptr<Peer> Room::GetPeer(uint32_t id)
{
    std::lock_guard<std::mutex> lg(peers_mutex_);

    auto it = peers_.find(id);
    if (it != peers_.end()){
        return it->second;
    }
    return std::shared_ptr<Peer>(nullptr);
}
bool Room::RemovePeer(uint32_t id)
{
    std::lock_guard<std::mutex> lg(peers_mutex_);

    auto it = peers_.find(id);
    if (it != peers_.end()){
        peers_.erase(it);
        return true;
    }
    return false;
}


