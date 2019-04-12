#include "webrtcconnection.h"
#include "room_mgr.h"
#include "sbs_error.h"

#include <pc/webrtc_sdp.h>
#include <api/jsep_session_description.h>

WebRtcConnection::WebRtcConnection()
{
}
WebRtcConnection::~WebRtcConnection()
{
}

int WebRtcConnection::Initialize()
{
    // Init the session id.
    session_id_ = rtc::ToString(rtc::CreateRandomId64() & LLONG_MAX);

    // Create the jsep transport controller
//    webrtc::JsepTransportController::Config config;
 //   transport_controller_.reset(new webrtc::JsepTransportController(
  //              NULL,NULL, NULL, NULL, config));
      return 0;
}

int WebRtcConnection::SetRemoteSdp(const std::string &sdp)
{
    // Save the sdp
    RTC_LOG(LS_INFO) << "The remote sdp=" <<sdp;
    remote_sdp_ = sdp;

    // Deserialize the sdp to SessionDescription
    webrtc::SdpParseError error;
    auto desc = webrtc::CreateSessionDescription(webrtc::SdpType::kOffer, sdp, &error); 
    if (!desc){
        RTC_LOG(LS_ERROR) << "Set remote sdp create session desc by sdp failed. " << error.description;
        return SBS_GENERAL_ERROR;
    }

    remote_desc_ = std::move(desc);

    std::string tmpsdp;
    remote_desc_->ToString(&tmpsdp);

    RTC_LOG(LS_INFO) << "After deserialize===" << tmpsdp;
    return SBS_SUCCESS;
}

int WebRtcConnection::CreateAnswer()
{
    cricket::MediaSessionOptions options;
    if (!remote_desc_){
        RTC_LOG(LS_ERROR) << "Create answer failed. You must call SetRemoteSdp firstly";
        return SBS_GENERAL_ERROR;
    }

    auto desc = RoomMgr::Instance()->media_session_desc_factory()->CreateAnswer(
            remote_desc_->description(),
            options,
            local_desc_?local_desc_->description():nullptr);
    if (!desc){
        RTC_LOG(LS_ERROR) << "Create answer failed";
        return SBS_GENERAL_ERROR;
    }

    auto answer = absl::make_unique<webrtc::JsepSessionDescription>(
    webrtc::SdpType::kAnswer, std::move(desc), session_id_,
    rtc::ToString(session_version_++));

    local_desc_ = std::move(answer);

    local_desc_->ToString(&local_sdp_);

    RTC_LOG(LS_INFO) << "The local sdp :" << local_sdp_;

    return SBS_SUCCESS;
}

