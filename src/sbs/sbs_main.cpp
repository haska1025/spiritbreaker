#include <fstream>
#include <rtc_base/logging.h>
#include <rtc_base/strings/json.h>

#include <json/json.h>

#include "sbs_log.h"
#include "sbs_mgr.h"


Json::Value g_rtp_media_config;

int read_rtp_media_config()
{
    Json::Reader reader;
    std::ifstream jsonfile("/home/zixing.chen/spiritbreaker/build/sbs/rtp_media_config.json", std::ifstream::binary);
    if (!jsonfile){
        RTC_LOG(LS_ERROR) << "Open media config file failed.";
        return -1;
    }

    if (!reader.parse(jsonfile, g_rtp_media_config, false)){
        RTC_LOG(LS_ERROR) << "Parse rtp media config failed! " << reader.getFormattedErrorMessages();
        return -1;
    }else{
        RTC_LOG(LS_INFO) << "Parse rtp media config success!";
        RTC_LOG(LS_INFO) << "The json content====" << rtc::JsonValueToString(g_rtp_media_config);
    }
    return 0;
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
        exit(-1);
    }

    rc = read_rtp_media_config();
    if (rc != 0){
        exit(-1);
    }

    pthrMain->Run();
    return 0;
}

