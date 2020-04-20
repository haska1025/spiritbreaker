#ifndef _DUMMY_VIDEO_CODEC_H_
#define _DUMMY_VIDEO_CODEC_H_

#include "api/video_codecs/video_decoder_factory.h"
#include "api/video_codecs/video_encoder_factory.h"
#include "api/video_codecs/video_decoder.h"
#include "api/video_codecs/video_encoder.h"
#include "api/video_codecs/sdp_video_format.h"

class DummyVideoDecoder : public webrtc::VideoDecoder
{
public:
     virtual int32_t InitDecode(const webrtc::VideoCodec* codec_settings,
                                          int32_t number_of_cores) override;

     virtual int32_t Decode(const webrtc::EncodedImage& input_image,
             bool missing_frames,
             const webrtc::CodecSpecificInfo* codec_specific_info,
             int64_t render_time_ms) override;

     virtual int32_t RegisterDecodeCompleteCallback( webrtc::DecodedImageCallback* callback) override;

     virtual int32_t Release() override;
     virtual const char* ImplementationName() const override;
};

class DummyVideoDecoderFactory : public webrtc::VideoDecoderFactory
{
public:
    std::vector<webrtc::SdpVideoFormat> GetSupportedFormats() const override;
    std::unique_ptr<webrtc::VideoDecoder> CreateVideoDecoder( const webrtc::SdpVideoFormat& format) override;
};

class DummyVideoEncoder : public webrtc::VideoEncoder
{
public:
    int32_t InitEncode(const webrtc::VideoCodec* config,
            int32_t number_of_cores,
            size_t max_payload_size) override;
    int32_t Encode(const webrtc::VideoFrame& input_image,
            const webrtc::CodecSpecificInfo* codec_specific_info,
            const std::vector<webrtc::FrameType>* frame_types) override;
    int32_t RegisterEncodeCompleteCallback( webrtc::EncodedImageCallback* callback) override;
    int32_t Release() override;
    int32_t SetRates(uint32_t bitrate, uint32_t framerate) override;
  //  int32_t SetRateAllocation(const webrtc::VideoBitrateAllocation& rate_allocation, uint32_t framerate) override;
 //   int GetConfiguredInputFramerate() const;
//    webrtc::EncoderInfo GetEncoderInfo() const override;
    void SendData(const webrtc::EncodedImage& encoded_image,
                    const webrtc::CodecSpecificInfo* codec_specific_info);
    
    webrtc::EncodedImageCallback *callback_ = {nullptr};
};

class DummyVideoEncoderFactory : public webrtc::VideoEncoderFactory
{
public:
    virtual std::vector<webrtc::SdpVideoFormat> GetSupportedFormats() const override;
    virtual webrtc::VideoEncoderFactory::CodecInfo QueryVideoEncoder(const webrtc::SdpVideoFormat& format) const override;
    virtual std::unique_ptr<webrtc::VideoEncoder> CreateVideoEncoder( const webrtc::SdpVideoFormat& format) override;
};
#endif//_DUMMY_VIDEO_CODEC_H_

