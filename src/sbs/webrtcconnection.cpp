#include "webrtcconnection.h"


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
                NULL,NULL, NULL, NULL, config));return 0;
      return 0;
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
