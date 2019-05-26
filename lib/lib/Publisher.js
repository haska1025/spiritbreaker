const WebrtcConnection = require('./WebrtcConnection');
const logger = require('./Logger');
const EventEmitter = require('events').EventEmitter;

class Publisher extends EventEmitter
{
    constructor(id){
        super();

        this.id_ = id;
        this.subscribers_ = new Map();
        this.webrtc_conn_ = new WebrtcConnection();
        this.local_sdp_ = null;
        this.remote_sdp_ = null;


        this.webrtc_conn_.on('@localsdp', ()=>{
            this.local_sdp_ = this.webrtc_conn_.webrtc.getLocalSdp();
            logger.debug('Get local sdp==========' + this.local_sdp_);            

            let data={
                type:'answer',
                sdp:this.local_sdp_,
                pubid:this.id_
            };

            this.emit('@pub_send', 'pub_answer', data);
        });


        this.webrtc_conn_.on('@candidate', (message)=>{
            logger.debug('The spiritbreaker candidate ====' + message);            
            
            let data = JSON.parse(message);
            data.pubid = this.id;

            this.emit('@pub_send', 'pub_candidate', data);
        });

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

