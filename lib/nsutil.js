var platform = require('os').platform();

var nsutil;

switch(platform) {
    case 'linux':
        break;
    case 'darwin':
        nsutil = require('../src/nsutil/nsosx.js');
        break;
    default:
        throw "nsutil don't support your platform";
        break;
}

module.exports = nsutil;

