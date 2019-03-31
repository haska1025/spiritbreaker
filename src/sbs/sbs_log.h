#ifndef _SBS_LOG_H_
#define _SBS_LOG_H_

#include "sbs_decl.h"
#include <rtc_base/logging.h>

SBS_NAMESPACE_DECL_BEGIN

class ConsoleLogSink : public rtc::LogSink
{
public:
    virtual void OnLogMessage(const std::string& message);
};
SBS_NAMESPACE_DECL_END
#endif//_SBS_LOG_H_

