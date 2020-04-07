#ifndef _PEER_H_
#define _PEER_H_

#include "sbs_decl.h"
#include <memory>
#include <mutex>
#include <unordered_map>

SBS_NAMESPACE_DECL_BEGIN
class Room;
class Publisher;
class Subscriber;
class Peer
{
public:
    Peer(uint32_t id, std::shared_ptr<Room> r);
    ~Peer();

    bool AddPublisher(std::shared_ptr<Publisher> pub);
    bool RemovePublisher(uint32_t id);
    std::shared_ptr<Publisher> GetPublisher(uint32_t id);

    bool AddSubscriber(std::shared_ptr<Subscriber> sub);
    bool RemoveSubscriber(uint32_t pubid);
    std::shared_ptr<Subscriber> GetSubscriber(uint32_t pubid);

    uint32_t id(){return id_;}
    std::shared_ptr<Room> room(){return room_;}

private:
    uint32_t id_;
    std::shared_ptr<Room> room_;
    
    // The key is publisherid
    std::unordered_map<uint32_t, std::shared_ptr<Publisher>> publishers_;
    std::mutex publishers_mutex_;

    // The key is publisher id of the Subscriber belongs to.
    std::unordered_map<uint32_t, std::shared_ptr<Subscriber>> subscribers_;
    std::mutex subscribers_mutex_;
};
SBS_NAMESPACE_DECL_END
#endif//_PEER_H_

