const WebrtcConnection = require('./WebrtcConnection');

class Publisher
{
    constructor(id){
        this.id_ = id;
        this.subscribers_ = new Map();
        this.webrtc_conn_ = new WebrtcConnection();
        this.local_sdp_ = null;
        this.remote_sdp_ = null;
    }

    addSubsciber(s){
        this.subscribers_.set(s.peerid, s);
    }
    removeSubsciber(peerid){
        this.subscribers_.delete(peerid);
    }
    getSubsciber(peerid){
        this.subscribers_.get(peerid);
    }

    get id(){
        return this.id_;
    }

    setRemoteSdp(sdp){
        this.remote_sdp_ = sdp;

        return this.webrtc_conn_.webrtc.setRemoteSdp(sdp);
    }
}

module.exports = Publisher;
