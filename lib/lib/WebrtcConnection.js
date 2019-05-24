//const sbs = require('../../build/Debug/sbs');
const sbs = require('../build/Release/sbs');
const logger = require('./Logger');

class WebrtcConnection
{
    constructor(){
        this.webrtc_ = new sbs.WebRtcConnectionWrap;
        let rc = this.webrtc_.initialize();
        if (rc != 0){
            logger.error('Init the webrtc native failed. rc=' + rc);
            throw 'Init the webrtc native failed. rc=' + rc;
        }
        logger.debug('Init the webrtc success...');
    }

    get webrtc(){
        return this.webrtc_;
    }

    close(){
    }
}

module.exports = WebrtcConnection;
