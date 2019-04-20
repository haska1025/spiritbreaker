#include "room_mgr.h"
#include "room.h"
#include "sbs_error.h"
#include "httpcommon.h"
#include "peer.h"
#include "publisher.h"
#include "subscriber.h"
#include "configuration.h"

#include <rtc_base/logging.h>
#include <api/audio_codecs/audio_decoder_factory.h>
#include <api/audio_codecs/audio_encoder_factory.h>
#include <api/audio_codecs/builtin_audio_decoder_factory.h>
#include <api/audio_codecs/builtin_audio_encoder_factory.h>
#include <api/create_peerconnection_factory.h>

#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "api/video_codecs/video_decoder_factory.h"
#include "api/video_codecs/video_encoder_factory.h"


RoomMgr::RoomMgr()
{
}
RoomMgr RoomMgr::instance_;

RoomMgr *RoomMgr::Instance()
{
    return &instance_;
}

int RoomMgr::Initialize()
{
    int rc = SBS_SUCCESS;

    peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(
            nullptr /* network_thread */,
            nullptr /* worker_thread */,
            nullptr /* signaling_thread */, 
            nullptr /* default_adm */,
            webrtc::CreateBuiltinAudioEncoderFactory(),
            webrtc::CreateBuiltinAudioDecoderFactory(),
            webrtc::CreateBuiltinVideoEncoderFactory(),
            webrtc::CreateBuiltinVideoDecoderFactory(),
            nullptr /* audio_mixer */,
            nullptr /* audio_processing */);

    if (!peer_connection_factory_) {
        return false;
    }

    return rc;
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
    uint32_t publisherid = 0;

    auto room = GetRoom(request.room_id());
    if (!room){
        RTC_LOG(LS_ERROR) << "Add publisher. the room isn't exist. roomid=" << request.room_id();
        return SBS_ERROR_ROOM_NOT_EXIST;
    }
    auto peer = room->GetPeer(request.peer_id());
    if (!peer){
        RTC_LOG(LS_ERROR) << "Add publisher. the peer isn't exist. roomid=" << request.peer_id();
        return SBS_ERROR_PEER_NOT_EXIST;
    }

    auto publisher = peer->GetPublisher(publisherid); 
    if (publisher){
        RTC_LOG(LS_ERROR) << "Add publisher. the peer already exist. roomid=" << request.peer_id();
        return SBS_SUCCESS;
    }

    publisher = std::make_shared<Publisher>(publisherid);
    peer->AddPublisher(publisher);

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

