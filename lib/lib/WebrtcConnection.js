class WebrtcConnection
{
    constructor(){
        this.webrtc_ = null;
    }

    get webrtc(){
        return this.webrtc_;
    }
}

module.exports = WebrtcConnection;