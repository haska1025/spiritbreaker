#include "sbs_mgr.h"
#include <rtc_base/async_tcp_socket.h>

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
    int rc = 0;
    // Get port from configuration
    rtc::SocketAddress serveraddr("0.0.0.0", 9999);

    http_listener_ = new HttpListenServer(); 
    rc = http_listener_->Listen(serveraddr);

    if (rc != 0){
        printf("Listen failed!rc(%d)\n", rc);
        return -1;
    }

    http_listener_->SignalHttpRequest.connect(this, &SBSMgr::OnHttpRequest);
    return 0;
}

void SBSMgr::OnHttpRequest(HttpServer* server, HttpServerTransaction* trans)
{
    printf("The request verb(%d) path(%s)\n", trans->request.verb, trans->request.path.c_str());
    trans->response.set_success();
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

