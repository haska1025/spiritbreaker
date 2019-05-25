const sbs = require('../build/Debug/sbs');
//const sbs = require('../build/Release/sbs');
const logger = require('./Logger');

class WebrtcConnection
{
    constructor(){
        this.webrtc_ = new sbs.WebRtcConnectionWrap(this);
        let rc = this.webrtc_.initialize();
        if (rc != 0){
            logger.error('Init the webrtc native failed. rc=' + rc);
            throw 'Init the webrtc native failed. rc=' + rc;
        }
        logger.debug('Init the webrtc success...');

        this.onLocalSdp = () => {
            let sdp = this.webrtc_.getLocalSdp();
            logger.debug('The js local sdp=' + sdp);
        }

        this.onCandidate = () => {
            logger.debug('The js on candidate');
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
