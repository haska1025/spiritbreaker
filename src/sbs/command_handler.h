#ifndef _COMMAND_HANDLER_H_
#define _COMMAND_HANDLER_H_

#include "sbs_decl.h"
#include "httpcommon.h"
#include "sbs_message.h"

SBS_NAMESPACE_DECL_BEGIN

typedef int (*cmd_process_fun_t)(const Message &req, Message &rsp);
class CmdProcessorEntry
{
public:
    int verb; 
    const char * cmd_name;
    cmd_process_fun_t cpft;
};

#define CMD_PROCESSOR_DECL() \
    private: \
    const static CmdProcessorEntry * GetProcessorEntry();

#define BEGIN_CMD_PROCESSOR_IMPL() \
    const CmdProcessorEntry * CommandHandler::GetProcessorEntry()\
{ \
    const static CmdProcessorEntry cmd_processors_[] = {

#define END_CMD_PROCESSOR_IMPL() \
        {-1, (const char*)0, (cmd_process_fun_t)0} \
    }; \
    return cmd_processors_; \
}

#define REG_PROCESSOR(verb, cmd_name , fun) \
{verb, cmd_name , fun},

class CommandHandler
{
public:
    static int HandleRequest(HttpRequestData &request, HttpResponseData &response);
private:
    // path=/room/join
    static int CMD_PeerJoinRoom(const Message &req, Message &rsp);
    // path=/room/leave
    static int CMD_PeerLeaveRoom(const Message &req, Message &rsp);
    static int CMD_PeerAddPublisher(const Message &req, Message &rsp);
    static int CMD_PeerRemovePublisher(const Message &req, Message &rsp);
    static int CMD_PeerAddSubscriber(const Message &req, Message &rsp);
    static int CMD_PeerRemoveSubscriber(const Message &req, Message &rsp);
    static int CMD_PeerSetRemoteSDP(const Message &req, Message &rsp);
    static int CMD_PeerGetCandidate(const Message &req, Message &rsp);

    static int __GetBody(rtc::StreamInterface *document, std::string &body);
    static int __SetBody(rtc::StreamInterface *document, const std::string &body);
    static int __JsonToMessage(const Json::Value &req, Message &m);
    static int __MessageToJson(const Message &m, Json::Value &rsp);

    CMD_PROCESSOR_DECL();
};
SBS_NAMESPACE_DECL_END
#endif//_COMMAND_HANDLER_H_

