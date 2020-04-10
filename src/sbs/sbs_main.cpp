#include <rtc_base/logging.h>

#include "sbs_log.h"
#include "sbs_mgr.h"
#include "room_mgr.h"
#include "configuration.h"

rtc::Thread *pthrMain = NULL;

int main(int argc, char *argv[])
{
    int rc = 0;

    rtc::LogMessage::AddLogToStream(new ConsoleLogSink(), rtc::LS_VERBOSE);
    RTC_LOG(LS_ERROR) << "Start spirit breaker....";
/*
    rc = Configuration::Initialize();
    if (rc != 0){
        RTC_LOG(LS_ERROR) << "Initialize configuration failed! rc="<< rc;
        exit(-1);
    }
*/
    rc = RoomMgr::Instance()->Initialize();
    if (rc != 0){
        exit(-1);
    }

    pthrMain = rtc::Thread::Current();

    rc = SBSMgr::Instance()->Initialize();
    if (rc != 0){
        RTC_LOG(LS_ERROR) << "Initialize sbs mgr failed! rc=" << rc;
        exit(-1);
    }

    pthrMain->Run();
    return 0;
}

