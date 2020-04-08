#ifndef _SUBSCRIBER_H_
#define _SUBSCRIBER_H_

#include <memory>
#include <json/json.h>

#include "webrtcconnection.h"
#include "sbs_decl.h"

SBS_NAMESPACE_DECL_BEGIN
class Peer;
class Publisher;
class WebRtcConnection;
class Subscriber
{
public:
    Subscriber(std::shared_ptr<Peer> peer, std::shared_ptr<Publisher> pub);
    ~Subscriber();

    uint32_t peer_id();
    uint32_t pub_id();


    int Initialize();
    int Subscribe(Json::Value &value);

private:
    std::shared_ptr<Peer> peer_;
    std::shared_ptr<Publisher> publisher_;

    // Just send data
    rtc::scoped_refptr<WebRtcConnection> webrtc_conn_;
};
SBS_NAMESPACE_DECL_END
#endif//_SUBSCRIBER_H_

