#include <rtc_base/logging.h>

#include "sbs_mgr.h"
#include "sbs_error.h"
#include "sbs_log.h"
#include "room_mgr.h"

int SBSMgr::Initialize()
{
    int rc = SBS_SUCCESS;

    rtc::LogMessage::AddLogToStream(new ConsoleLogSink(), rtc::LS_VERBOSE);
    RTC_LOG(LS_ERROR) << "Start spirit breaker....";
 
    rc = RoomMgr::Instance()->Initialize();
    if (rc != SBS_SUCCESS){
        RTC_LOG(LS_ERROR) << "Init room manager failed! rc=" << rc;
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

