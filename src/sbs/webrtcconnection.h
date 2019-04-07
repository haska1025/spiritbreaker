#ifndef _WEBRTCCONNECTION_H_
#define _WEBRTCCONNECTION_H_

#include "sbs_decl.h"
#include <pc/session_description.h>

#include <memory>

SBS_NAMESPACE_DECL_BEGIN

class WebRtcConnection
{
public:
    WebRtcConnection();
    ~WebRtcConnection();

    int CreateLocalSdp();
    std::string GetLocalSdp();

private:
    std::unique_ptr<cricket::SessionDescription> local_sdp_;
};

SBS_NAMESPACE_DECL_END
#endif//_WEBRTCCONNECTION_H_

