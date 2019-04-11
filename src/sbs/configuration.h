#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <json/json.h>
#include <rtc_base/logging.h>
#include <rtc_base/strings/json.h>
#include <absl/strings/str_split.h> 

#include <pc/media_session.h>

#include "sbs_decl.h"
#include "sbs_error.h"

SBS_NAMESPACE_DECL_BEGIN

class Configuration
{
public:
    Configuration()=delete;
    Configuration(const Configuration& c)=delete;
    ~Configuration()=delete;
    Configuration &operator =(const Configuration &)=delete;

    static int Initialize();

    template <typename T> 
        static int GetCodecs(const std::string &mediatype, std::vector<T> &codecs);
    static std::string &GetExeDir() {return executable_dir;}
private:
    static int ParseMediaConfig();
    static int GetCWD();
private:
    static Json::Value rtp_media_config;
    static std::string executable_dir;
};

template <typename T> 
int Configuration::GetCodecs(const std::string &mediatype, std::vector<T> &codecs)
{
    std::string mode;
    if (rtp_media_config[0]["mode"].isNull() || rtc::GetStringFromJson(rtp_media_config[0]["mode"], &mode)){
        RTC_LOG(LS_ERROR) << "Get media codes failed. Invalid mode.";
        return SBS_ERROR_RTP_CFG_INVALID_MODE;
    }

    Json::Value media = rtp_media_config[1][mode]["rtpMappings"][mediatype];
    int media_size = media.size();
    for ( int i=0; i < media_size;++i){
        T codec;
        rtc::GetIntFromJson(media[i]["payloadType"], &codec.id);
        rtc::GetStringFromJson(media[i]["encodingName"], &codec.name);
        rtc::GetIntFromJson(media[i]["clockRate"], &codec.clockrate);

        rtc::GetUIntFromJson(media[i]["channels"], (uint32_t*)&codec.channels);

        if(!media[i]["formatParameters"].isNull()){
            std::string level_id;
            rtc::GetStringFromJson(media[i]["formatParameters"]["profile-level-id"], &level_id);
            codec.SetParam("profile-level-id", level_id);

            int level_allow = 0;
            rtc::GetIntFromJson(media[i]["formatParameters"]["level-asymmetry-allowed"], &level_allow);
            codec.SetParam("level-asymmetry-allowed", level_allow);

            int pmode= 0;
            rtc::GetIntFromJson(media[i]["formatParameters"]["packetization-mode"], &pmode);
            codec.SetParam("level-asymmetry-allowed", pmode);
        }

        if (!media[i]["feedbackTypes"].isNull()){
            for(int k=0; k < media[k]["feedbackTypes"].size(); ++k){
                std::string v;
                rtc::GetStringFromJson(media[i]["feedbackTypes"][k], &v);
                std::vector<std::string> kv = absl::StrSplit(v, " ");
                cricket::FeedbackParam param(kv[0],kv.size() > 1?kv[1]:"");
                codec.AddFeedbackParam(param);
            }
        }

        codecs.push_back(codec);
    }
}

SBS_NAMESPACE_DECL_END
#endif//_CONFIGURATION_H_

