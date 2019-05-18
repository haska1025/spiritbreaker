const express = require('express');
const RoomMgr = require('./RoomMgr');
const fs = require( 'fs' );

class HttpServer
{
    constructor(){
        this.app_ = express();
        this.room_mgr_ = new RoomMgr();
    }

    start(params){
        let server = null;
        if (params.type === 'https'){
            console.log('Create https server...');
            server = require('https').createServer({
                key: fs.readFileSync(params.key),
                cert: fs.readFileSync(params.cert),
                requestCert:false,
                rejectUnauthorized: false
            }, this.app_);
        }

        if (params.type === 'http'){
            console.log('Create http server...');
            server = require('http').createServer(this.app_);
        }        

        server.listen(params.port, params.hostname, () => {
            console.log('Start server hostname='+params.hostname + ' port=' + params.port +' ....');
        });
        
        var io = require('socket.io').listen(server);
        
        io.sockets.on('connection', (socket) => {
            console.log('Some client connect....');
            socket.on('test', (msg) => {
                console.log('test msg===' + msg);
            });
        });

        
        this._registerHandler();
    }

    _registerHandler(){
        this.app_.get('/test/:p1/:p2', (req, res) => {
            console.log('p1=' + req.params.p1 + ' p2=' + req.params.p2);
            let res_msg = {};
            res_msg.hello = "From SpiritBreaker.";
            res_msg.p1 = req.params.p1;
            res_msg.p2 = req.params.p2;
            res.json(res_msg);
        })
        this.app_.post('/room/peer/join/:roomid/:peerid', (req, res) => {
            this.room_mgr_.joinRoom(req, res);
        });

        this.app_.post('/room/peer/leave/:roomid/:peerid', (req, res) => {
            this.room_mgr_.leaveRoom(req, res);
        });
    }
}

module.exports = HttpServer;
