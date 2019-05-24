%module sbs 

%{
#include "../../src/sbs/sbs_decl.h"
#include "../../src/sbs/sbs_error.h"
#include "../../src/sbs/sbs_mgr.h"
#include "../../src/sbs/webrtc_connection_interface.h"
#include "../../src/sbs/webrtc_connection_notify.h"
%}

%include "std_string.i"

%include "../../src/sbs/sbs_decl.h"
%include "../../src/sbs/sbs_mgr.h"

class WebRtcFacade : public WebRtcConnectionNotify
{
public:
    WebRtcFacade(v8::Handle<v8:Object> object):persistent{object}
    {
        webrtc_ =
    }

    int Initialize()
    {
    }
    int Close();

    int SetRemoteSdp(const std::string &sdp);
    int SetLocalSdp(const std::string &sdp);

    std::string GetLocalSdp() override{return local_sdp_;}
    std::string GetRemoteSdp()override {return remote_sdp_;}


private:
    Nan::Persistent<v8::Object> persistent;
    WebRtcConnectionInterface *webrtc_{nullptr};
};
