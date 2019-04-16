#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <rtc_base/logging.h>
#include "../../src/sbs/sbs_log.h"

int main(int argc, char *argv[])
{
    rtc::LogMessage::AddLogToStream(new ConsoleLogSink(), rtc::LS_VERBOSE);

    CppUnit::TextUi::TestRunner runner;
    runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
    return !runner.run();
}
