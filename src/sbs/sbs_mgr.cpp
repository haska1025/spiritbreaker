#include "sbs_mgr.h"
#include <rtc_base/async_tcp_socket.h>

SBSMgr::SBSMgr(rtc::Thread *thr):thread_(thr){}
SBSMgr::~SBSMgr(){}

int SBSMgr::Initialize()
{
    // Get port from configuration
    rtc::SocketAddress serveraddr("0.0.0.0", 9999);
    server_socket_.reset(thread_->socketserver()->CreateAsyncSocket(serveraddr.family(), SOCK_STREAM));

    server_socket_->Bind(serveraddr);
    server_socket_->Listen(5);
    server_socket_->SignalReadEvent.connect(this, &SBSMgr::OnAccept);
    return 0;
}

void SBSMgr::OnAccept(rtc::AsyncSocket* socket) {
    rtc::AsyncSocket* raw_socket = socket->Accept(nullptr);
    if (raw_socket) {
        rtc::AsyncTCPSocket* packet_socket = new rtc::AsyncTCPSocket(raw_socket, false);
    }   
}

