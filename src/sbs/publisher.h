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
    Publisher();
    ~Publisher();

    uint32_t id(){return id_;}
private:
    uint32_t id_;
    std::unordered_map<uint32_t, std::shared_ptr<Subscriber>> subscribers_;
};
SBS_NAMESPACE_DECL_END
#endif//_PUBLISHER_H_

