const HttpServer = require('./HttpServer');

let hs = new HttpServer();

let params = {
    hostname:"0.0.0.0",
    port:443,
    certpath:"D:/work/code/spiritbreaker/test/cdts_validCertKey.pem",
    password:'test'
}
hs.start(params);
