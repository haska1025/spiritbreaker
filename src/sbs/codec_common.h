#ifndef _CODEC_COMMON_H_
#define _CODEC_COMMON_H_

#include "modules/video_coding/include/video_codec_interface.h"

class video_decoder_data
{
public:
    webrtc::EncodedImage input_image;
    bool missing_frames;
    webrtc::CodecSpecificInfo codec_specific_info;
    int64_t render_time_ms;
};

class audio_frame
{
public:
    void *samples;
    uint32_t samples_per_channel;
    int nchannels; 
};
#endif//_CODEC_COMMON_H_

