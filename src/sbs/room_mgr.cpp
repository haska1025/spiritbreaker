#include "room_mgr.h"
#include "sbs_error.h"
#include "configuration.h"

#include <rtc_base/logging.h>
#include <api/audio_codecs/audio_decoder_factory.h>
#include <api/audio_codecs/audio_encoder_factory.h>
#include <api/audio_codecs/builtin_audio_decoder_factory.h>
#include <api/audio_codecs/builtin_audio_encoder_factory.h>
#include <api/create_peerconnection_factory.h>

#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "api/video_codecs/video_decoder_factory.h"
#include "api/video_codecs/video_encoder_factory.h"


RoomMgr::RoomMgr()
{
}
RoomMgr RoomMgr::instance_;

RoomMgr *RoomMgr::Instance()
{
    return &instance_;
}

int RoomMgr::Initialize()
{
    int rc = SBS_SUCCESS;

    peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(
            nullptr /* network_thread */,
            nullptr /* worker_thread */,
            nullptr /* signaling_thread */, 
            nullptr /* default_adm */,
            webrtc::CreateBuiltinAudioEncoderFactory(),
            webrtc::CreateBuiltinAudioDecoderFactory(),
            webrtc::CreateBuiltinVideoEncoderFactory(),
            webrtc::CreateBuiltinVideoDecoderFactory(),
            nullptr /* audio_mixer */,
            nullptr /* audio_processing */);

    if (!peer_connection_factory_) {
        RTC_LOG(LS_ERROR) << "Create peer connection factory failed!";
        return SBS_GENERAL_ERROR;
    }

    return rc;
}

int RoomMgr::Close()
{
    return 0;
}


