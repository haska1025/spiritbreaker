#include <rtc_base/async_tcp_socket.h>
#include <rtc_base/logging.h>

#include "sbs_mgr.h"
#include "sbs_error.h"
#include "command_handler.h"

SBSMgr SBSMgr::instance_;

SBSMgr::SBSMgr()
{
    http_listener_ = nullptr;
}

SBSMgr::~SBSMgr(){}

SBSMgr * SBSMgr::Instance()
{
    return &instance_;
}
int SBSMgr::Initialize()
{
    int rc = SBS_SUCCESS;
    // Get port from configuration
    rtc::SocketAddress serveraddr("0.0.0.0", 9999);

    http_listener_ = new HttpListenServer(); 
    rc = http_listener_->Listen(serveraddr);

    if (rc != SBS_SUCCESS){
        RTC_LOG(LS_ERROR) << "Listen failed! rc=" << rc;
        return rc;
    }

    http_listener_->SignalHttpRequest.connect(this, &SBSMgr::OnHttpRequest);
    return rc;
}

void SBSMgr::OnHttpRequest(HttpServer* server, HttpServerTransaction* trans)
{
    RTC_LOG(LS_INFO) << "The request verb=" << trans->request.verb << " path=" << trans->request.path;
    int rc = CommandHandler::HandleRequest(trans->request, trans->response);
    server->Respond(trans);
}

int SBSMgr::Close()
{
    if (http_listener_ != nullptr){
        delete http_listener_;
        http_listener_ = nullptr;
    }
    return 0;
}

