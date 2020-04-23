#include "dummy_audio_device.h"
#include <rtc_base/logging.h>

int32_t DummyAudioDevice::RegisterAudioCallback(webrtc::AudioTransport* audioCallback)
{
    RTC_LOG(LS_INFO) << __FUNCTION__;
    if (!audio_device_buffer_){
        audio_device_buffer_ = std::unique_ptr<webrtc::AudioDeviceBuffer>(new webrtc::AudioDeviceBuffer());
    }
    audio_device_buffer_->RegisterAudioCallback(audioCallback);
}

int32_t DummyAudioDevice::StartPlayout()
{
 // RECORDING
    _ptrThreadPlay.reset(new rtc::PlatformThread( RecThreadFunc, this, "webrtc_audio_module_playout_thread"));

    _ptrThreadPlay->Start();
    _ptrThreadPlay->SetPriority(rtc::kRealtimePriority);

    return 0;
}
int32_t DummyAudioDevice::StopPlayout()
{
    return true;
}
bool DummyAudioDevice::Playing() const
{
    return true;
}
int32_t DummyAudioDevice::StartRecording()
{
    // RECORDING
    RTC_LOG(LS_INFO) << "Start recording...";
    _ptrThreadRec.reset(new rtc::PlatformThread( RecThreadFunc, this, "webrtc_audio_module_capture_thread"));

    _ptrThreadRec->Start();
    _ptrThreadRec->SetPriority(rtc::kRealtimePriority);

    return 0;
}
int32_t DummyAudioDevice::StopRecording()
{
       return 0;
}
bool DummyAudioDevice::Recording() const
{
    return true;
}

bool DummyAudioDevice::PlayThreadFunc(void* pThis)
{
      return (static_cast<DummyAudioDevice*>(pThis)->PlayThreadProcess());
}

bool DummyAudioDevice::RecThreadFunc(void* pThis)
{
      return (static_cast<DummyAudioDevice*>(pThis)->RecThreadProcess());
}

bool DummyAudioDevice::RecThreadProcess()
{
    usleep(10*1000);

    RTC_LOG(LS_INFO) << "sample rate:" << audio_device_buffer_->RecordingChannels();

    return true;
}
bool DummyAudioDevice::PlayThreadProcess()
{
    return true;
}

