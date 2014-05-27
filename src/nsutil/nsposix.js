// nsposix.js
var fs = require('fs');
var os = require('os');
var _arch = os.arch();
if (os.platform() == 'darwin') {
    var _osx = require('../../build/Release/' + _arch + '/nsutil_osx.node');
    var _posix = require('../../build/Release/' + _arch + '/nsutil_posix_osx.node');
    function pidExists(pid) {
        return _osx.nsutil_pid_exists_sync(pid);
    }
} else {
    var pidExists = null;
    var _posix = require('../../build/Release/' + _arch + '/nsutil_posix_linux.node');
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

function getPriority(pid) {
    return _posix.nsutil_posix_getpriority_sync(pid);
}

function setPriority(pid, prio) {
    return _posix.nsutil_posix_setpriority_sync(pid, prio);
}

module.exports = {
    pidExists: pidExists,
    getTerminalMap: getTerminalMap,
    getPriority: getPriority,
    setPriority: setPriority
};
