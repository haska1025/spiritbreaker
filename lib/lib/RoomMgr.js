const Room = require('./Room');
const Peer = require('./Peer');
const logger = require('./Logger');

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
            logger.error("Probe the roomid=" + req.roomid + " not exist.");
            return -1;
        }
        let peer = room.getPeer(req.peerid);
        if (peer === null){
            logger.error("Probe the peerid=" + req.roomid + " not exist.");
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

        return room.join(req.peerid);        
    }
    leaveRoom(req, res)
    {

    }

    addPublisher(req, res)
    {
        let room = this.getRoom(req.roomid);
        if (room === null){
            // No exist
            logger.error("The roomid=" + req.roomid + " not exist.");
            return -1;
        }
        return room.addPublisher(req, res);
    }
    reomvePublisher(req, res)
    {

    }

    setRemoteSdp(req, res)
    {
        let room = this.getRoom(req.roomid);
        if (room === null){
            logger.error("setRemoteSdp the roomid=" + req.roomid + " not exist.");
            return -1;
        }
        logger.debug('=============== data.type = ' + req.data.type);

        return room.setRemoteSdp(req, res);
    }
};

module.exports = RoomMgr;