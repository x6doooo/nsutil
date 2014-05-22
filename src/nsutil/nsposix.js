// nsposix.js
var fs = require('fs');
var os = require('os');
if (os.platform == 'darwin') {
    var _osx = require('../../build/Release/nsutil_osx.node');
    function pidExists(pid) {
        return _osx.nsutil_pid_exists_sync(pid);
    }
} else {
    var pidExists = null;
}

function getTerminalMap() {
    var terminalMap = {};
    var f_arr;
    var path;
    if (fs.existsSync('/dev/')) {
        f_arr = fs.readdirSync('/dev/');
        f_arr.forEach(function(v) {
            if (v.indexOf('tty') >= 0) {
                path = '/dev/' + v;
                terminalMap[fs.statSync(path).rdev] = path;
            }
        });

    }
    if (fs.existsSync('/dev/pts/')) {
        f_arr = fs.readdirSync('/dev/pts/'); 
        f_arr.forEach(function(v) {
            path = '/dev/pts/' + v;
            terminalMap[fs.statSync(path).rdev] = path; 
        });
    }
    return terminalMap;
}

module.exports = {
    pidExists: pidExists,
    getTerminalMap: getTerminalMap
};
