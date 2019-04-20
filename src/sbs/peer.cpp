#include "peer.h"
#include "room.h"
#include "publisher.h"
#include "subscriber.h"

#include <rtc_base/logging.h>

Peer::Peer(uint32_t id, std::shared_ptr<Room> r):id_(id), room_(r)
{
    RTC_LOG(LS_INFO) << "Create Peer id=" << id_ << " roomid=" << room_->id();
}
Peer::~Peer()
{
    RTC_LOG(LS_INFO) << "Destroy Peer id=" << id_;
}

bool Peer::AddPublisher(std::shared_ptr<Publisher> pub)
{
    std::lock_guard<std::mutex> lg(publishers_mutex_);

    if (publishers_.find(pub->id()) == publishers_.end()){
        publishers_[pub->id()] = pub;
        return true;
    }

    return false;
}
bool Peer::RemovePublisher(uint32_t id)
{
    std::lock_guard<std::mutex> lg(publishers_mutex_);

    auto it = publishers_.find(id);
    if (it != publishers_.end()){
        publishers_.erase(id);
        return true;
    }
    return false;
}
std::shared_ptr<Publisher> Peer::GetPublisher(uint32_t id)
{
    std::lock_guard<std::mutex> lg(publishers_mutex_);

    auto it = publishers_.find(id);
    if (it != publishers_.end()){
        return it->second;
    }
    return std::shared_ptr<Publisher>(nullptr);
}

bool Peer::AddSubsciber(std::shared_ptr<Subscriber> sub)
{
    std::lock_guard<std::mutex> lg(subscribers_mutex_);

    if (subscribers_.find(sub->peer_id()) == subscribers_.end()){
        subscribers_[sub->peer_id()] = sub;
        return true;
    }

    return false;
}
bool Peer::RemoveSubsciber(uint32_t peerid)
{
    std::lock_guard<std::mutex> lg(subscribers_mutex_);

    auto it = subscribers_.find(peerid);
    if (it != subscribers_.end()){
        subscribers_.erase(it);
        return true;
    }
    return false;
}
std::shared_ptr<Subscriber> Peer::GetSubsciber(uint32_t peerid)
{
    std::lock_guard<std::mutex> lg(subscribers_mutex_);

    auto it = subscribers_.find(peerid);
    if (it != subscribers_.end()){
        return it->second;
    }
    return std::shared_ptr<Subscriber>(nullptr);
}


