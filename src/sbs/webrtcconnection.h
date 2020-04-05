#ifndef _WEBRTCCONNECTION_H_
#define _WEBRTCCONNECTION_H_

#include <api/jsep.h>
#include <api/peer_connection_interface.h>

#include <memory>
#include <string>
#include <future>

#include "sbs_decl.h"

SBS_NAMESPACE_DECL_BEGIN

class WebRtcConnectionNotify;

class WebRtcConnection: public webrtc::PeerConnectionObserver,
    public rtc::RefCountInterface
{
public:
    WebRtcConnection(WebRtcConnectionNotify *notify);
    ~WebRtcConnection();

    //
    // WebRtcConnectionInterface implementation.
    //

    int Initialize();
    int Close();

    int SetRemoteSdp(const std::string &sdp);
    int SetLocalSdp(const std::string &sdp);

    std::string GetLocalSdp(){return local_sdp_;}
    std::string GetRemoteSdp(){return remote_sdp_;}

    //
    // PeerConnectionObserver implementation.
    //
    
    void OnSignalingChange( webrtc::PeerConnectionInterface::SignalingState new_state) override;
    void OnAddTrack( rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
            const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams) override;
    void OnRemoveTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) override;
    void OnDataChannel( rtc::scoped_refptr<webrtc::DataChannelInterface> channel) override; 
    void OnRenegotiationNeeded() override;
    void OnIceConnectionChange( webrtc::PeerConnectionInterface::IceConnectionState new_state) override;
    void OnIceGatheringChange( webrtc::PeerConnectionInterface::IceGatheringState new_state) override;
    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override;
    void OnIceConnectionReceivingChange(bool receiving) override; 

    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_conn(){return peer_connection_;}

    // CreateSessionDescriptionObserver implementation.
    class DummyCreateSessionDescriptionObserver
        : public webrtc::CreateSessionDescriptionObserver
    {
    public:
        static DummyCreateSessionDescriptionObserver* Create(rtc::scoped_refptr<WebRtcConnection> conn);

        DummyCreateSessionDescriptionObserver(rtc::scoped_refptr<WebRtcConnection> conn):conn_(conn){}

        void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;
        void OnFailure(webrtc::RTCError error) override;
    private:
        rtc::scoped_refptr<WebRtcConnection> conn_;
    };

    class DummySetSessionDescriptionObserver
        : public webrtc::SetSessionDescriptionObserver 
    {
    public:
        static DummySetSessionDescriptionObserver* Create(rtc::scoped_refptr<WebRtcConnection> conn); 

        DummySetSessionDescriptionObserver(rtc::scoped_refptr<WebRtcConnection> conn):conn_(conn){}

        virtual void OnSuccess() override;
        virtual void OnFailure(webrtc::RTCError error) override; 
    private:
        rtc::scoped_refptr<WebRtcConnection> conn_;
    };

    void getKeyAndCert(const char *file, std::string &key);

private:
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_{nullptr};

    // The answer sdp
    std::string local_sdp_;
    // The remote sdp
    std::string remote_sdp_;

    WebRtcConnectionNotify *notify_;
};

SBS_NAMESPACE_DECL_END
#endif//_WEBRTCCONNECTION_H_

