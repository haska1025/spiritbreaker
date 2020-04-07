#include "publisher.h"
#include "subscriber.h"

Publisher::Publisher(uint32_t id, std::shared_ptr<Peer> peer):id_(id), peer_{peer}
{
    webrtc_conn_ = new rtc::RefCountedObject<WebRtcConnection>();
    webrtc_conn_->Initialize();
}
Publisher::~Publisher()
{
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

int Publisher::SetRemoteSdp(const std::string &sdp, std::string &lsdp)
{
    webrtc_conn_->SetRemoteSdp(sdp);
    lsdp = webrtc_conn_->GetLocalSdp();

    return 0;
}
