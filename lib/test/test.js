const TestHttpServer = require('./test_httpserver');
const TestSBS = require('./test_spiritbreaker');

let sbs = new TestSBS();
sbs.test_sbs();

let ths = new TestHttpServer();

ths.socketioTest();


