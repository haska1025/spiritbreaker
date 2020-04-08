#include "command_handler.h"
#include "memory_stream.h"
#include "sbs_error.h"
#include "sbs_message.h"
#include "room_mgr.h"

#include <rtc_base/logging.h>
#include <rtc_base/strings/json.h>

BEGIN_CMD_PROCESSOR_IMPL()
    REG_PROCESSOR(HV_POST, "/room/peer/join/" , CMD_PeerJoinRoom)
    REG_PROCESSOR(HV_POST, "/room/peer/leave/" , CMD_PeerLeaveRoom)
    REG_PROCESSOR(HV_POST, "/room/peer/publish/" , CMD_PeerAddPublisher)
    REG_PROCESSOR(HV_POST, "/room/peer/unpublish/" , CMD_PeerRemovePublisher)
    REG_PROCESSOR(HV_POST, "/room/peer/subscribe/" , CMD_PeerAddSubscriber)
    REG_PROCESSOR(HV_POST, "/room/peer/unsubscribe/" , CMD_PeerRemoveSubscriber)
    REG_PROCESSOR(HV_POST, "/room/peer/setremotesdp/" , CMD_PeerSetRemoteSDP)
    REG_PROCESSOR(HV_GET, "/room/peer/candidate/" , CMD_PeerGetCandidate)
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

            rc = __GetBody(request.document.get(), body);
            if (rc != SBS_SUCCESS){
                RTC_LOG(LS_ERROR) << "Read body failed.";
                return rc ;
            }

            RTC_LOG(LS_VERBOSE) << "The body:" << body;

            if (body.empty() || !reader.parse(body, in)){
                RTC_LOG(LS_ERROR) << "JsonCpp parse failed.";
                return SBS_ERROR_INVALID_PARAM;
            }

            Message req, rsp;

            __JsonToMessage(in, req);
          
            rsp = req;
            rc = pEntry[i].cpft(req, rsp);

            if (rc == HC_OK){
                __MessageToJson(rsp, out);
                std::string str_response = rtc::JsonValueToString(out);
                RTC_LOG(LS_VERBOSE) << "The answer body:" << str_response;
                if (!str_response.empty()){
                    rtc::StreamInterface *out_doc = new MemoryStream();
                    rc = __SetBody(out_doc, str_response);
                    if (rc != SBS_SUCCESS){
                        RTC_LOG(LS_ERROR) << "Write body failed.";
                        response.set_success(HC_NO_CONTENT);
                        return rc ;
                    }
                    response.set_success("application/json", out_doc, HC_OK);
                }else{
                    response.set_success(HC_BAD_REQUEST);
                }
            }else{
                 response.set_error(rc);
            }
        }
    }
    return rc;
}
// Internal functions
int CommandHandler::CMD_PeerJoinRoom(const Message &req, Message &rsp)
{
    return RoomMgr::Instance()->PeerJoinRoom(req, rsp);
}

int CommandHandler::CMD_PeerLeaveRoom(const Message &req, Message &rsp)
{
    return 0;
}

int CommandHandler::CMD_PeerAddPublisher(const Message &req, Message &rsp)
{
    return RoomMgr::Instance()->AddPublisher(req, rsp);
}
int CommandHandler::CMD_PeerRemovePublisher(const Message &req, Message &rsp)
{
    return 0;
}
int CommandHandler::CMD_PeerAddSubscriber(const Message &req, Message &rsp)
{
    return RoomMgr::Instance()->Subscribe(req, rsp);
}

int CommandHandler::CMD_PeerSetRemoteSDP(const Message &req, Message &rsp)
{
    return RoomMgr::Instance()->SetRemoteSdp(req, rsp);
}

int CommandHandler::CMD_PeerRemoveSubscriber(const Message &req, Message &rsp)
{
    return 0;
}

int CommandHandler::CMD_PeerGetCandidate(const Message &req, Message &rsp)
{
    return RoomMgr::Instance()->GetCandidate(req, rsp);
}

int CommandHandler::__GetBody(rtc::StreamInterface *document, std::string &body)
{
    char buffer[1024];
    size_t nread = 0;
    int rc = SBS_SUCCESS;

    do{
        document->Read(buffer, 1024, &nread, &rc);
        if (rc != 0){
            RTC_LOG(LS_ERROR) << "Get document failed. rc=" << rc;
            break;
        }

        if (nread > 0){
            body.append(buffer, nread);
        }

    }while(nread==1024);

    return rc;
}

int CommandHandler::__SetBody(rtc::StreamInterface *document, const std::string &body)
{
    int rc = 0;
    size_t nwrite = 0;
    size_t datasize = body.size();
    size_t total_nwrite = 0;

    while (total_nwrite < datasize){
        document->Write(body.c_str()+total_nwrite, datasize-total_nwrite, &nwrite , &rc); 
        if (rc != 0){
            RTC_LOG(LS_ERROR) << "Write document failed. rc=" << rc << " body=" << body;
            break;
        }
        total_nwrite += nwrite;
        nwrite = 0;
        rc = 0;
    }
    return rc;
}

int CommandHandler::__JsonToMessage(const Json::Value &req, Message &m)
{
    uint32_t rid = 0;
    if (req["roomid"].isNull() || !rtc::GetUIntFromJson(req["roomid"], &rid)){
        RTC_LOG(LS_ERROR) << "Peer join room invalid roomid";
        return SBS_ERROR_INVALID_PARAM;
    }

    m.room_id(rid);

    std::string cmd;
    if (req["cmd"].isNull() || !rtc::GetStringFromJson(req["cmd"], &cmd)){
        RTC_LOG(LS_ERROR) << "Peer join room invalid cmd";
        return SBS_ERROR_INVALID_PARAM;
    }

    m.cmd_type(cmd);

    uint32_t pid = 0;
    if (req["peerid"].isNull() || !rtc::GetUIntFromJson(req["peerid"], &pid)){
        RTC_LOG(LS_ERROR) << "Peer join room invalid peerid";
        return SBS_ERROR_INVALID_PARAM;
    }

    m.peer_id(pid);

    if (req["data"].isNull()){
        RTC_LOG(LS_WARNING) << "Peer join room message no data.";
    }

    m.data_value(req["data"]);

    return SBS_SUCCESS;
}

int CommandHandler::__MessageToJson(const Message &m, Json::Value &rsp)
{
    rsp["cmd"] = m.cmd_type();
    rsp["roomid"] = m.room_id();
    rsp["peerid"] = m.peer_id();
    rsp["data"] = m.data_value();

    return SBS_SUCCESS;
}

