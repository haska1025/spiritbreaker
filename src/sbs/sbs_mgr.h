#ifndef _SBS_MGR_H_
#define _SBS_MGR_H_

#include <rtc_base/async_socket.h>
#include <rtc_base/thread.h>
#include <rtc_base/third_party/sigslot/sigslot.h>

#include "sbs_decl.h"
#include "httpserver.h"

SBS_NAMESPACE_DECL_BEGIN

class SBSMgr: public sigslot::has_slots<>
{
public:
    ~SBSMgr();

    static SBSMgr * Instance();
    int Initialize();
    int Close();

private:
    void OnHttpRequest(HttpServer* server, HttpServerTransaction* trans); 

private:
    static SBSMgr instance_;

    HttpListenServer *http_listener_;

    SBS_DISALLOW_CONSTRUCT(SBSMgr);
    SBS_DISALLOW_COPY_CONSTRUCT(SBSMgr);
    SBS_DISALLOW_ASSIGNMENT(SBSMgr);
};

SBS_NAMESPACE_DECL_END
#endif//_SBS_MGR_H_

