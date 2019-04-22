#include <fstream>

#include <libgen.h>
#include <unistd.h>


#include "configuration.h"


Json::Value Configuration::rtp_media_config;
std::string Configuration::executable_dir;

int Configuration::Initialize()
{
    int rc = SBS_SUCCESS;
    
    rc = GetCWD();
    if (rc != SBS_SUCCESS)
        return rc;

    rc = ParseMediaConfig();
    if (rc != SBS_SUCCESS)
        return rc;

    return rc;
}

int Configuration::ParseMediaConfig()
{
    Json::Reader reader;
    std::string filepath = executable_dir;
    filepath += "/rtp_media_config.json"; 
    std::ifstream jsonfile(filepath, std::ifstream::binary);
    if (!jsonfile){
        RTC_LOG(LS_ERROR) << "Open media config file failed. path="<< filepath;
        return SBS_ERROR_READ_FILE_FAILED;
    }

    if (!reader.parse(jsonfile, rtp_media_config, false)){
        RTC_LOG(LS_ERROR) << "Parse rtp media config failed! " << reader.getFormattedErrorMessages();
        return SBS_ERROR_JSON_PARSE_FAILED;
    }else{
        RTC_LOG(LS_INFO) << "Parse rtp media config success!";
        RTC_LOG(LS_INFO) << "The json content====" << rtc::JsonValueToString(rtp_media_config);
    }
 
    return SBS_SUCCESS;
}


int Configuration::GetCWD()
{
    int rc = SBS_SUCCESS;
    //Get executable dir
    char cwd[256];
    rc = readlink("/proc/self/exe" , cwd, sizeof(cwd));
    if (rc == -1) {
        RTC_LOG(LS_ERROR) << "readlink failed, errno=" << errno;
        return SBS_ERROR_GET_CWD_FAILED;
    }else{
        rc = SBS_SUCCESS;
    }

    RTC_LOG(LS_ERROR) << "The spirit breaker executable cwdpath=" << cwd;

    cwd[255] = '\0';
    executable_dir = dirname(cwd); 

    RTC_LOG(LS_ERROR) << "The spirit breaker executable exedir=" << executable_dir;

    return rc;
}

