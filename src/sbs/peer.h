#ifndef _PEER_H_
#define _PEER_H_

#include "sbs_decl.h"
#include <memory>

SBS_NAMESPACE_DECL_BEGIN
class Room;
class Peer : public std::enable_shared_from_this<Peer>
{
public:
    Peer(uint32_t id, std::shared_ptr<Room> r);
    ~Peer();

    uint32_t id(){return id_;}

    std::shared_ptr<Room> room(){return room_;}

private:
    uint32_t id_;
    std::shared_ptr<Room> room_;
};
SBS_NAMESPACE_DECL_END
#endif//_PEER_H_

