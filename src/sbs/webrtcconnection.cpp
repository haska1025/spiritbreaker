#include "webrtcconnection.h"
#include "room_mgr.h"
#include "sbs_error.h"

#include <pc/webrtc_sdp.h>
#include <api/jsep_session_description.h>


class DummySetSessionDescriptionObserver : public webrtc::SetSessionDescriptionObserver {
public:
    static DummySetSessionDescriptionObserver* Create() {
        return new rtc::RefCountedObject<DummySetSessionDescriptionObserver>();
    }
    virtual void OnSuccess() { RTC_LOG(INFO) << __FUNCTION__; }
    virtual void OnFailure(webrtc::RTCError error) {
        RTC_LOG(INFO) << __FUNCTION__ << " " << ToString(error.type()) << ": "
            << error.message();
    }
};

WebRtcConnection::WebRtcConnection()
{
}
WebRtcConnection::~WebRtcConnection()
{
}

int WebRtcConnection::Initialize()
{

    // Create the jsep transport controller
    //    webrtc::JsepTransportController::Config config;
    //   transport_controller_.reset(new webrtc::JsepTransportController(
    //              NULL,NULL, NULL, NULL, config));

    webrtc::PeerConnectionInterface::RTCConfiguration config;
    config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
    config.enable_dtls_srtp = true;
    webrtc::PeerConnectionInterface::IceServer server;
    //server.uri = GetPeerConnectionString();
    config.servers.push_back(server);

    peer_connection_ = RoomMgr::Instance()->pc_factory()->CreatePeerConnection(
            config, nullptr, nullptr, nullptr);
    if ( peer_connection_ == nullptr){
        RTC_LOG(LS_ERROR) << "Create peer connection failed.";
        return -1;
    }

    return 0;
}

int WebRtcConnection::SetRemoteSdp(const std::string &sdp)
{
    // Save the sdp
    RTC_LOG(LS_INFO) << "The remote sdp=" <<sdp;
    remote_sdp_ = sdp;

    // Deserialize the sdp to SessionDescription
    webrtc::SdpParseError error;
    auto desc = webrtc::CreateSessionDescription(webrtc::SdpType::kOffer, sdp, &error); 
    if (!desc){
        RTC_LOG(LS_ERROR) << "Set remote sdp create session desc by sdp failed. " << error.description;
        return SBS_GENERAL_ERROR;
    }

    remote_desc_ = std::move(desc);

    peer_connection_->SetRemoteDescription(
            DummySetSessionDescriptionObserver::Create(),
            remote_desc_.release());

    std::string tmpsdp;
    remote_desc_->ToString(&tmpsdp);

    RTC_LOG(LS_INFO) << "After deserialize===" << tmpsdp;
    return SBS_SUCCESS;
}

int WebRtcConnection::CreateAnswer()
{
 //   cricket::MediaSessionOptions options;
    if (!remote_desc_){
        RTC_LOG(LS_ERROR) << "Create answer failed. You must call SetRemoteSdp firstly";
        return SBS_GENERAL_ERROR;
    }

    peer_connection_->CreateAnswer(nullptr, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
/*
    auto desc = RoomMgr::Instance()->media_session_desc_factory()->CreateAnswer(
            remote_desc_->description(),
            options,
            local_desc_?local_desc_->description():nullptr);
    if (!desc){
        RTC_LOG(LS_ERROR) << "Create answer failed";
        return SBS_GENERAL_ERROR;
    }

    local_desc_ = std::move(answer);

    local_desc_->ToString(&local_sdp_);

    RTC_LOG(LS_INFO) << "The local sdp :" << local_sdp_;
*/
    return SBS_SUCCESS;
}

