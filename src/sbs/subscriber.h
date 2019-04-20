#ifndef _SUBSCRIBER_H_
#define _SUBSCRIBER_H_

#include "sbs_decl.h"
#include <memory>

SBS_NAMESPACE_DECL_BEGIN
class Peer;
class Publisher;
class Subscriber
{
public:
    Subscriber(std::shared_ptr<Peer> peer, std::shared_ptr<Publisher> pub);
    ~Subscriber();

    uint32_t peer_id();
    uint32_t pub_id();
private:
    std::shared_ptr<Peer> peer_;
    std::shared_ptr<Publisher> publisher_;
};
SBS_NAMESPACE_DECL_END
#endif//_SUBSCRIBER_H_

