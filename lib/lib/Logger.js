const log4js = require('log4js');

console.log('load log');

log4js.configure(__dirname + '/../log4js.json');

const logger = log4js.getLogger('Spiritbreaker');

module.exports = logger;

console.log('load log over');
