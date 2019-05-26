#ifndef _WEBRTC_CONNECTION_NOTIFY_H_
#define _WEBRTC_CONNECTION_NOTIFY_H_

#include "sbs_decl.h"
#include <string>

SBS_NAMESPACE_DECL_BEGIN

class WebRtcConnectionNotify
{
public:
    virtual void OnLocalSDP() = 0;
    virtual void OnCandidate(const std::string &candidate) = 0;
};

SBS_NAMESPACE_DECL_END

#endif//_WEBRTC_CONNECTION_NOTIFY_H_

