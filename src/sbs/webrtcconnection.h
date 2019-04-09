#ifndef _WEBRTCCONNECTION_H_
#define _WEBRTCCONNECTION_H_

#include "sbs_decl.h"
#include <pc/session_description.h>
//#include <api/peer_connection_interface.h>
#include <pc/jsep_transport_controller.h>

#include <memory>

SBS_NAMESPACE_DECL_BEGIN

class WebRtcConnection
{
public:
    WebRtcConnection();
    ~WebRtcConnection();

    int Initialize();

    int CreateOffer();
    int CreateAnswer();
    int SetRemoteSdp(cricket::SessionDescription *offer);
    std::string GetLocalSdp();

private:
//    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
 //   rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory_;
    std::unique_ptr<webrtc::JsepTransportController> transport_controller_;
    std::unique_ptr<cricket::SessionDescription> local_desc_;
    std::unique_ptr<cricket::SessionDescription> remote_desc_;
    std::string local_sdp_;
    std::string remote_sdp_;
};

SBS_NAMESPACE_DECL_END
#endif//_WEBRTCCONNECTION_H_

