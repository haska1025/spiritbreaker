#include "dummy_video_codec.h"
#include "publisher.h"
#include <atomic>

static const char kFakeCodecFactoryCodecName[] = "FakeCodec";
DummyVideoEncoder *gEncoder = NULL;
extern Publisher *pub;

std::atomic<int> requestKeyFrame{0};

int32_t DummyVideoDecoder::InitDecode(const webrtc::VideoCodec* codec_settings,
        int32_t number_of_cores)
{
    return 0;
}

int32_t DummyVideoDecoder::Decode(const webrtc::EncodedImage& input_image,
        bool missing_frames,
        const webrtc::CodecSpecificInfo* codec_specific_info,
        int64_t render_time_ms)
{

//    RTC_LOG(LS_ERROR) << "Decoder recv buffer=" << input_image.data() << " size=" << input_image.size() << "timestamp=" << input_image.Timestamp();
    video_decoder_data *data = new video_decoder_data;

    data->input_image = input_image;

    uint8_t *buffer = (uint8_t*)malloc(input_image.capacity());
    memcpy(buffer, input_image.data(), input_image.capacity()); 
    data->input_image.set_buffer(buffer, input_image.capacity());
    data->input_image.timing_.encode_start_ms = input_image.timing_.encode_start_ms;
    data->input_image.timing_.encode_finish_ms = input_image.timing_.encode_finish_ms;
    data->codec_specific_info = *codec_specific_info;

    pub->OnRecvVideoData(data);
    return 0;
}

int32_t DummyVideoDecoder::RegisterDecodeCompleteCallback( webrtc::DecodedImageCallback* callback)
{
    return 0;
}

int32_t DummyVideoDecoder::Release()
{
    return 0;
}

const char* DummyVideoDecoder::ImplementationName() const
{
    int isPLI = requestKeyFrame.exchange(0);
    return isPLI==1?"htalk:requestkeyframe":"htalk";
}

std::vector<webrtc::SdpVideoFormat> DummyVideoDecoderFactory::GetSupportedFormats() const
{
    return std::vector<webrtc::SdpVideoFormat>();
}

std::unique_ptr<webrtc::VideoDecoder> DummyVideoDecoderFactory::CreateVideoDecoder( const webrtc::SdpVideoFormat& format)
{
    return std::unique_ptr<webrtc::VideoDecoder>(new DummyVideoDecoder());
}

int32_t DummyVideoEncoder::InitEncode(const webrtc::VideoCodec* config,
        int32_t number_of_cores,
        size_t max_payload_size) 
{
    return 0;
}
int32_t DummyVideoEncoder::Encode(const webrtc::VideoFrame& input_image,
        const webrtc::CodecSpecificInfo* codec_specific_info,
        const std::vector<webrtc::FrameType>* frame_types) 
{
    if (frame_types){
        for (auto it = frame_types->begin(); it != frame_types->end(); ++it){
            if (*it == webrtc::FrameType::kVideoFrameKey){
                RTC_LOG(LS_INFO) << "Encode frame type:" << (*it);
                requestKeyFrame.store(1);
                break;
            }
        }
    }
    return 0;
}
int32_t DummyVideoEncoder::RegisterEncodeCompleteCallback( webrtc::EncodedImageCallback* callback) 
{
    callback_ = callback;
    return 0;
}
int32_t DummyVideoEncoder::Release() 
{
    return 0;
}
int32_t DummyVideoEncoder::SetRates(uint32_t bitrate, uint32_t framerate) 
{
    return 0;
}
void DummyVideoEncoder::SendData(const webrtc::EncodedImage& encoded_image,
                    const webrtc::CodecSpecificInfo* codec_specific_info)
{
    //RTC_LOG(LS_INFO) << "Send data............";
    callback_->OnEncodedImage(encoded_image, codec_specific_info, nullptr);
}

std::vector<webrtc::SdpVideoFormat> DummyVideoEncoderFactory::GetSupportedFormats() const 
{
    std::vector<webrtc::SdpVideoFormat> vec;
    vec.push_back(webrtc::SdpVideoFormat("VP8"));
    vec.push_back(webrtc::SdpVideoFormat("VP9"));
    vec.push_back(webrtc::SdpVideoFormat("H264"));

    return vec;
}

webrtc::VideoEncoderFactory::CodecInfo DummyVideoEncoderFactory::QueryVideoEncoder(const webrtc::SdpVideoFormat& format) const 
{
    return webrtc::VideoEncoderFactory::CodecInfo{false, true};
}
std::unique_ptr<webrtc::VideoEncoder> DummyVideoEncoderFactory::CreateVideoEncoder( const webrtc::SdpVideoFormat& format) 
{
    RTC_LOG(LS_INFO) << "Create Encoder";

    gEncoder = new DummyVideoEncoder();
    return std::unique_ptr<webrtc::VideoEncoder>(gEncoder);
}

