#include "sbs_log.h"
#include <iostream>
#include <syslog.h>

void ConsoleLogSink::OnLogMessage(const std::string& message)
{
}

void ConsoleLogSink::OnLogMessage(const std::string& message, rtc::LoggingSeverity severity)
{
    int level = 3;
    if (severity == rtc::LS_VERBOSE)
        level = 7;
    else if (severity == rtc::LS_INFO)
        level = 6;
    else if (severity == rtc::LS_WARNING)
        level = 4;
    else if (severity == rtc::LS_ERROR)
        level = 3;

    syslog(LOG_LOCAL3|level, "%s:%d:(%s): %s" , __FILE__,__LINE__,__FUNCTION__, message.c_str());
}

