#include <fstream>
#include <rtc_base/logging.h>

#include <json/json.h>

#include "sbs_log.h"
#include "sbs_mgr.h"


Json::Value g_rtp_media_config;

void read_rtp_media_config()
{
    Json::Reader reader;
    std::ifstream jsonfile("./rtp_media_config.json", std::ifstream::binary);

    if (!reader.parse(jsonfile, g_rtp_media_config, false)){
        RTC_LOG(LS_ERROR) << "Parse rtp media config failed! " << reader.getFormattedErrorMessages();
    }else{
        RTC_LOG(LS_ERROR) << "Parse rtp media config success!";
    }
}

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

    read_rtp_media_config();

    pthrMain->Run();
    return 0;
}

