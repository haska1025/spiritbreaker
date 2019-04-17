#include "room_mgr.h"
#include "room.h"
#include "sbs_error.h"
#include "httpcommon.h"
#include "peer.h"
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

    //Create audio codecs
    cricket::AudioCodecs audio_codecs;
    rc = Configuration::GetCodecs("audio", audio_codecs);
    if (rc != SBS_SUCCESS)
        return rc;

    std::for_each(audio_codecs.begin(), audio_codecs.end(), [](const cricket::AudioCodec &codec){
            RTC_LOG(LS_INFO) << "audio===" << codec.ToString();
            });

    //media_session_factory_.set_audio_codecs(audio_codecs, audio_codecs);
    //Create video codecs
    cricket::VideoCodecs video_codecs;
    rc = Configuration::GetCodecs("video", video_codecs);
    if (rc != SBS_SUCCESS)
        return rc;

    std::for_each(video_codecs.begin(), video_codecs.end(), [](const cricket::VideoCodec &codec){
            RTC_LOG(LS_INFO) << "video===" << codec.ToString();
            });

    //media_session_factory_.set_video_codecs(video_codecs);


    //webrtc::PeerConnectionFactoryDependencies dependencies;
    //peer_connection_factory_ = webrtc::CreateModularPeerConnectionFactory(std::move(dependencies));

    peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(
            nullptr /* network_thread */, nullptr /* worker_thread */,
            nullptr /* signaling_thread */, nullptr /* default_adm */,
            webrtc::CreateBuiltinAudioEncoderFactory(),
            webrtc::CreateBuiltinAudioDecoderFactory(),
            webrtc::CreateBuiltinVideoEncoderFactory(),
            webrtc::CreateBuiltinVideoDecoderFactory(), nullptr /* audio_mixer */,
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

