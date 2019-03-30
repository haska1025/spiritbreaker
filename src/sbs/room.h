#ifndef _ROOM_H_
#define _ROOM_H_

#include "sbs_decl.h"
#include <unordered_map>
#include <memory>
#include <mutex>

SBS_NAMESPACE_DECL_BEGIN
class Peer;
class Room
{
public:
    Room();
    ~Room();

    bool AddPeer(std::shared_ptr<Peer> peer);
    std::shared_ptr<Peer> GetPeer(uint32_t id);
    bool RemovePeer(uint32_t id);

    uint32_t id(){return id_;}
private:
    uint32_t id_;

    std::unordered_map<uint32_t, std::shared_ptr<Peer>> peers_;
    std::mutex peers_mutex_;

    SBS_DISALLOW_COPY_CONSTRUCT(Room);
    SBS_DISALLOW_ASSIGNMENT(Room);
};
SBS_NAMESPACE_DECL_END
#endif//_ROOM_H_

