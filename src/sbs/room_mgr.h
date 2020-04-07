#ifndef _ROOM_MGR_H_
#define _ROOM_MGR_H_

#include "sbs_decl.h"
#include "sbs_message.h"
#include <mutex>
#include <memory>
#include <unordered_map>

#include <api/peer_connection_interface.h>

SBS_NAMESPACE_DECL_BEGIN
class Room;
class RoomMgr
{
public:
    static RoomMgr *Instance();
    
    int Initialize();

    bool AddRoom(std::shared_ptr<Room> r);
    bool RemoveRoom(uint32_t id);
    std::shared_ptr<Room> GetRoom(uint32_t id);

    int PeerJoinRoom(const Message &request, Message &response);
    int PeerLeaveRoom(const Message &request, Message &response);

    int AddPublisher(const Message &request, Message &response);
    int ReomvePublisher(const Message &request, Message &response);

    int SetRemoteSdp(const Message &request, Message &response);
    int GetCandidate(const Message &request, Message &response);
    int Subscribe(const Message &request, Message &response);

    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> pc_factory(){return peer_connection_factory_;}
private:
    static RoomMgr instance_;
    std::unordered_map<uint32_t, std::shared_ptr<Room>> rooms_;
    std::mutex rooms_mutex_;

    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory_;

    SBS_DISALLOW_CONSTRUCT(RoomMgr);
    SBS_DISALLOW_COPY_CONSTRUCT(RoomMgr);
    SBS_DISALLOW_ASSIGNMENT(RoomMgr);
};
SBS_NAMESPACE_DECL_END
#endif//_ROOM_MGR_H_

