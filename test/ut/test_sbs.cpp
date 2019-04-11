#include <cppunit/config/SourcePrefix.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/RepeatedTest.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>

#include <unistd.h>
#include <string.h>
#include "../../src/sbs/webrtcconnection.h"

using namespace CPPUNIT_NS;

class TestWebRtcConnection
{
public:
    static void test_CreateOffer()
    {
        WebRtcConnection conn;
        CPPUNIT_ASSERT(0==conn.CreateOffer());
    }
};

class TestSBS : public TestFixture
{
    CPPUNIT_TEST_SUITE(TestSBS);
    CPPUNIT_TEST(test_createoffer);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp()
    {
    }
    void setDown()
    {
    }

    void test_createoffer()
    {
        TestWebRtcConnection::test_CreateOffer();
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSBS);
