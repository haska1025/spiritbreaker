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
    RTC_LOG(LS_INFO) << "Start playout...";

    if (_playing)
        return 0;

    _ptrThreadPlay.reset(new rtc::PlatformThread( PlayThreadFunc, this, "webrtc_audio_module_playout_thread"));

    _ptrThreadPlay->Start();
    _ptrThreadPlay->SetPriority(rtc::kRealtimePriority);

    _playing = true;

    return 0;
}
int32_t DummyAudioDevice::StopPlayout()
{
    return true;
}
bool DummyAudioDevice::Playing() const
{
    return _playing;
}
int32_t DummyAudioDevice::StartRecording()
{
    // RECORDING
    RTC_LOG(LS_INFO) << "Start recording...";

    if (_recording)
        return 0;

    _ptrThreadRec.reset(new rtc::PlatformThread( RecThreadFunc, this, "webrtc_audio_module_capture_thread"));

    _ptrThreadRec->Start();
    _ptrThreadRec->SetPriority(rtc::kRealtimePriority);

    _recording = true;

    return 0;
}
int32_t DummyAudioDevice::StopRecording()
{
       return 0;
}
bool DummyAudioDevice::Recording() const
{
    return _recording;
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
    RTC_LOG(LS_INFO) << "recording sample rate:" << audio_device_buffer_->RecordingSampleRate()
        << " channels:" << audio_device_buffer_->RecordingChannels();

    return true;
}
bool DummyAudioDevice::PlayThreadProcess()
{
    usleep(10*1000);
    RTC_LOG(LS_INFO) << "playout thread sample rate:" << audio_device_buffer_->PlayoutSampleRate() 
        << " channels:" << audio_device_buffer_->PlayoutChannels();

    return true;
}

