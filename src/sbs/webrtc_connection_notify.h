#ifndef _WEBRTC_CONNECTION_NOTIFY_H_
#define _WEBRTC_CONNECTION_NOTIFY_H_

#include "sbs_decl.h"

SBS_NAMESPACE_DECL_BEGIN

class WebRtcConnectionNotify
{
public:
    virtual void OnLocalSDP() = 0;
    virtual void OnCandidate() = 0;
};

SBS_NAMESPACE_DECL_END

#endif//_WEBRTC_CONNECTION_NOTIFY_H_

