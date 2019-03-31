#include <rtc_base/logging.h>
#include "sbs_log.h"
#include "sbs_mgr.h"

int main(int argc, char *argv[])
{
    int rc = 0;
    rtc::Thread *pthrMain = rtc::Thread::Current();

    rtc::LogMessage::AddLogToStream(new ConsoleLogSink(), rtc::LS_VERBOSE);

    RTC_LOG(LS_ERROR) << "Start spirit breaker....";

    rc = SBSMgr::Instance()->Initialize();
    if (rc != 0){
        RTC_LOG(LS_ERROR) << "Initialize sbs mgr failed!";
        return -1;
    }

    pthrMain->Run();
    return 0;
}

