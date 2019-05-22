#ifndef _WEBRTCCONNECTION_H_
#define _WEBRTCCONNECTION_H_

#include <api/jsep.h>
#include <api/peer_connection_interface.h>

#include <memory>
#include <string>
#include <future>

#include "sbs_decl.h"
#include "webrtc_connection_interface.h"

SBS_NAMESPACE_DECL_BEGIN

class WebRtcConnection: public webrtc::PeerConnectionObserver,
    public rtc::RefCountInterface,
    public WebRtcConnectionInterface 
{
public:
    WebRtcConnection();
    ~WebRtcConnection();

    //
    // WebRtcConnectionInterface implementation.
    //

    int Initialize() override;
    int Close() override;

    int SetRemoteSdp(const std::string &sdp) override;
    int SetLocalSdp(const std::string &sdp) override;

    std::string GetLocalSdp() override{return local_sdp_;}
    std::string GetRemoteSdp()override {return remote_sdp_;}

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
        static DummyCreateSessionDescriptionObserver* Create(rtc::scoped_refptr<WebRtcConnection> conn,
                std::shared_ptr<std::promise<std::string>> p);

        DummyCreateSessionDescriptionObserver(rtc::scoped_refptr<WebRtcConnection> conn,
                std::shared_ptr<std::promise<std::string>> p):conn_(conn),result_promise_{p}{}

        void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;
        void OnFailure(webrtc::RTCError error) override;
    private:
        rtc::scoped_refptr<WebRtcConnection> conn_;
        std::shared_ptr<std::promise<std::string>> result_promise_;
    };

    class DummySetSessionDescriptionObserver
        : public webrtc::SetSessionDescriptionObserver 
    {
    public:
        static DummySetSessionDescriptionObserver* Create(rtc::scoped_refptr<WebRtcConnection> conn,
                std::shared_ptr<std::promise<std::string>> p); 

        DummySetSessionDescriptionObserver(rtc::scoped_refptr<WebRtcConnection> conn, 
                std::shared_ptr<std::promise<std::string>> p):conn_(conn),result_promise_{p}{}

        virtual void OnSuccess() override;
        virtual void OnFailure(webrtc::RTCError error) override; 
    private:
        rtc::scoped_refptr<WebRtcConnection> conn_;
        std::shared_ptr<std::promise<std::string>> result_promise_;
    };


private:
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_{nullptr};

    // The answer sdp
    std::string local_sdp_;
    // The remote sdp
    std::string remote_sdp_;
};

SBS_NAMESPACE_DECL_END
#endif//_WEBRTCCONNECTION_H_

