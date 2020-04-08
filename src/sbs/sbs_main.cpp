#include <rtc_base/logging.h>


#include "sbs_log.h"
#include "sbs_mgr.h"
#include "room_mgr.h"
#include "configuration.h"

int main(int argc, char *argv[])
{
    int rc = 0;

    rtc::LogMessage::AddLogToStream(new ConsoleLogSink(), rtc::LS_INFO);
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

    rtc::Thread *pthrMain = rtc::Thread::Current();

    rc = SBSMgr::Instance()->Initialize();
    if (rc != 0){
        RTC_LOG(LS_ERROR) << "Initialize sbs mgr failed! rc=" << rc;
        exit(-1);
    }

    pthrMain->Run();
    return 0;
}

