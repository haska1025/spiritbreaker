const sbs = require('../build/Debug/sbs');
//const sbs = require('../build/Release/sbs');
const logger = require('./Logger');
const EventEmitter = require('events').EventEmitter;

class WebrtcConnection extends EventEmitter
{
    constructor(){
        super();

        this.webrtc_ = new sbs.WebRtcConnectionWrap(this);
        let rc = this.webrtc_.initialize();
        if (rc != 0){
            logger.error('Init the webrtc native failed. rc=' + rc);
            throw 'Init the webrtc native failed. rc=' + rc;
        }
        logger.debug('Init the webrtc success...');

        this.onLocalSdp = () => {
            this.emit('@localsdp');
        }

        this.onCandidate = (candidate) => {
            this.emit('@candidate', candidate);
        }
    }

    close(){
        this.webrtc_.close();
    }


    get webrtc(){
        return this.webrtc_;
    }
    
}

module.exports = WebrtcConnection;
