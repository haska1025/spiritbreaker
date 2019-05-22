#ifndef _WEBRTC_CONNECTION_INTERFACE_H_
#define _WEBRTC_CONNECTION_INTERFACE_H_

#include "sbs_decl.h"
#include <string>

SBS_NAMESPACE_DECL_BEGIN

class SBS_DLL_DECL WebRtcConnectionInterface
{
public:
    static WebRtcConnectionInterface * Create();
    static void Delete(WebRtcConnectionInterface * conn);

    virtual ~WebRtcConnectionInterface();

    virtual int Initialize() = 0;
    virtual int Close() = 0;

    virtual int SetRemoteSdp(const std::string &sdp) = 0;
    virtual int SetLocalSdp(const std::string &sdp) = 0;

    virtual std::string GetLocalSdp() = 0;
    virtual std::string GetRemoteSdp() = 0;
};
SBS_NAMESPACE_DECL_END
#endif//_WEBRTC_CONNECTION_INTERFACE_H_

