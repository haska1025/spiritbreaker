#ifndef _SUBSCRIBLER_H_
#define _SUBSCRIBLER_H_

#include "sbs_decl.h"
#include <inttypes.h>

SBS_NAMESPACE_DECL_BEGIN

class Subscriber
{
public:
    Subscriber();
    ~Subscriber();

    uint32_t id(){return id_;}
private:
    uint32_t id_;// The id is peerid
};
SBS_NAMESPACE_DECL_END
#endif//_SUBSCRIBLER_H_

