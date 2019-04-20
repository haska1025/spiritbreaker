#ifndef _PUBLISHER_H_
#define _PUBLISHER_H_

#include "sbs_decl.h"
#include <unordered_map>
#include <memory>

SBS_NAMESPACE_DECL_BEGIN
class Subscriber;
class Publisher
{
public:
    Publisher(uint32_t id);
    ~Publisher();

    bool AddSubsciber(std::shared_ptr<Subscriber> s);
    bool RemoveSubsciber(uint32_t id);
    std::shared_ptr<Subscriber> GetSubsciber(uint32_t id);

    uint32_t id(){return id_;}
private:
    uint32_t id_;
    // The key is the peer id of the Subscriber belongs to.
    std::unordered_map<uint32_t, std::shared_ptr<Subscriber>> subscribers_;
};
SBS_NAMESPACE_DECL_END
#endif//_PUBLISHER_H_

