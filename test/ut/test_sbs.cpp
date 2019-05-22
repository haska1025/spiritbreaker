#include <cppunit/config/SourcePrefix.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/RepeatedTest.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>

#include <unistd.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include "../../src/sbs/webrtcconnection.h"
#include "../../src/sbs/sbs_log.h"
#include "../../src/sbs/sbs_mgr.h"
#include "../../src/sbs/room_mgr.h"
#include "../../src/sbs/configuration.h"
#include "../../src/sbs/webrtc_connection_interface.h"

using namespace CPPUNIT_NS;

class TestWebRtcConnection
{
public:
    static void test_CreateOffer()
    {
        rtc::scoped_refptr<WebRtcConnection> conn(new rtc::RefCountedObject<WebRtcConnection>());

        std::string exedir = Configuration::GetExeDir();
        CPPUNIT_ASSERT(!exedir.empty());
        std::ifstream offer_stream(exedir + "/offer.sdp");

        CPPUNIT_ASSERT(offer_stream);

        std::stringstream strStream;
        std::string offer;

        strStream << offer_stream.rdbuf();
        offer=strStream.str();

        std::cout << offer << std::endl;

        CPPUNIT_ASSERT(0==conn->Initialize());
        CPPUNIT_ASSERT(0==conn->SetRemoteSdp(offer));
        std::string sdp = conn->GetLocalSdp();
        CPPUNIT_ASSERT(!sdp.empty());

        std::this_thread::sleep_for(std::chrono::seconds(10));

        WebRtcConnectionInterface *conninter = WebRtcConnectionInterface::Create();

        CPPUNIT_ASSERT(0==conninter->Initialize());
        CPPUNIT_ASSERT(0==conninter->SetRemoteSdp(offer));
        sdp = conninter->GetLocalSdp();
        CPPUNIT_ASSERT(!sdp.empty());

        std::this_thread::sleep_for(std::chrono::seconds(10));

        WebRtcConnectionInterface::Delete(conninter);
        conninter = nullptr;
        
        rtc::Thread *pthrMain = rtc::Thread::Current();
        pthrMain->Run();
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
        int rc = 0;
        
        rc = Configuration::Initialize();
        if (rc != 0){
            RTC_LOG(LS_ERROR) << "Initialize configuration failed! rc="<< rc;
            exit(-1);
        }

        rc = RoomMgr::Instance()->Initialize();
        if (rc != 0){
            exit(-1);
        }
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
