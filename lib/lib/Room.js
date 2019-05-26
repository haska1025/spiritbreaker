const Publisher = require('./Publisher');
const logger = require('./Logger');
const Peer = require('./Peer');

class Room
{
    constructor(id){
        this.id_ = id;
        this.peers_ = new Map();
        this.pubid_base_ = 0x1000;
    }

    get id(){
        return this.id_;
    }
  
    addPeer(peer){
        this.peers_.set(peer.id, peer);
    }
    getPeer(id){
        if (this.peers_.has(id)){
            return this.peers_.get(id);
        }

        return null;
    }
    removePeer(id){
        return this.peers_.delete(id);
    }

    join(peerid){
        let peer = this.getPeer(peerid);
        if (peer === null){
            peer = new Peer(this.id_, peerid);
            this.addPeer(peer);
        }
        return 0;
    }
    
    addPublisher(req, res){
        let peer = this.getPeer(req.peerid);
        if (peer === null){
            // Not exist.
            logger.error("Add publisher the peerid=" + req.peerid + " not exist.");
            return -2;
        }
        let pubid = this.pubid_base_++;
        let pub = peer.getPublisher(pubid);
        if (pub !== null){
            // Exist already
            logger.error("Add publisher the pubid=" + pubid + "has exist.");
            return -5;
        }

        pub = new Publisher(pubid);
        peer.addPublisher(pub);

        // Set response.
        res.pubid = pubid;
     
        return 0;
    }
    setRemoteSdp(req)
    {
        let peer = this.getPeer(req.peerid);
        if (peer === null){
            // Not exist.
            logger.error("Add publisher the peerid=" + req.peerid + " not exist.");
            return -2;
        }
        return peer.setRemoteSdp(req);
    }
};

module.exports = Room;
