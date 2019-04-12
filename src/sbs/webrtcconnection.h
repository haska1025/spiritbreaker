#ifndef _WEBRTCCONNECTION_H_
#define _WEBRTCCONNECTION_H_

#include "sbs_decl.h"

#include <pc/session_description.h>
#include <pc/jsep_transport_controller.h>
#include <api/jsep.h>

#include <memory>

SBS_NAMESPACE_DECL_BEGIN

class WebRtcConnection
{
public:
    WebRtcConnection();
    ~WebRtcConnection();

    int Initialize();

    int SetRemoteSdp(const std::string &sdp);
    int CreateAnswer();
    std::string GetLocalSdp(){return local_sdp_;}
    std::string GetRemoteSdp(){return remote_sdp_;}

private:
    std::string session_id_;
    uint64_t session_version_{2};

    std::unique_ptr<webrtc::JsepTransportController> transport_controller_{nullptr};
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

