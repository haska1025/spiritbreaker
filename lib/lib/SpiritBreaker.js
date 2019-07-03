const HttpServer = require('./HttpServer');
const sbs = require('../build/Release/sbs');
//const sbs = require('../build/Debug/sbs');
const logger = require('./Logger');

let rc = sbs.initialize();
if (rc !== 0){
    logger.error('Init the sbs manager failed. rc=' + rc);
    return;
}

let hs = new HttpServer();

let params = {
    hostname:"0.0.0.0",
    port:443,
//    key:"d:/work/code/spiritbreaker/test/server.key",
//    cert:"d:/work/code/spiritbreaker/test/server.cert",
    key:"../test/server.key",
    cert:"../test/server.cert",
    type:'https'
}
hs.start(params);
