#ifndef _SIGNAL_SESSION_H_
#define _SIGNAL_SESSION_H_

#include "sbs_decl.h"

#include <rtc_base/async_tcp_socket.h>

SBS_NAMESPACE_DECL_BEGIN

class SignalSession: public sigslot::has_slots<>
{
public:
    SignalSession();
    ~SignalSession();

    void Accept(rtc::AsyncTCPSocket *socket);

private:
    void OnPacket(rtc::AsyncPacketSocket* socket,
            const char* buf,
            size_t size,
            const rtc::SocketAddress& remote_addr,
            const int64_t& /* packet_time_us */);

    void OnClose(rtc::AsyncPacketSocket* socket, int err);

private:
    std::unique_ptr<rtc::AsyncTCPSocket> socket_;
};
SBS_NAMESPACE_DECL_END
#endif//_SIGNAL_SESSION_H_

