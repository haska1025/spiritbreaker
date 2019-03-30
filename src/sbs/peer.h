#ifndef _PEER_H_
#define _PEER_H_

#include "sbs_decl.h"

SBS_NAMESPACE_DECL_BEGIN

class Peer
{
public:
    Peer(uint32_t id);
    ~Peer();

    uint32_t id(){return id_;}
private:
    uint32_t id_;
};
SBS_NAMESPACE_DECL_END
#endif//_PEER_H_

