#ifndef _SUBSCRIBER_H_
#define _SUBSCRIBER_H_

#include <memory>
#include <deque>
#include <mutex>

#include <json/json.h>
#include <rtc_base/message_handler.h>

#include "webrtcconnection.h"
#include "sbs_decl.h"
#include "codec_common.h"

SBS_NAMESPACE_DECL_BEGIN
class Peer;
class Publisher;
class WebRtcConnection;

class Subscriber : public rtc::MessageHandler
{
public:
    Subscriber(std::shared_ptr<Peer> peer, std::shared_ptr<Publisher> pub);
    ~Subscriber();

    uint32_t peer_id();
    uint32_t pub_id();


    int Initialize();
    int Subscribe(Json::Value &value);
    int SetRemoteSdp(const std::string &sdp, const std::string &type);

    int PushData(video_decoder_data *data);
    void SendVideoData();

    Json::Value GetCandidate(){return webrtc_conn_->GetCandidate();}

    int GetAudioData(audio_frame &frame);
    int PushAudioData(audio_frame &frame);

    virtual void OnMessage(rtc::Message* msg) override;

private:
    std::shared_ptr<Peer> peer_;
    std::shared_ptr<Publisher> publisher_;

    // Just send data
    rtc::scoped_refptr<WebRtcConnection> webrtc_conn_;

    std::deque<video_decoder_data*> video_datas_;
    std::mutex data_queue_mutex_;

    std::deque<audio_frame> audio_frames_;
    std::mutex audio_frame_mutex_;
};
SBS_NAMESPACE_DECL_END
#endif//_SUBSCRIBER_H_

