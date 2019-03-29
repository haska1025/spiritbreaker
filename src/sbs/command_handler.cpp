#include "command_handler.h"
#include "memory_stream.h"
#include <rtc_base/logging.h>
#include <rtc_base/strings/json.h>

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
            std::string body;
            __GetBody(request.document.get(), body);
            if (body.empty() || !reader.parse(body, in)){
                RTC_LOG(LS_ERROR) << "JsonCpp parse failed.";
                return -1;
            }
          
            rc = pEntry[i].cpft(in, out);
            std::string str_response = rtc::JsonValueToString(out);
            rtc::StreamInterface *out_doc = new ExternalMemoryStream();
            if (rc == HC_OK){
                if (!str_response.empty()){
                    __SetBody(out_doc, str_response);
                    response.set_success("Application/json", out_doc, rc);
                }else{
                    response.set_success(rc);
                }
            }else{
                 response.set_error(rc);
            }
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

void CommandHandler::__GetBody(rtc::StreamInterface *document, std::string &body)
{
    char buffer[1024];
    size_t nread = 0;
    int rc = 0;
    do{
        document->Read(buffer, 1024, &nread, &rc);
        if (rc != 0)
            break;
        if (nread > 0){
            body.append(buffer, nread);
        }

    }while(nread==1024);
}

void CommandHandler::__SetBody(rtc::StreamInterface *document, const std::string &body)
{
}

