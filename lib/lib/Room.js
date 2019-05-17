class Room
{
    constructor(id){
        this.id_ = id;
        this.peers_ = new Map();
    }
  
    AddPeer(peer){
        this.peers_.set(peer.id, peer);
    }
    GetPeer(id){
        return this.peers_.get(id);
    }
    RemovePeer(id){
        return this.peers_.delete(id);
    }   
};