const HttpServer = require('./HttpServer');

let hs = new HttpServer();

let params = {
    hostname:"0.0.0.0",
    port:443,
    key:"d:/work/code/spiritbreaker/test/server.key",
    cert:"d:/work/code/spiritbreaker/test/server.cert",
    type:'https'
}
hs.start(params);
