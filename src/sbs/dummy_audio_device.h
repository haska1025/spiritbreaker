#ifndef _DUMMY_AUDIO_DEVICE_H_
#define _DUMMY_AUDIO_DEVICE_H_

#include <modules/audio_device/include/fake_audio_device.h>
#include <modules/audio_device/audio_device_buffer.h>
#include "rtc_base/platform_thread.h"
#include "subscriber.h"

class DummyAudioDevice : public webrtc::FakeAudioDeviceModule
{
public:
     int32_t RegisterAudioCallback(webrtc::AudioTransport* audioCallback) override;
     int32_t StartPlayout() override;
     int32_t StopPlayout() override;
     bool Playing() const override;
     int32_t StartRecording() override;
     int32_t StopRecording() override;
     bool Recording() const override;

     static bool RecThreadFunc(void*);
     static bool PlayThreadFunc(void*);

     bool RecThreadProcess();
     bool PlayThreadProcess();

     void RegisterSubscriber(std::shared_ptr<Subscriber> sub)
     {
         sub_ = sub;
     }
private:
     std::unique_ptr<webrtc::AudioDeviceBuffer> audio_device_buffer_; 
//    webrtc::FineAudioBuffer fine_audio_buffer_;

    std::unique_ptr<rtc::PlatformThread> _ptrThreadRec;
    std::unique_ptr<rtc::PlatformThread> _ptrThreadPlay;
    bool _recording{false};
    bool _playing{false};
    bool _recIsInitialized;
    bool _playIsInitialized;

    std::shared_ptr<Subscriber> sub_;
};
#endif//_DUMMY_AUDIO_DEVICE_H_

