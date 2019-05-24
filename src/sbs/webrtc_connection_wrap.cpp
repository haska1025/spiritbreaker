#include "webrtc_connection_wrap.h"
#include "webrtcconnection.h"
#include "room_mgr.h"
#include "sbs_error.h"

#include <api/proxy.h>
#include <rtc_base/bind.h>

Nan::Persistent<v8::Function> WebRtcConnectionWrap::constructor_;
WebRtcConnectionWrap::WebRtcConnectionWrap()
{
    sbs_thr_ = RoomMgr::Instance()->sbs_thr();
}

WebRtcConnectionWrap::~WebRtcConnectionWrap()
{
}

NAN_MODULE_INIT(WebRtcConnectionWrap::Init)
{
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("WebRtcConnectionWrap").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "initialize", Initialize);
    Nan::SetPrototypeMethod(tpl, "close", Close);
    Nan::SetPrototypeMethod(tpl, "setRemoteSdp", SetRemoteSdp);
    Nan::SetPrototypeMethod(tpl, "setLocalSdp", SetLocalSdp);

    Nan::SetPrototypeMethod(tpl, "getRemoteSdp", GetRemoteSdp);
    Nan::SetPrototypeMethod(tpl, "getLocalSdp", GetLocalSdp);

    constructor_.Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("WebRtcConnectionWrap").ToLocalChecked(),
            Nan::GetFunction(tpl).ToLocalChecked());
}
NAN_METHOD(WebRtcConnectionWrap::New)
{
    if (!info.IsConstructCall()) {
        return Nan::ThrowError("`new` required");
    }

    WebRtcConnectionWrap *obj = new WebRtcConnectionWrap();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(WebRtcConnectionWrap::Initialize)
{
    int rc = SBS_SUCCESS;

    WebRtcConnectionWrap *obj = Nan::ObjectWrap::Unwrap<WebRtcConnectionWrap>(info.Holder());
    if (!obj->webrtc_){
        rtc::scoped_refptr<WebRtcConnection> conn(new rtc::RefCountedObject<WebRtcConnection>());
        if (!conn){
            return Nan::ThrowError("Create webrt connection obj failed.");
        }
        obj->webrtc_ = conn; 
        rc = obj->sbs_thr_->Invoke<int>(RTC_FROM_HERE, rtc::Bind(&WebRtcConnection::Initialize, obj->webrtc_.get())); 
    }else{
        rc = SBS_GENERAL_ERROR;
    }

    info.GetReturnValue().Set(rc);
}
NAN_METHOD(WebRtcConnectionWrap::Close)
{
    int rc = SBS_SUCCESS;

    WebRtcConnectionWrap *obj = Nan::ObjectWrap::Unwrap<WebRtcConnectionWrap>(info.Holder());
    if (obj->webrtc_){
        rc = obj->sbs_thr_->Invoke<int>(RTC_FROM_HERE, rtc::Bind(&WebRtcConnection::Close, obj->webrtc_.get())); 
    }

    info.GetReturnValue().Set(rc);
}

NAN_METHOD(WebRtcConnectionWrap::SetRemoteSdp)
{
    int rc = SBS_SUCCESS;

    Nan::Utf8String sdp(info[0]);
    if (!*sdp){
        return Nan::ThrowError("Set remote sdp invalid param");
    }

    WebRtcConnectionWrap *obj = Nan::ObjectWrap::Unwrap<WebRtcConnectionWrap>(info.Holder());
    if (!obj->webrtc_){
        return Nan::ThrowError("The webrtc is null. `Invoke Initialize firstly` required");
    }

    rc = obj->sbs_thr_->Invoke<int>(RTC_FROM_HERE, rtc::Bind(&WebRtcConnection::SetRemoteSdp, obj->webrtc_.get(), std::string(*sdp, sdp.length())));

    info.GetReturnValue().Set(rc);

}
NAN_METHOD(WebRtcConnectionWrap::SetLocalSdp)
{
    int rc = SBS_SUCCESS;

    Nan::Utf8String sdp(info[0]);
    if (!*sdp){
        return Nan::ThrowError("Set local sdp invalid param");
    }

    WebRtcConnectionWrap *obj = Nan::ObjectWrap::Unwrap<WebRtcConnectionWrap>(info.Holder());
    if (!obj->webrtc_){
        return Nan::ThrowError("The webrtc is null. `Invoke Initialize firstly` required");
    }

    rc = obj->sbs_thr_->Invoke<int>(RTC_FROM_HERE, rtc::Bind(&WebRtcConnection::SetLocalSdp, obj->webrtc_.get(), std::string(*sdp, sdp.length()))); 

    info.GetReturnValue().Set(rc);

}

NAN_METHOD(WebRtcConnectionWrap::GetLocalSdp)
{
    WebRtcConnectionWrap *obj = Nan::ObjectWrap::Unwrap<WebRtcConnectionWrap>(info.Holder());
    if (!obj->webrtc_){
        return Nan::ThrowError("The webrtc is null. `Invoke Initialize firstly` required");
    }

    std::string sdp = obj->sbs_thr_->Invoke<std::string>(RTC_FROM_HERE, rtc::Bind(&WebRtcConnection::GetLocalSdp, obj->webrtc_.get())); 

    info.GetReturnValue().Set(Nan::Encode(sdp.c_str(), sdp.length(), Nan::Encoding::UTF8));
}
NAN_METHOD(WebRtcConnectionWrap::GetRemoteSdp)
{
    WebRtcConnectionWrap *obj = Nan::ObjectWrap::Unwrap<WebRtcConnectionWrap>(info.Holder());
    if (!obj->webrtc_){
        return Nan::ThrowError("The webrtc is null. `Invoke Initialize firstly` required");
    }

    std::string sdp = obj->sbs_thr_->Invoke<std::string>(RTC_FROM_HERE, rtc::Bind(&WebRtcConnection::GetRemoteSdp, obj->webrtc_.get())); 

    info.GetReturnValue().Set(Nan::Encode(sdp.c_str(), sdp.length(), Nan::Encoding::UTF8));
}

