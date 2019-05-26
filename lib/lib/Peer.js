const logger = require('./Logger');

class Peer
{
    constructor(roomid, id){
        this.id_ = id;
        this.publishers_ = new Map();
        this.subscribers_ = new Map();
        this.socket_ = null;
        this.roomid_ = roomid;
    }
    
    get id(){
        return this.id_;
    }
    get roomid(){
        return this.roomid_;
    }

    saveSocket(s){
        this.socket_ = s;
    }

    addPublisher(pub)
    {        
        this.publishers_.set(pub.id, pub);

        pub.on('@pub_send', (rescmd, data) => {
           let res = {};
           res.result = 0;
           res.roomid = this.roomid;
           res.peerid = this.id;
           res.data = data;
           this.socket_.emit(rescmd, res);

        });
    }
    removePublisher(id)
    {
        this.publishers_.delete(id);
    }
    getPublisher(id)
    {
        if (this.publishers_.has(id)){
            return this.publishers_.get(id);
        }

        return null;
    }

    addSubsciber(sub)
    {
        this.subscribers_.set(sub.pubid)
    }
    removeSubsciber(pubid)
    {
        this.subscribers_.delete(pubid);
    }
    getSubsciber(pubid)
    {
        if (this.subscribers_.has(pubid)){
            return this.subscribers_.get(pubid);
        }

        return null;
    }

    setRemoteSdp(req)
    {
        let pub = this.getPublisher(req.data.pubid);
        if (pub === null){
            logger.error('[Peer] Set remote sdp. the pubid=' + req.data.pubid + ' not exist.');
            return -3;
        }

        try{
            return pub.setRemoteSdp(req.data.sdp);
        }catch(error){
            logger.error('Set remote failed. error=' + error);
            return -1;
        }
    }
}

module.exports = Peer;
