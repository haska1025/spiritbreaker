
class WebrtcConnection
{
    constructor(){
        this.webrtc_ = sbs.WebRtcConnectionInterface.Create();
    }

    get webrtc(){
        return this.webrtc_;
    }

    close(){
        sbs.WebRtcConnectionInterface.Delete(this.webrtc_);
    }
}

module.exports = WebrtcConnection;