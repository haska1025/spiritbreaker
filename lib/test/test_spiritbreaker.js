const sbs = require('../build/Release/sbs');
const logger = require('../lib/Logger');

class TestSBS
{
    test_sbs(){
        let rc = sbs.SBSMgr.Initialize();
        if (rc !== 0){
            logger.error('Init the sbs manager failed. rc=' + rc);
            return;
        }

        let webrtc_conn = sbs.WebRtcConnectionInterface.Create();
    }
}

module.exports = TestSBS;

