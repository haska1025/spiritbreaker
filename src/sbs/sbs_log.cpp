#include "sbs_log.h"
#include <iostream>

void ConsoleLogSink::OnLogMessage(const std::string& message)
{
    std::cout << message << std::endl;
}

