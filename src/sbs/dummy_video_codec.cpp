#include "dummy_video_codec.h"

static const char kFakeCodecFactoryCodecName[] = "FakeCodec";

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

    //         RTC_LOG(LS_ERROR) << "Decoder recv buffer=" << input_image.data() << " size=" << input_image.size() << "timestamp=" << input_image.Timestamp();
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
    return 0;
}
int32_t DummyVideoEncoder::RegisterEncodeCompleteCallback( webrtc::EncodedImageCallback* callback) 
{
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
    return std::unique_ptr<webrtc::VideoEncoder>(new DummyVideoEncoder());
}

