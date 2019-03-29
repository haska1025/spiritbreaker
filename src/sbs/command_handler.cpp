#include "command_handler.h"


BEGIN_CMD_PROCESSOR_IMPL()
    REG_PROCESSOR(HV_POST, "/room/join" , CMD_PeerJoinRoom)
    REG_PROCESSOR(HV_POST, "/room/leave" , CMD_PeerLeaveRoom)
END_CMD_PROCESSOR_IMPL()

int CommandHandler::HandleRequest(HttpRequestData &request, HttpResponseData &response)
{
    int rc = -1;

    const CmdProcessorEntry *pEntry = GetProcessorEntry();
    for(int i = 0; pEntry[i].cmd_name != NULL; ++i){
        if (request.path.compare(pEntry[i].cmd_name) == 0 && request.verb==pEntry[i].verb){
            Json::Value in, out;
            Json::Reader reader;
//            if (!reader.parse(request.
          
 //           rc = pEntry[i].cpft(cp);
        }
    }
    return rc;
}
// Internal functions
int CommandHandler::CMD_PeerJoinRoom(const Json::Value &req, Json::Value &rsp)
{

    return 0;
}

int CommandHandler::CMD_PeerLeaveRoom(const Json::Value &req, Json::Value &rsp)
{
    return 0;
}

