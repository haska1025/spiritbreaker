#include "publisher.h"
#include "subscriber.h"
#include <string>

Publisher::Publisher(uint32_t id, std::shared_ptr<Peer> peer):id_(id), peer_{peer}
{
}
Publisher::~Publisher()
{
}

int Publisher::Initialize()
{
    webrtc_conn_ = new rtc::RefCountedObject<WebRtcConnection>();
    return webrtc_conn_->Initialize();
}

bool Publisher::AddSubscriber(std::shared_ptr<Subscriber> s)
{
    std::lock_guard<std::mutex> lg(subscribers_mutex_);

    if (subscribers_.find(s->peer_id()) == subscribers_.end()){
        subscribers_[s->peer_id()] = s;
        return true;
    }

    return false;
}
bool Publisher::RemoveSubscriber(uint32_t peerid)
{
    std::lock_guard<std::mutex> lg(subscribers_mutex_);

    auto it = subscribers_.find(peerid);
    if (it != subscribers_.end()){
        subscribers_.erase(it);
        return true;
    }

    return false;
}

std::shared_ptr<Subscriber> Publisher::GetSubscriber(uint32_t peerid)
{
    std::lock_guard<std::mutex> lg(subscribers_mutex_);

    auto it = subscribers_.find(peerid);
    if (it != subscribers_.end()){
        return it->second;
    }

    return std::shared_ptr<Subscriber>(nullptr);
}

int Publisher::SetRemoteSdp(const std::string &sdp, const std::string &type, Json::Value &value)
{
    webrtc_conn_->SetRemoteSdp(sdp, type);
    std::string lsdp, ltype;
    webrtc_conn_->GetLocalSdp(lsdp, ltype);
    value["sdp"] = lsdp;
    value["type"] = ltype;
    return 0;
}

