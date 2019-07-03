#include <rtc_base/logging.h>

#include "sbs_mgr.h"
#include "sbs_error.h"
#include "sbs_log.h"
#include "room_mgr.h"
#include "webrtc_connection_wrap.h"

MessageQueue SBSMgr::msg_queue_;

int SBSMgr::Initialize()
{
    int rc = SBS_SUCCESS;

//    rtc::LogMessage::AddLogToStream(new ConsoleLogSink(), rtc::LS_VERBOSE);
    RTC_LOG(LS_ERROR) << "Start spirit breaker....";
    rtc::LogMessage::LogToDebug(rtc::LS_VERBOSE);
 
    rc = RoomMgr::Instance()->Initialize();
    if (rc != SBS_SUCCESS){
        RTC_LOG(LS_ERROR) << "Init room manager failed! rc=" << rc;
        return rc;
    }

    rc = msg_queue_.init();
    if (rc != SBS_SUCCESS){
        RTC_LOG(LS_ERROR) << "Init message queue failed! rc=" << rc;
        return rc;
    }

    //rtc::Thread *pthrMain = rtc::Thread::Current();
    // pthrMain->Run();

    // Maybe need thread pool.
    return rc;
}

int SBSMgr::Close()
{
    RoomMgr::Instance()->Close();
    return 0;
}

#include <nan.h>

void Initialize(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    int rc = SBSMgr::Initialize();
    info.GetReturnValue().Set(rc);
}

void Close(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    int rc = SBSMgr::Initialize();
    info.GetReturnValue().Set(rc);
}

void Init(v8::Local<v8::Object> exports) {
    exports->Set(Nan::New("initialize").ToLocalChecked(),
            Nan::New<v8::FunctionTemplate>(Initialize)->GetFunction());

    exports->Set(Nan::New("close").ToLocalChecked(),
            Nan::New<v8::FunctionTemplate>(Close)->GetFunction());

    WebRtcConnectionWrap::Init(exports);
}

NODE_MODULE(sbs, Init)


