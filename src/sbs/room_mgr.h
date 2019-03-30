#ifndef _ROOM_MGR_H_
#define _ROOM_MGR_H_

#include "sbs_decl.h"
#include <mutex>
#include <memory>
#include <unordered_map>

SBS_NAMESPACE_DECL_BEGIN
class Room;
class RoomMgr
{
public:
    static RoomMgr *Instance();

    bool AddRoom(std::shared_ptr<Room> r);
    bool RemoveRoom(uint32_t id);
    std::shared_ptr<Room> GetRoom(uint32_t id);

private:
    static RoomMgr instance_;
    std::unordered_map<uint32_t, std::shared_ptr<Room>> rooms_;
    std::mutex rooms_mutex_;

    SBS_DISALLOW_CONSTRUCT(RoomMgr);
    SBS_DISALLOW_COPY_CONSTRUCT(RoomMgr);
    SBS_DISALLOW_ASSIGNMENT(RoomMgr);
};
SBS_NAMESPACE_DECL_END
#endif//_ROOM_MGR_H_

