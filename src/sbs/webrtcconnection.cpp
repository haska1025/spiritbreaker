#include "webrtcconnection.h"
#include "room_mgr.h"
#include "sbs_error.h"
#include "fake_video_track_source.h"

#include <pc/webrtc_sdp.h>
#include <api/jsep_session_description.h>

WebRtcConnection::DummyCreateSessionDescriptionObserver* 
WebRtcConnection::DummyCreateSessionDescriptionObserver::Create(rtc::scoped_refptr<WebRtcConnection> conn, 
        std::shared_ptr<std::promise<std::string>> p)
{
    return new rtc::RefCountedObject<WebRtcConnection::DummyCreateSessionDescriptionObserver>(conn, p);
}
void WebRtcConnection::DummyCreateSessionDescriptionObserver::OnSuccess(webrtc::SessionDescriptionInterface* desc) 
{
    RTC_LOG(LS_INFO) << "Success. The type=" << desc->type();

    std::string sdp;
    desc->ToString(&sdp);
    conn_->SetLocalSdp(sdp);
    conn_->local_sdp_type(desc->GetType());

    // Just has answer
    conn_->peer_conn()->SetLocalDescription(WebRtcConnection::DummySetSessionDescriptionObserver::Create(conn_, result_promise_),
            desc);

}
void WebRtcConnection::DummyCreateSessionDescriptionObserver::OnFailure(webrtc::RTCError error)
{
    RTC_LOG(LS_INFO) << "Failed";
}

WebRtcConnection::DummySetSessionDescriptionObserver*
WebRtcConnection::DummySetSessionDescriptionObserver::Create(rtc::scoped_refptr<WebRtcConnection> conn, 
        std::shared_ptr<std::promise<std::string>> p)
{
    return new rtc::RefCountedObject<WebRtcConnection::DummySetSessionDescriptionObserver>(conn, p);
}
void WebRtcConnection::DummySetSessionDescriptionObserver::OnSuccess()
{
    RTC_LOG(LS_INFO) << "Success";

    std::string sdp = conn_->GetLocalSdp();
    if (!sdp.empty()){
        result_promise_->set_value(sdp);
    }else{
        if (conn_->remote_sdp_type() == webrtc::SdpType::kOffer){
            // The callback signaled by the SetRemoteDescription
            conn_->peer_conn()->CreateAnswer(WebRtcConnection::DummyCreateSessionDescriptionObserver::Create(conn_, result_promise_),
                    webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
        }
    }

}
void WebRtcConnection::DummySetSessionDescriptionObserver::OnFailure(webrtc::RTCError error)
{
    RTC_LOG(LS_INFO) << "Failed";
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
    server.uri = "stun:192.168.29.91:3478"; 
    config.servers.push_back(server);

    peer_connection_ = RoomMgr::Instance()->pc_factory()->CreatePeerConnection(
            config, nullptr, nullptr, this);
    if ( peer_connection_ == nullptr){
        RTC_LOG(LS_ERROR) << "Create peer connection failed.";
        return -1;
    }

    rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
            RoomMgr::Instance()->pc_factory()->CreateAudioTrack(
                "audio_label", RoomMgr::Instance()->pc_factory()->CreateAudioSource(
                    cricket::AudioOptions())));
    auto result_or_error = peer_connection_->AddTrack(audio_track, {"stream_id"});
    if (!result_or_error.ok()) {
        RTC_LOG(LS_ERROR) << "Failed to add audio track to PeerConnection: "
            << result_or_error.error().message();
    }

    rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track_(
            RoomMgr::Instance()->pc_factory()->CreateVideoTrack("video_label", webrtc::FakeVideoTrackSource::Create()));

    result_or_error = peer_connection_->AddTrack(video_track_, {"stream_id"});
    if (!result_or_error.ok()) {
        RTC_LOG(LS_ERROR) << "Failed to add video track to PeerConnection: "
            << result_or_error.error().message();
    }   

    return 0;
}

int WebRtcConnection::CreateLocalSdp()
{
    webrtc::PeerConnectionInterface::RTCOfferAnswerOptions option;
    option.offer_to_receive_audio = 0;
    option.offer_to_receive_video = 0;

    std::shared_ptr<std::promise<std::string>> answer_promise = std::make_shared<std::promise<std::string>>();
    std::future<std::string> answer_future = answer_promise->get_future();

    peer_connection_->CreateOffer(WebRtcConnection::DummyCreateSessionDescriptionObserver::Create(this, answer_promise), option);

    answer_future.get();

    return SBS_SUCCESS;
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

    remote_sdp_type_ = desc->GetType();
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

void WebRtcConnection::OnSignalingChange( webrtc::PeerConnectionInterface::SignalingState new_state)
{
    RTC_LOG(LS_INFO) << "OnSignalingChange new_state=" << new_state;
}
void WebRtcConnection::OnAddTrack( rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
        const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams)
{
    RTC_LOG(LS_INFO) << "OnAddTrack";
}
void WebRtcConnection::OnRemoveTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver)
{
    RTC_LOG(LS_INFO) << "OnRemoveTrack";
}
void WebRtcConnection::OnDataChannel( rtc::scoped_refptr<webrtc::DataChannelInterface> channel)
{
    RTC_LOG(LS_INFO) << "OnDataChannel";
}
void WebRtcConnection::OnRenegotiationNeeded()
{
    RTC_LOG(LS_INFO) << "OnRenegotiationNeeded";
}
void WebRtcConnection::OnIceConnectionChange( webrtc::PeerConnectionInterface::IceConnectionState new_state)
{
    RTC_LOG(LS_INFO) << "OnIceConnectionChange new_state=" << new_state;
}
void WebRtcConnection::OnIceGatheringChange( webrtc::PeerConnectionInterface::IceGatheringState new_state)
{
    RTC_LOG(LS_INFO) << "OnIceGatheringChange new_state=" << new_state;
}
void WebRtcConnection::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
{
    std::string sdp_candidate;

    if (candidate->candidate().protocol().compare("tcp") ==0)
        return;

    candidate->ToString(&sdp_candidate);

    RTC_LOG(LS_INFO) << "OnIceCandidate sdpMid=" << candidate->sdp_mid() 
        << " sdp_mindex=" << candidate->sdp_mline_index()
        << " server_url=" << candidate->server_url()
        << " candidate_sdp=" << sdp_candidate; 

    candidate_["candidate"] = sdp_candidate;
    candidate_["sdpMid"] = candidate->sdp_mid();
    candidate_["sdpMLineIndex"] = candidate->sdp_mline_index();
}
void WebRtcConnection::OnIceConnectionReceivingChange(bool receiving)
{
    RTC_LOG(LS_INFO) << "OnIceConnectionReceivingChange";
}

