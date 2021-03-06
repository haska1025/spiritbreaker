#include "webrtcconnection.h"
#include "webrtc_connection_notify.h"
#include "room_mgr.h"
#include "sbs_error.h"

#include <fstream>
#include <iostream>

#include <pc/webrtc_sdp.h>
#include <api/jsep_session_description.h>
#include <rtc_base/strings/json.h>
#include <rtc_base/rtc_certificate.h>

const char kCandidateSdpMidName[] = "sdpMid";
const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
const char kCandidateSdpName[] = "candidate";

WebRtcConnection::DummyCreateSessionDescriptionObserver* 
WebRtcConnection::DummyCreateSessionDescriptionObserver::Create(rtc::scoped_refptr<WebRtcConnection> conn)
{
    return new rtc::RefCountedObject<WebRtcConnection::DummyCreateSessionDescriptionObserver>(conn);
}
void WebRtcConnection::DummyCreateSessionDescriptionObserver::OnSuccess(webrtc::SessionDescriptionInterface* desc) 
{
    RTC_LOG(LS_INFO) << "Success. The type=" << desc->type();

    std::string sdp;
    desc->ToString(&sdp);
    conn_->SetLocalSdp(sdp);

    // Just has answer
    conn_->peer_conn()->SetLocalDescription(WebRtcConnection::DummySetSessionDescriptionObserver::Create(conn_), desc);

}
void WebRtcConnection::DummyCreateSessionDescriptionObserver::OnFailure(webrtc::RTCError error)
{
    RTC_LOG(LS_INFO) << "Failed";
}

WebRtcConnection::DummySetSessionDescriptionObserver*
WebRtcConnection::DummySetSessionDescriptionObserver::Create(rtc::scoped_refptr<WebRtcConnection> conn)
{
    return new rtc::RefCountedObject<WebRtcConnection::DummySetSessionDescriptionObserver>(conn);
}
void WebRtcConnection::DummySetSessionDescriptionObserver::OnSuccess()
{
    RTC_LOG(LS_INFO) << "Success";

    std::string sdp = conn_->GetLocalSdp();
    if (!sdp.empty()){
        // Call other functions
        conn_->notify_->OnLocalSDP();

    }else{
        // The callback signaled by the SetRemoteDescription
        conn_->peer_conn()->CreateAnswer(WebRtcConnection::DummyCreateSessionDescriptionObserver::Create(conn_),
                webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
    }

}
void WebRtcConnection::DummySetSessionDescriptionObserver::OnFailure(webrtc::RTCError error)
{
    RTC_LOG(LS_INFO) << "Failed";
}



WebRtcConnection::WebRtcConnection(WebRtcConnectionNotify *notify):notify_(notify)
{
}
WebRtcConnection::~WebRtcConnection()
{
}

void WebRtcConnection::getKeyAndCert(const char *file, std::string &key)
{
    std::ifstream ifs;

    ifs.open(file, std::ifstream::in);
    if (ifs){
        std::ostringstream os;
        os << ifs.rdbuf();
        key = os.str();
        ifs.close();
    }
}

int WebRtcConnection::Initialize()
{
    webrtc::PeerConnectionInterface::RTCConfiguration config;
    config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
    config.enable_dtls_srtp = true;
    webrtc::PeerConnectionInterface::IceServer server;
//    server.uri = "stun:192.168.29.91:3478"; 
    config.servers.push_back(server);
    
    std::string strkey;
    std::string strcert;

    getKeyAndCert("/home/haska/spiritbreaker/test/server.key", strkey);
    getKeyAndCert("/home/haska/spiritbreaker/test/server.cert", strcert);

    std::cout << " key-----------------" << strkey << std::endl;

    //rtc::scoped_refptr<rtc::RTCCertificate> cert = rtc::RTCCertificate::FromPEM(rtc::RTCCertificatePEM(strkey, strcert));
    //config.certificates.push_back(cert);

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

    peer_connection_->SetRemoteDescription(
            WebRtcConnection::DummySetSessionDescriptionObserver::Create(this),
            desc.release());

    return SBS_SUCCESS;
}

int WebRtcConnection::SetLocalSdp(const std::string &sdp)
{
    RTC_LOG(LS_INFO) << "The local sdp=" << sdp;
    local_sdp_ = sdp;
    return 0;
}

int WebRtcConnection::Close()
{
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
    Json::StyledWriter writer;
    Json::Value jmessage;

    jmessage[kCandidateSdpMidName] = candidate->sdp_mid();
    jmessage[kCandidateSdpMlineIndexName] = candidate->sdp_mline_index();
    std::string sdp;
    if (!candidate->ToString(&sdp)) {
        RTC_LOG(LS_ERROR) << "Failed to serialize candidate";
        return;
    }
    jmessage[kCandidateSdpName] = sdp;


    notify_->OnCandidate(rtc::JsonValueToString(jmessage));
}
void WebRtcConnection::OnIceConnectionReceivingChange(bool receiving)
{
    RTC_LOG(LS_INFO) << "OnIceConnectionReceivingChange";
}

