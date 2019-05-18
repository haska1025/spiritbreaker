const Publisher = require('./Publisher');

class Room
{
    constructor(id){
        this.id_ = id;
        this.peers_ = new Map();
        this.pubid_base_ = 0x1000;
    }
  
    addPeer(peer){
        this.peers_.set(peer.id, peer);
    }
    getPeer(id){
        return this.peers_.get(id);
    }
    removePeer(id){
        return this.peers_.delete(id);
    }
    
    addPublisher(req, res){
        let peer = this.getPeer(req.peerid);
        if (peer === null){
            // Not exist.
            return -2;
        }
        let pubid = this.pubid_base_++;
        let pub = peer.getPublisher(pubid);
        if (pub !== null){
            // Exist already
            return -3;
        }

        pub = new Publisher(pubid);
        peer.addPublisher(pub);

        // Set response.
        res.pubid = pubid;

        return 0;
    }
};

module.exports = Room;
