#ifndef _ROOM_MGR_H_
#define _ROOM_MGR_H_

#include "sbs_decl.h"

#include <api/peer_connection_interface.h>
#include <rtc_base/thread.h>

SBS_NAMESPACE_DECL_BEGIN
class Room;
class RoomMgr
{
public:
    static RoomMgr *Instance();
    
    int Initialize();
    int Close();

    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> pc_factory(){return peer_connection_factory_;}

    rtc::Thread *sbs_thr(){return sbs_thread_;}
private:
    int _Initialize();
    int _Close();

private:
    static RoomMgr instance_;

    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory_;

    rtc::Thread *sbs_thread_{nullptr};

    SBS_DISALLOW_CONSTRUCT(RoomMgr);
    SBS_DISALLOW_COPY_CONSTRUCT(RoomMgr);
    SBS_DISALLOW_ASSIGNMENT(RoomMgr);
};
SBS_NAMESPACE_DECL_END
#endif//_ROOM_MGR_H_

