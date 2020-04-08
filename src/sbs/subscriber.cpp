#include "subscriber.h"
#include "publisher.h"
#include "peer.h"

Subscriber::Subscriber(std::shared_ptr<Peer> peer, std::shared_ptr<Publisher> pub)
    :peer_{peer}, publisher_{pub}
{
}
Subscriber::~Subscriber()
{
}

int Subscriber::Initialize()
{
    webrtc_conn_ = new rtc::RefCountedObject<WebRtcConnection>();
    return webrtc_conn_->Initialize();
}
int Subscriber::Subscribe(Json::Value &value)
{
    int rc = webrtc_conn_->CreateLocalSdp();
    if (rc != 0)
        return rc;

    std::string lsdp = webrtc_conn_->GetLocalSdp();
    value["sdp"] = lsdp;
    value["type"] = webrtc::SdpTypeToString(webrtc_conn_->local_sdp_type());

    return 0;
}
uint32_t Subscriber::peer_id()
{
    return peer_->id();
}
uint32_t Subscriber::pub_id()
{
    publisher_->id();
}

