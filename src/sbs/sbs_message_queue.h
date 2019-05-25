#ifndef _SBS_MESSAGE_QUEUE_H_
#define _SBS_MESSAGE_QUEUE_H_

#include <uv.h>
#include <functional>
#include <deque>

#include "sbs_decl.h"

SBS_NAMESPACE_DECL_BEGIN

class MessageQueue
{
public:
    MessageQueue();
    ~MessageQueue();

    int init();
    int push(std::function<void ()> cb);
    std::function<void ()> pop();

    static void async_cb(uv_async_t *handle);
private:
    uv_async_t notify_;
    uv_mutex_t queue_mutex_;

    std::deque<std::function<void ()>> callbacks_;
};

SBS_NAMESPACE_DECL_END
#endif//_SBS_MESSAGE_QUEUE_H_

