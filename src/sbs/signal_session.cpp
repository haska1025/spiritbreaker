#include "signal_session.h"

SignalSession::SignalSession()
{
}
SignalSession::~SignalSession()
{
}

void SignalSession::Accept(rtc::AsyncTCPSocket *socket)
{
    socket_.reset(socket);
    socket_->SignalReadPacket.connect(this, &SignalSession::OnPacket);
    socket_->SignalClose.connect(this, &SignalSession::OnClose);
}
void SignalSession::OnPacket(rtc::AsyncPacketSocket* socket,
        const char* buf,
        size_t size,
        const rtc::SocketAddress& remote_addr,
        const int64_t& /* packet_time_us */) {
    //rtc::PacketOptions options;
    //socket->Send(buf, size, options);
}
void SignalSession::OnClose(rtc::AsyncPacketSocket* socket, int err) {
    /*
    ClientList::iterator it =
        std::find(client_sockets_.begin(), client_sockets_.end(), socket);
    client_sockets_.erase(it);
    Thread::Current()->Dispose(socket);
    */
}
