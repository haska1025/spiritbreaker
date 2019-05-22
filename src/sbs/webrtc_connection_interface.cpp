#include "webrtc_connection_interface.h"
#include "webrtcconnection.h"


WebRtcConnectionInterface::~WebRtcConnectionInterface()
{
}
WebRtcConnectionInterface * WebRtcConnectionInterface::Create()
{
    WebRtcConnection *conn = new rtc::RefCountedObject<WebRtcConnection>();
    conn->AddRef();
    return conn;
}

void WebRtcConnectionInterface::Delete(WebRtcConnectionInterface * conn)
{
    WebRtcConnection * webrtcconn = (WebRtcConnection*)conn;
    webrtcconn->Release();
}

