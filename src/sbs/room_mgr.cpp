#include "room_mgr.h"
#include "room.h"

RoomMgr::RoomMgr()
{
}
RoomMgr RoomMgr::instance_;

RoomMgr *RoomMgr::Instance()
{
    return &instance_;
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


