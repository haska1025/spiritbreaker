#ifndef _WEBRTCCONNECTION_H_
#define _WEBRTCCONNECTION_H_

#include "sbs_decl.h"

#include <api/jsep.h>
#include <api/peer_connection_interface.h>

#include <memory>

SBS_NAMESPACE_DECL_BEGIN

class WebRtcConnection: public webrtc::PeerConnectionObserver,
    public webrtc::CreateSessionDescriptionObserver
{
public:
    WebRtcConnection();
    ~WebRtcConnection();

    int Initialize();

    int SetRemoteSdp(const std::string &sdp);
    int CreateAnswer();
    std::string GetLocalSdp(){return local_sdp_;}
    std::string GetRemoteSdp(){return remote_sdp_;}

    //
    // PeerConnectionObserver implementation.
    //
    
    void OnSignalingChange( webrtc::PeerConnectionInterface::SignalingState new_state) override{};
    void OnAddTrack( rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver, const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams) override{};
    void OnRemoveTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) override{};
    void OnDataChannel( rtc::scoped_refptr<webrtc::DataChannelInterface> channel) override {}
    void OnRenegotiationNeeded() override {}
    void OnIceConnectionChange( webrtc::PeerConnectionInterface::IceConnectionState new_state) override{};
    void OnIceGatheringChange( webrtc::PeerConnectionInterface::IceGatheringState new_state) override{};
    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override{};
    void OnIceConnectionReceivingChange(bool receiving) override {}

    // CreateSessionDescriptionObserver implementation.
    void OnSuccess(webrtc::SessionDescriptionInterface* desc) override{};
    void OnFailure(webrtc::RTCError error) override{};
    
private:
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_{nullptr};

    // Create by create answer
    std::unique_ptr<webrtc::SessionDescriptionInterface> local_desc_{nullptr};
    // Come from remote endpoint
    std::unique_ptr<webrtc::SessionDescriptionInterface> remote_desc_{nullptr};
    // The answer sdp
    std::string local_sdp_;
    // The remote sdp
    std::string remote_sdp_;
};

SBS_NAMESPACE_DECL_END
#endif//_WEBRTCCONNECTION_H_

