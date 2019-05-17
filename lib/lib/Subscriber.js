class Subscriber{
    constructor(pubid, peerid){
        this.pubid_ = pubid;
        this.peerid_ = peerid;
    }

    get pubid(){
        return this.pubid_;
    }

    get peerid(){
        return this.peerid_;
    }
}