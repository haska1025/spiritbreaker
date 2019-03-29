#ifndef _COMMAND_HANDLER_H_
#define _COMMAND_HANDLER_H_

#include "sbs_decl.h"
#include "httpcommon.h"
#include <json/json.h>

SBS_NAMESPACE_DECL_BEGIN

typedef int (*cmd_process_fun_t)(const Json::Value &req, Json::Value &rsp);
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
    static int CMD_PeerJoinRoom(const Json::Value &req, Json::Value &rsp);
    // path=/room/leave
    static int CMD_PeerLeaveRoom(const Json::Value &req, Json::Value &rsp);

    CMD_PROCESSOR_DECL();
};
SBS_NAMESPACE_DECL_END
#endif//_COMMAND_HANDLER_H_

