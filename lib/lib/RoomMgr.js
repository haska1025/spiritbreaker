class RoomMgr
{
    constructor(){
        this.rooms_ = new Map();
    }
    Initialize(){

    }

    AddRoom(r)
    {
        this.rooms_.set(r.id, r);
    }
    RemoveRoom(id)
    {
        this.rooms_.delete(id);
    }
    GetRoom(id)
    {
        return this.rooms_.get(id);
    }

    joinRoom(req, res)
    {

    }
    leaveRoom(req, res)
    {

    }

    AddPublisher(req, res)
    {

    }
    ReomvePublisher(req, res)
    {

    }

    SetRemoteSdp(req, res)
    {

    }
};

module.exports = RoomMgr;