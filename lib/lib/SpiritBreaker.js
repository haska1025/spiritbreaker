const HttpServer = require('./HttpServer');

let hs = new HttpServer();

let params = {
    hostname:"0.0.0.0",
    port:443,
    key:"e:/work/code/spiritbreaker/test/server.key",
    cert:"e:/work/code/spiritbreaker/test/server.cert",
    type:'https'
}
hs.start(params);
