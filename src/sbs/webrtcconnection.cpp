#include "webrtcconnection.h"
#include "room_mgr.h"
#include "sbs_error.h"

#include <pc/webrtc_sdp.h>
#include <api/jsep_session_description.h>

WebRtcConnection::DummyCreateSessionDescriptionObserver* WebRtcConnection::DummyCreateSessionDescriptionObserver::Create(rtc::scoped_refptr<WebRtcConnection> conn, std::shared_ptr<std::promise<std::string>> p)
{
    return new rtc::RefCountedObject<WebRtcConnection::DummyCreateSessionDescriptionObserver>(conn, p);
}
void WebRtcConnection::DummyCreateSessionDescriptionObserver::OnSuccess(webrtc::SessionDescriptionInterface* desc) 
{
    RTC_LOG(LS_INFO) << __FUNCTION__ << "Success";

    std::string sdp;
    desc->ToString(&sdp);
    conn_->SetLocalSdp(sdp);
    result_promise_->set_value(sdp);
}
void WebRtcConnection::DummyCreateSessionDescriptionObserver::OnFailure(webrtc::RTCError error)
{
    RTC_LOG(LS_INFO) << __FUNCTION__ << "Failed";
}

WebRtcConnection::DummySetSessionDescriptionObserver* WebRtcConnection::DummySetSessionDescriptionObserver::Create(rtc::scoped_refptr<WebRtcConnection> conn, std::shared_ptr<std::promise<std::string>> p)
{
    return new rtc::RefCountedObject<WebRtcConnection::DummySetSessionDescriptionObserver>(conn, p);
}
void WebRtcConnection::DummySetSessionDescriptionObserver::OnSuccess()
{
    RTC_LOG(LS_INFO) << __FUNCTION__ << "Success";

    conn_->peer_conn()->CreateAnswer(WebRtcConnection::DummyCreateSessionDescriptionObserver::Create(conn_, result_promise_), webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
}
void WebRtcConnection::DummySetSessionDescriptionObserver::OnFailure(webrtc::RTCError error)
{
    RTC_LOG(LS_INFO) << __FUNCTION__ << "Failed";
}



WebRtcConnection::WebRtcConnection()
{
}
WebRtcConnection::~WebRtcConnection()
{
}

int WebRtcConnection::Initialize()
{
    webrtc::PeerConnectionInterface::RTCConfiguration config;
    config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
    config.enable_dtls_srtp = true;
    webrtc::PeerConnectionInterface::IceServer server;
    server.uri = "stun:192.168.32.86:3478"; 
    config.servers.push_back(server);

    peer_connection_ = RoomMgr::Instance()->pc_factory()->CreatePeerConnection(
            config, nullptr, nullptr, this);
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

    std::string tmpsdp;
    desc->ToString(&tmpsdp);

    RTC_LOG(LS_INFO) << "After deserialize===" << tmpsdp;

    std::shared_ptr<std::promise<std::string>> answer_promise = std::make_shared<std::promise<std::string>>();

    std::future<std::string> answer_future = answer_promise->get_future();

    peer_connection_->SetRemoteDescription(
            WebRtcConnection::DummySetSessionDescriptionObserver::Create(this, answer_promise),
            desc.release());

    answer_future.get();
    return SBS_SUCCESS;
}

int WebRtcConnection::SetLocalSdp(const std::string &sdp)
{
    RTC_LOG(LS_INFO) << "The local sdp=" << sdp;
    local_sdp_ = sdp;
    return 0;
}


