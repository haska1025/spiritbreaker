const io = require('socket.io-client');
const logger = require('../lib/Logger');

class TestHttpServer
{
    constructor(){
        
    }

    socketioTest(){
        logger.debug('Ready to connect');
        let options = {
            secure:true,
            reconnect:true,
            rejectUnauthorized:false
        };
        let sock = io('http://192.168.43.49', options);
        sock.on('connect_error', (error) => {
            logger.debug('connect error==='+error);
        });

        sock.on('error', (error) => {
            logger.debug('error === '+ error);
        });

        sock.on('reconnect', () => {
            logger.debug('reconnect ....');
        });

        sock.on('disconnect', () => {
            logger.debug('disconnect');
        });

        // The user-defined events.
        sock.on('test_res', (msg) => {
            logger.debug(msg);
        });

        sock.on('probe_res', (msg) => {
            logger.debug(JSON.stringify(msg));
        });

        sock.on('connect', () => {
            logger.debug('Connected......');
            sock.emit('test_req', 'Hello i am spiritbreaker client.');
            let req = {};
            req.roomid = 100;
            req.peerid =10;
            sock.emit('probe_req', req);
        });

        logger.debug('Emit test');
    }
}

module.exports = TestHttpServer;