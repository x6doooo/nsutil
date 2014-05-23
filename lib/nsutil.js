var platform = require('os').platform();

var nsutil;

switch(platform) {
    case 'linux':
        nsutil = require('../src/nsutil/nslinux.js');
        break;
    case 'darwin':
        nsutil = require('../src/nsutil/nsosx.js');
        break;
    default:
        throw "nsutil don't support your platform";
        break;
}

module.exports = nsutil;

