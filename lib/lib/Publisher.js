class Publisher
{
    constructor(id){
        this.id_ = id;
        this.subscribers_ = new Map();
        this. webrtc_conn_ = null;
    }

    AddSubsciber(s){
        this.subscribers_.set(s.peerid, s);
    }
    RemoveSubsciber(peerid){
        this.subscribers_.delete(peerid);
    }
    GetSubsciber(peerid){
        this.subscribers_.get(peerid);
    }

    get id(){
        return this.id_;
    }
}

module.exports = Publisher;