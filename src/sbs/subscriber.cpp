#include "subscriber.h"
#include "publisher.h"
#include "dummy_video_codec.h"
#include "peer.h"

extern rtc::Thread *pthrMain; 
extern DummyVideoEncoder *gEncoder;

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

    pthrMain->Post(RTC_FROM_HERE, this);
    return 0;
}

void Subscriber::OnMessage(rtc::Message* msg)
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
      //  for(auto it = video_datas_.begin(); it != video_datas_.end(); ){
    //        delete (*it);
        //    video_datas_.erase(it++); 
       // }
    }
}

