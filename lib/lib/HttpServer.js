const express = require('express');
const fs = require('fs');
const RoomMgr = require('./RoomMgr');
const logger = require('./Logger');

class HttpServer {
    constructor() {
        this.app_ = express();
        this.room_mgr_ = new RoomMgr();
        this.io_ = null;
    }

    start(params) {
        let server = null;
        if (params.type === 'https') {
            logger.info('Create https server...');
            server = require('https').createServer({
                key: fs.readFileSync(params.key),
                cert: fs.readFileSync(params.cert),
                requestCert: false,
                rejectUnauthorized: false
            }, this.app_);
        }

        if (params.type === 'http') {
            logger.info('Create http server...');
            server = require('http').createServer(this.app_);
        }

        server.listen(params.port, params.hostname, () => {
            logger.info('Start server hostname=' + params.hostname + ' port=' + params.port + ' ....');
        });

        this.io_ = require('socket.io').listen(server);

        this._registerSignalHandler();
        this._registerRestfulHandler();
    }

    _registerSignalHandler() {
        this.io_.sockets.on('connection', (socket) => {
            logger.debug('Some client connect....');
            socket.on('test_req', (msg) => {
                logger.debug('test msg===' + msg);
                socket.emit('test_res', 'Response from spiritbreaker.');
            });

            socket.on('probe_req', (req) => {
                logger.debug('probe msg===' + JSON.stringify(req));
                let res = {};
                let rc = this.room_mgr_.probe(req, res);
                //res.result = rc;
                res.roomid = req.roomid;
                res.peerid = req.peerid;
                socket.emit('prob_res', res);
                logger.debug('probe msg response===' + JSON.stringify(res));
            });

            socket.on('pub_req', (req) => {
                let res = {};
                let rc = this.room_mgr_.addPublisher(req, res);
                res.result = rc;
                res.roomid = req.roomid;
                res.peerid = req.peerid;
                socket.emit('pub_res', res);
            });
        });
    }

    _registerRestfulHandler() {
        this.app_.get('/test/:p1/:p2', (req, res) => {
            logger.debug('p1=' + req.params.p1 + ' p2=' + req.params.p2);
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
