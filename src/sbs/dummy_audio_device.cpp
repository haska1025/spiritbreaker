#include "dummy_audio_device.h"
#include <rtc_base/logging.h>
#include "publisher.h"

extern Publisher *pub;

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

    audio_device_buffer_->SetPlayoutSampleRate(48000);
    audio_device_buffer_->SetPlayoutChannels(1);

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

    audio_device_buffer_->SetRecordingChannels(1);
    audio_device_buffer_->SetRecordingSampleRate(48000);

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

    audio_frame frame;
    int rc = sub_->GetAudioData(frame);

    audio_device_buffer_->SetRecordedBuffer(frame.samples, frame.samples_per_channel);
    audio_device_buffer_->SetVQEData(0, 0);
    audio_device_buffer_->DeliverRecordedData();

    RTC_LOG(LS_INFO) << "recording sample rate:" << audio_device_buffer_->RecordingSampleRate()
        << " channels:" << audio_device_buffer_->RecordingChannels()
        << " rc: " << rc;
       

    return true;
}
bool DummyAudioDevice::PlayThreadProcess()
{
    usleep(10*1000);

    uint32_t nSamples = audio_device_buffer_->RequestPlayoutData(480);

    RTC_LOG(LS_INFO) << "playout thread sample rate:" << audio_device_buffer_->PlayoutSampleRate() 
        << " channels:" << audio_device_buffer_->PlayoutChannels() << " samples " << nSamples;

    audio_frame frame;
    frame.nchannels = 1;
    frame.samples_per_channel = 480;

    frame.samples = malloc(480*sizeof(uint16_t));

    audio_device_buffer_->GetPlayoutData(frame.samples);

    pub->OnRecvAudioData(frame);

    return true;
}

