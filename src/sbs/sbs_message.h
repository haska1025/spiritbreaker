#ifndef _SBS_MESSAGE_H_
#define _SBS_MESSAGE_H_

#include "sbs_decl.h"
#include <string>
#include <json/json.h>

SBS_NAMESPACE_DECL_BEGIN

class Message
{
public:
    Message(){}
    ~Message(){}

    Message(const std::string &cmd,
            uint32_t rid,
            uint32_t pid,
            Json::Value &d):cmdtype(cmd), roomid(rid), peerid(pid), datavalue(d)
    {
    }

    Message(const Message &m)
    {
        cmdtype = m.cmdtype;
        roomid = m.roomid;
        peerid = m.peerid;
        datavalue = m.datavalue;
    }
    const Message &operator=(const Message &m)
    {
        if (this != &m){
            cmdtype = m.cmdtype;
            roomid = m.roomid;
            peerid = m.peerid;
            datavalue = m.datavalue;
        }
        return *this;
    }

    void cmd_type(const std::string &cmd){cmdtype = cmd;}
    std::string cmd_type() const {return cmdtype;}

    void room_id(uint32_t rid){roomid = rid;}
    uint32_t room_id()const {return roomid;}

    void peer_id(uint32_t pid){peerid = pid;}
    uint32_t peer_id()const{return peerid;}

    void data_value(const Json::Value &v){datavalue = v;}
    Json::Value data_value() const{return datavalue;}

private:
    std::string cmdtype;
    uint32_t roomid;
    uint32_t peerid;
    Json::Value datavalue;
};
SBS_NAMESPACE_DECL_END
#endif//_SBS_MESSAGE_H_

