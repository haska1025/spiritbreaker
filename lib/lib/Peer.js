const logger = require('./Logger');

class Peer
{
    constructor(id){
        this.id_ = id;
        this.publishers_ = new Map();
        this.subscribers_ = new Map();
    }

    get id(){
        return this.id_;
    }

    addPublisher(pub)
    {        
        this.publishers_.set(pub.id, pub);
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

    setRemoteSdp(req, res)
    {
        let pub = this.getPublisher(req.data.pubid);
        if (pub === null){
            logger.error('[Peer] Set remote sdp. the pubid=' + req.data.pubid + ' not exist.');
            return -3;
        }
        pub.setRemoteSdp(req.data);
    }
}

module.exports = Peer;