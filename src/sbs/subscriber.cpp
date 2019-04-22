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

uint32_t Subscriber::peer_id()
{
    return peer_->id();
}
uint32_t Subscriber::pub_id()
{
    publisher_->id();
}

