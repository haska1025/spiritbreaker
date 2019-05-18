const io = require('socket.io-client');

class TestHttpServer
{
    constructor(){
        
    }

    socketioTest(){

        console.log('Ready to connect');
        let options = {
            secure:true,
            reconnect:true,
            rejectUnauthorized:false
        };
        let sock = io('https://192.168.43.49', options);
        sock.on('connect_error', (error) => {
            console.log('connect error==='+error);
        });

        sock.on('error', (error) => {
            console.log('error === '+ error);
        });

        sock.on('reconnect', () => {
            console.log('reconnect ....');
        });

        sock.on('disconnect', () => {
            console.log('disconnect');
        });

        sock.on('connect', (socket) => {
            console.log('Connected......');
        });

        console.log('Emit test');
        sock.emit('test', '1', '2');
        
    }
}

module.exports = TestHttpServer;