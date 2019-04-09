#include "room_mgr.h"
#include "room.h"
#include "sbs_error.h"
#include "httpcommon.h"
#include "peer.h"

#include <rtc_base/logging.h>

RoomMgr::RoomMgr()
{
}
RoomMgr RoomMgr::instance_;

RoomMgr *RoomMgr::Instance()
{
    return &instance_;
}

int RoomMgr::PeerJoinRoom(const Message &request, Message &response)
{
    RTC_LOG(LS_INFO) << "Peer join the room";

    auto room = GetRoom(request.room_id());
    if (!room){
        room = std::make_shared<Room>(request.room_id());
        RoomMgr::Instance()->AddRoom(room);
    }
    auto peer = room->GetPeer(request.peer_id());
    if (!peer){
        peer = std::make_shared<Peer>(request.peer_id(), room);
        room->AddPeer(peer);
    }

    return HC_OK;
}
int RoomMgr::PeerLeaveRoom(const Message &request, Message &response)
{
    return SBS_SUCCESS;
}

int RoomMgr::AddPublisher(const Message &request, Message &response)
{
    return SBS_SUCCESS;
}
int RoomMgr::ReomvePublisher(const Message &request, Message &response)
{
    return SBS_SUCCESS;
}


bool RoomMgr::AddRoom(std::shared_ptr<Room> r)
{
    std::lock_guard<std::mutex> lg(rooms_mutex_);
    if (rooms_.find(r->id()) == rooms_.end()){
        rooms_.insert(std::pair<uint32_t, std::shared_ptr<Room>>(r->id(), r));
        return true; 
    }
    return false;
}
bool RoomMgr::RemoveRoom(uint32_t id)
{
    std::lock_guard<std::mutex> lg(rooms_mutex_);
    auto it = rooms_.find(id);
    if ( it != rooms_.end()){
        rooms_.erase(it);
        return true; 
    }
    return false;
}
std::shared_ptr<Room> RoomMgr::GetRoom(uint32_t id)
{
    std::lock_guard<std::mutex> lg(rooms_mutex_);
    auto it = rooms_.find(id);
    if ( it != rooms_.end()){
        return it->second; 
    }
    return std::shared_ptr<Room>(nullptr);
}


