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
        return this.publishers_.get(id);
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
        return this.subscribers_.get(pubid);
    }
}

module.exports = Peer;