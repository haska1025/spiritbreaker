#include "room_mgr.h"
#include "sbs_error.h"
#include "configuration.h"

#include <rtc_base/logging.h>
#include <rtc_base/bind.h>
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
    // Create thread
    sbs_thread_ = new rtc::Thread();
    if (!sbs_thread_->Start()){
        RTC_LOG(LS_ERROR) << "Start sbs thread failed!";
        return SBS_ERROR_START_SBS_THREAD_FAILED;
    }

    rc = sbs_thread_->Invoke<int>(RTC_FROM_HERE, rtc::Bind(&RoomMgr::_Initialize, this)); 

    return rc;
}

int RoomMgr::Close()
{
    int rc = SBS_SUCCESS;

    rc = sbs_thread_->Invoke<int>(RTC_FROM_HERE, rtc::Bind(&RoomMgr::_Close, this)); 

    if (sbs_thread_){
        sbs_thread_->Stop();
        sbs_thread_ = nullptr;
    }

    return rc;
}

int RoomMgr::_Initialize()
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

int RoomMgr::_Close()
{
    // No implementation.
    return 0;
}


