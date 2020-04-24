#include "subscriber.h"
#include "publisher.h"
#include "dummy_video_codec.h"
#include "peer.h"

extern rtc::Thread *pthrMain; 
extern DummyVideoEncoder *gEncoder;

constexpr int MSGID_SEND_VIDEO = 1; 
constexpr int MSGID_SEND_AUDIO = 2; 

Subscriber::Subscriber(std::shared_ptr<Peer> peer, std::shared_ptr<Publisher> pub)
    :peer_{peer}, publisher_{pub}
{
    RTC_LOG(LS_INFO) << "Create Subscriber peerid=" << peer->id() << " pubid=" << pub->id();
}
Subscriber::~Subscriber()
{
    RTC_LOG(LS_INFO) << "Subscriber peerid=" << peer_->id() << " pubid=" << publisher_->id();
}

int Subscriber::Initialize()
{
    webrtc_conn_ = new rtc::RefCountedObject<WebRtcConnection>();
    return webrtc_conn_->Initialize(true);
}
int Subscriber::Subscribe(Json::Value &value)
{
    int rc = webrtc_conn_->CreateLocalSdp();
    if (rc != 0)
        return rc;

    std::string lsdp, type;
    webrtc_conn_->GetLocalSdp(lsdp, type);
    value["sdp"] = lsdp;
    value["type"] = type; 

    return 0;
}

int Subscriber::SetRemoteSdp(const std::string &sdp, const std::string &type)
{
    return webrtc_conn_->SetRemoteSdp(sdp, type);
}

uint32_t Subscriber::peer_id()
{
    return peer_->id();
}
uint32_t Subscriber::pub_id()
{
    publisher_->id();
}

int Subscriber::PushData(video_decoder_data *data)
{
    std::lock_guard<std::mutex> lg(data_queue_mutex_);
    video_datas_.push_back(data);

    pthrMain->Post(RTC_FROM_HERE, this, MSGID_SEND_VIDEO);
    return 0;
}

int Subscriber::PushAudioData(audio_frame &frame)
{
    std::lock_guard<std::mutex> lg(audio_frame_mutex_);
    audio_frames_.push_back(frame);

//    pthrMain->Post(RTC_FROM_HERE, this, MSGID_SEND_AUDIO);

    return 0;
}

int Subscriber::GetAudioData(audio_frame &frame)
{
    std::lock_guard<std::mutex> lg(audio_frame_mutex_);
    if(!audio_frames_.empty()){
        auto data = audio_frames_.front();
        audio_frames_.pop_front();
        frame = data;
        return 0;
    }

    return -1;
}
void Subscriber::SendVideoData()
{
    std::lock_guard<std::mutex> lg(data_queue_mutex_);
    if (gEncoder){
        while (!video_datas_.empty()){
            auto data = video_datas_.front();
            video_datas_.pop_front();
            gEncoder->SendData(data->input_image, &data->codec_specific_info);
            delete data;
            data = nullptr;
        }
    }
}

void Subscriber::OnMessage(rtc::Message* msg)
{
    if (msg->message_id == MSGID_SEND_VIDEO){
        SendVideoData();
    }
}

