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

function __getTerminalMap_Handle(f_arr, terminalMap, path, isP1) {
    f_arr.forEach(function(v) {
        var p;
        if (isP1) {
            if (v.indexOf('tty') >= 0) {
                p = path + v;
                terminalMap[fs.statSync(p).rdev] = p;
            }
        } else {
            p = path + v;
            terminalMap[fs.statSync(p).rdev] = p;
        }
    });
}

function __getTerminalMap_Handle_async(data, terminalMap, path, isP1 , count, cb) {
    __getTerminalMap_Handle(data, terminalMap, path, isP1);
    if (count == 0) {
        cb(null, terminalMap);
    }
}

function getTerminalMap(cb) {
    var terminalMap = {};
    var f_arr;
    var path;

    var p1 = '/dev/';
    var p2 = '/dev/pts/';

    if (cb && typeof cb == 'function') {
        var count = 2;
        fs.readdir(p1, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            __getTerminalMap_Handle_async(data, terminalMap, p1, true, --count, cb);
        });
        fs.readdir(p2, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            __getTerminalMap_Handle_async(data, terminalMap, p2, false, --count, cb);
        });
        return;
    }

    if (fs.existsSync(p1)) {
        f_arr = fs.readdirSync(p1);
        __getTerminalMap_Handle(f_arr, terminalMap, p1, true);
    }
    if (fs.existsSync(p2)) {
        f_arr = fs.readdirSync(p2);
        __getTerminalMap_Handle(f_arr, terminalMap, p2);
    }
    return terminalMap;
}

function getPriority(pid) {
    return _posix.nsutil_posix_getpriority_sync(pid);
}

function setPriority(pid, prio) {
    _posix.nsutil_posix_setpriority_sync(pid, prio);
    return 0;
}

function statvfs(path) {
    return _posix.nsutil_posix_statvfs(path);
}


module.exports = {
    pidExists: pidExists,
    getTerminalMap: getTerminalMap,
    getPriority: getPriority,
    setPriority: setPriority,
    statvfs: statvfs
};
