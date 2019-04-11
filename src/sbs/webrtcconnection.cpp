#include "webrtcconnection.h"
#include "room_mgr.h"
#include "sbs_error.h"

WebRtcConnection::WebRtcConnection()
{
}
WebRtcConnection::~WebRtcConnection()
{
}

int WebRtcConnection::Initialize()
{
    webrtc::JsepTransportController::Config config;
    transport_controller_.reset(new webrtc::JsepTransportController(
                NULL,NULL, NULL, NULL, config));
      return 0;
}

int WebRtcConnection::CreateOffer()
{
    cricket::MediaSessionOptions options;

    local_desc_ = RoomMgr::Instance()->media_session_desc_factory()->CreateOffer(options, NULL);
    if (!local_desc_){
        RTC_LOG(LS_ERROR) << "Create offer failed";
        return SBS_GENERAL_ERROR;
    }


    return SBS_SUCCESS;
}

/*
int WebRtcConnection::CreateLocalSdp()
{
    return 0;
}

std::string WebRtcConnection::GetLocalSdp()
{
    return "";
}
*/
