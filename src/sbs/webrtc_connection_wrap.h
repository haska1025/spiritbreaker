#ifndef _WEBRTC_CONNECTION_WRAP_H_
#define _WEBRTC_CONNECTION_WRAP_H_

#include <nan.h>
#include <string>

#include "sbs_decl.h"
#include "webrtcconnection.h"
#include "webrtc_connection_notify.h"

SBS_NAMESPACE_DECL_BEGIN

class SBS_DLL_DECL WebRtcConnectionWrap: public Nan::ObjectWrap,
    public WebRtcConnectionNotify
{
public:
    static NAN_MODULE_INIT(Init);

    void OnLocalSDP() override;
    void OnCandidate() override;

private:
    WebRtcConnectionWrap(v8::Handle<v8::Object> object);
    ~WebRtcConnectionWrap();

    static NAN_METHOD(New);
    static NAN_METHOD(Initialize);
    static NAN_METHOD(Close);

    static NAN_METHOD(SetRemoteSdp);
    static NAN_METHOD(SetLocalSdp);

    static NAN_METHOD(GetLocalSdp);
    static NAN_METHOD(GetRemoteSdp);

    static Nan::Persistent<v8::Function> constructor_;

    Nan::Persistent<v8::Object> persistent_;

    rtc::scoped_refptr<WebRtcConnection> webrtc_;
    rtc::Thread *sbs_thr_{nullptr};
};
SBS_NAMESPACE_DECL_END
#endif//_WEBRTC_CONNECTION_WRAP_H_

