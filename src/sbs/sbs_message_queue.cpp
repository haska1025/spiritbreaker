#include <rtc_base/logging.h>
#include "sbs_message_queue.h"
#include "sbs_error.h"

void MessageQueue::async_cb(uv_async_t *handle)
{
    MessageQueue *queue = (MessageQueue*)handle->data;

    while(1){
        std::function<void ()> callback = queue->pop(); 
        if (!callback){
            break;
        }
        callback();
    }
}

MessageQueue::MessageQueue()
{
}

MessageQueue::~MessageQueue()
{
}

int MessageQueue::init()
{
    uv_mutex_init(&queue_mutex_);

    // Register message queue
    notify_.data = this;
    if (0 != uv_async_init(uv_default_loop(), &notify_, MessageQueue::async_cb)){
        RTC_LOG(LS_ERROR) << "Register notify for queue failed";
        return SBS_GENERAL_ERROR;
    }
    return SBS_SUCCESS;
}
int MessageQueue::push(std::function<void ()> cb)
{
    RTC_LOG(LS_INFO) << "push cb";

    uv_mutex_lock(&queue_mutex_);

    callbacks_.push_back(cb);

    uv_mutex_unlock(&queue_mutex_);

    uv_async_send(&notify_);

    return SBS_SUCCESS;
}
std::function<void ()> MessageQueue::pop()
{
    RTC_LOG(LS_INFO) << "pop cb";

    std::function<void ()> cb;

    uv_mutex_lock(&queue_mutex_);

    if (!callbacks_.empty()){
        cb = callbacks_.front();
        callbacks_.pop_front();
    }

    uv_mutex_unlock(&queue_mutex_);

    return cb;
}

