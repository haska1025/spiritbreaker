const Room = require('./Room');
const Peer = require('./Peer');

class RoomMgr
{
    constructor(){
        this.rooms_ = new Map();
    }
    Initialize(){

    }

    addRoom(r)
    {
        this.rooms_.set(r.id, r);
    }
    removeRoom(id)
    {
        this.rooms_.delete(id);
    }
    getRoom(id)
    {
        if (this.rooms_.has(id)){
            return this.rooms_.get(id);
        }
        return null;
    }

    probe(req, res)
    {
        let room = this.getRoom(req.roomid);
        if (room === null){
            return -1;
        }
        let peer = room.getPeer(req.peerid);
        if (peer === null){
            return -2;
        }
        return 0;
    }
    joinRoom(req, res)
    {
        let room = this.getRoom(req.roomid);
        if (room === null){
            room = new Room(req.roomid);
            this.addRoom(room);
        }

        let peer = room.getPeer(req.peerid);
        if (peer === null){
            peer = new Peer(req.peerid);
            room.addPeer(peer);
        }
        return 0;
    }
    leaveRoom(req, res)
    {

    }

    addPublisher(req, res)
    {
        let room = this.getRoom(req.roomid);
        if (room === null){
            // No exist
            return -1;
        }
        return room.addPublisher(req, res);
    }
    reomvePublisher(req, res)
    {

    }

    setRemoteSdp(req, res)
    {

    }
};

module.exports = RoomMgr;