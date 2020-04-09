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
    return webrtc_conn_->Initialize(true);
}
int Subscriber::Subscribe(Json::Value &value)
{
    int rc = webrtc_conn_->CreateLocalSdp();
    if (rc != 0)
        return rc;

    std::string lsdp, type;
    webrtc_conn_->GetLocalSdp(lsdp, type);
    value["sdp"] = lsdp;
    value["type"] = type; 

    return 0;
}

int Subscriber::SetRemoteSdp(const std::string &sdp, const std::string &type)
{
    return webrtc_conn_->SetRemoteSdp(sdp, type);
}

uint32_t Subscriber::peer_id()
{
    return peer_->id();
}
uint32_t Subscriber::pub_id()
{
    publisher_->id();
}

