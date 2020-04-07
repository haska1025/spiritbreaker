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
#include "api/video_codecs/video_decoder.h"

class DummyVideoDecoder : public webrtc::VideoDecoder
{
public:
     virtual int32_t InitDecode(const webrtc::VideoCodec* codec_settings,
                                          int32_t number_of_cores)
     {
         return 0;
     }

     virtual int32_t Decode(const webrtc::EncodedImage& input_image,
             bool missing_frames,
             const webrtc::CodecSpecificInfo* codec_specific_info,
             int64_t render_time_ms)
     {

         RTC_LOG(LS_ERROR) << "Decoder recv buffer=" << input_image.data() << " size=" << input_image.size() << "timestamp=" << input_image.Timestamp();
         return 0;
     }

     virtual int32_t RegisterDecodeCompleteCallback( webrtc::DecodedImageCallback* callback)
     {
         return 0;
     }

     virtual int32_t Release()
     {
         return 0;
     }
};

class DummyVideoDecoderFactory : public webrtc::VideoDecoderFactory
{
public:
    virtual std::vector<webrtc::SdpVideoFormat> GetSupportedFormats() const
    {
        return std::vector<webrtc::SdpVideoFormat>();
    }

    std::unique_ptr<webrtc::VideoDecoder> CreateVideoDecoder( const webrtc::SdpVideoFormat& format)
    {
        return std::unique_ptr<webrtc::VideoDecoder>(new DummyVideoDecoder());
    }
};

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

    std::unique_ptr<webrtc::VideoDecoderFactory> video_decoder_factory(new DummyVideoDecoderFactory());
    peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(
            nullptr /* network_thread */,
            nullptr /* worker_thread */,
            nullptr /* signaling_thread */, 
            nullptr /* default_adm */,
            webrtc::CreateBuiltinAudioEncoderFactory(),
            webrtc::CreateBuiltinAudioDecoderFactory(),
            webrtc::CreateBuiltinVideoEncoderFactory(),
            //webrtc::CreateBuiltinVideoDecoderFactory(),
            std::move(video_decoder_factory),
            nullptr /* audio_mixer */,
            nullptr /* audio_processing */);

    if (!peer_connection_factory_) {
        RTC_LOG(LS_ERROR) << "Create peer connection factory failed!";
        return SBS_GENERAL_ERROR;
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

    Json::Value data = request.data_value();
    uint32_t pubid = 0;
    if (data["pubid"].isNull() || !rtc::GetUIntFromJson(data["pubid"], &pubid)){
        RTC_LOG(LS_ERROR) << "Peer publish invalid pubid";
        return SBS_ERROR_INVALID_PARAM;
    }

    auto publisher = peer->GetPublisher(pubid); 
    if (publisher){
        RTC_LOG(LS_ERROR) << "Add publisher. the publisher already exist. roomid=" << request.peer_id() << " pubid=" << pubid;
        return HC_OK;
    }

    publisher = std::make_shared<Publisher>(pubid, peer);
    if (!publisher){
        RTC_LOG(LS_ERROR) << "Add publisher to create publisher failed. roomid=" << request.peer_id() << " pubid=" << pubid;
        return SBS_GENERAL_ERROR;
    }

    peer->AddPublisher(publisher);

    return HC_OK;
}
int RoomMgr::ReomvePublisher(const Message &request, Message &response)
{
    return SBS_SUCCESS;
}


int RoomMgr::SetRemoteSdp(const Message &request, Message &response)
{
    auto room = GetRoom(request.room_id());
    if (!room){
        RTC_LOG(LS_ERROR) << "Set remote sdp. the room isn't exist. roomid=" << request.room_id();
        return SBS_ERROR_ROOM_NOT_EXIST;
    }
    auto peer = room->GetPeer(request.peer_id());
    if (!peer){
        RTC_LOG(LS_ERROR) << "Set remote sdp. the peer isn't exist. roomid=" << request.peer_id();
        return SBS_ERROR_PEER_NOT_EXIST;
    }

    Json::Value data = request.data_value();
    uint32_t pubid = 0;
    if (data["pubid"].isNull() || !rtc::GetUIntFromJson(data["pubid"], &pubid)){
        RTC_LOG(LS_ERROR) << "Set remote sdp. invalid pubid";
        return SBS_ERROR_INVALID_PARAM;
    }

    auto publisher = peer->GetPublisher(pubid); 
    if (!publisher){
        RTC_LOG(LS_ERROR) << "Set remote sdp. the publisher isn't exist. roomid=" << request.peer_id() << " pubid=" << pubid;
        return SBS_ERROR_INVALID_PARAM;
    }

    std::string sdp;
    if (data["sdp"].isNull() || !rtc::GetStringFromJson(data["sdp"], &sdp)){
        RTC_LOG(LS_ERROR) << "Peer join room invalid cmd";
        return SBS_ERROR_INVALID_PARAM;
    }
 
    std::string localsdp;

    publisher->SetRemoteSdp(sdp, localsdp);

    Json::Value lsdp;
    lsdp["sdp"] = localsdp;
    
    response.data_value(lsdp);

    return HC_OK;
}

int RoomMgr::GetCandidate(const Message &request, Message &response)
{
    auto room = GetRoom(request.room_id());
    if (!room){
        RTC_LOG(LS_ERROR) << "Get candidate. the room isn't exist. roomid=" << request.room_id();
        return SBS_ERROR_ROOM_NOT_EXIST;
    }
    auto peer = room->GetPeer(request.peer_id());
    if (!peer){
        RTC_LOG(LS_ERROR) << "Get candidate. the peer isn't exist. roomid=" << request.peer_id();
        return SBS_ERROR_PEER_NOT_EXIST;
    }

    Json::Value data = request.data_value();
    uint32_t pubid = 0;
    if (data["pubid"].isNull() || !rtc::GetUIntFromJson(data["pubid"], &pubid)){
        RTC_LOG(LS_ERROR) << "Get candidate. invalid pubid";
        return SBS_ERROR_INVALID_PARAM;
    }

    auto publisher = peer->GetPublisher(pubid); 
    if (!publisher){
        RTC_LOG(LS_ERROR) << "Get candidate. the publisher isn't exist. roomid=" << request.peer_id() << " pubid=" << pubid;
        return SBS_ERROR_INVALID_PARAM;
    }

    Json::Value can = publisher->GetCandidate();
    if (!can["candidate"].isNull()){
        response.data_value(can);
    }

    return HC_OK;
}

int RoomMgr::Subscribe(const Message &request, Message &response)
{
    auto room = GetRoom(request.room_id());
    if (!room){
        RTC_LOG(LS_ERROR) << "Subscribe. the room isn't exist. roomid=" << request.room_id();
        return SBS_ERROR_ROOM_NOT_EXIST;
    }
    auto peer = room->GetPeer(request.peer_id());
    if (!peer){
        RTC_LOG(LS_ERROR) << "Subscribe. the peer isn't exist. roomid=" << request.peer_id();
        return SBS_ERROR_PEER_NOT_EXIST;
    }

    Json::Value data = request.data_value();

    uint32_t share_peerid = 0;
    if (data["shared_peerid"].isNull() || !rtc::GetUIntFromJson(data["shared_peerid"], &share_peerid)){
        RTC_LOG(LS_ERROR) << "Subscribe. invalid share peerid";
        return SBS_ERROR_INVALID_PARAM;
    }

    auto share_peer = room->GetPeer(share_peerid);
    if (!share_peer){
        RTC_LOG(LS_ERROR) << "Subscribe. the share peer isn't exist. roomid=" << request.peer_id() << " share peerid=" << share_peerid;
        return SBS_ERROR_PEER_NOT_EXIST;
    }

    uint32_t pubid = 0;
    if (data["pubid"].isNull() || !rtc::GetUIntFromJson(data["pubid"], &pubid)){
        RTC_LOG(LS_ERROR) << "Subscribe. invalid pubid";
        return SBS_ERROR_INVALID_PARAM;
    }

    auto publisher = share_peer->GetPublisher(pubid); 
    if (!publisher){
        RTC_LOG(LS_ERROR) << "Subscribe. the publisher isn't exist. roomid=" << request.peer_id() << " pubid=" << pubid;
        return SBS_ERROR_INVALID_PARAM;
    }

    auto sub = peer->GetSubscriber(pubid);
    if (sub){
        RTC_LOG(LS_ERROR) << "Subscribe. the subscriber is exist. roomid=" << request.peer_id() << " pubid=" << pubid;
        return HC_OK;
    }

    sub = std::make_shared<Subscriber>(peer, publisher);
    peer->AddSubscriber(sub);
    publisher->AddSubscriber(sub);


    return HC_OK;
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



