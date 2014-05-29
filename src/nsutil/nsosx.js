var fs = require('fs');
var os = require('os');

var _arch = os.arch();
var _osx = require('../../build/Release/' + _arch + '/nsutil_osx.node');
var _common = require('./common.js');
var _posix = require('./nsposix.js');

// OSX的大部分接口都是系统函数调用，异步意义不大
// 为了和linux接口统一格式，设置伪异步
function handleResult(result, cb, ctx) {
    if (cb && typeof cb == 'function') {
        cb.call(ctx, null, result);
        return;
    }
    return result;
}

var _process_class_ = function(pid) {
    this.pid = pid;
};

var __proto = _process_class_.prototype;

__proto.name = function(cb) {
    var r = _osx.nsutil_proc_name_sync(this.pid);
    return handleResult(r, cb, this);
};
__proto.exe = function(cb) {
    var r = _osx.nsutil_proc_exe_sync(this.pid);
    return handleResult(r, cb, this);
};
__proto.cmdline = function(cb) {
    var r = _osx.nsutil_proc_cmdline_sync(this.pid);
    return handleResult(r, cb, this);
};
__proto.ppid = function(cb) {
    var r = _osx.nsutil_proc_ppid_sync(this.pid);
    return handleResult(r, cb, this);
};
__proto.cwd = function(cb) {
    var r = _osx.nsutil_proc_cwd_sync(this.pid);
    return handleResult(r, cb, this);
};
__proto.uids = function(cb) {
    var u = _osx.nsutil_proc_uids_sync(this.pid);
    var r = {
        real: u[0],
        effective: u[1],
        saved: u[2]
    };
    return handleResult(r, cb, this);
};
__proto.gids = function(cb) {
    var g = _osx.nsutil_proc_gids_sync(this.pid);
    var r = {
        real: g[0],
        effective: g[1],
        saved: g[2]
    };
    return handleResult(r, cb, this);
};
__proto.terminal = function(cb) {
    var tty_nr = _osx.nsutil_proc_tty_nr_sync(this.pid);
    var terminalMap = _posix.getTerminalMap();
    var r = terminalMap[tty_nr];
    return handleResult(r, cb, this);
};
__proto.memoryInfo = function(cb) {
    var r = _osx.nsutil_proc_memory_info_sync(this.pid);
    return handleResult(r, cb, this);
};
__proto.cpuTimes = function(cb) {
    var r = _osx.nsutil_proc_cpu_times_sync(this.pid); 
    return handleResult(r, cb, this);
};
__proto.createTime = function(cb) {
    var r = _osx.nsutil_proc_create_time_sync(this.pid) * 1000;
    return handleResult(r, cb, this);
};
__proto.numCtxSwitches = function(cb) {
    var r = {
        voluntary: _osx.nsutil_proc_num_ctx_switches_sync(this.pid),
        involuntary: 0
    };
    return handleResult(r, cb, this);
};
__proto.numThreads = function(cb) {
    var r = _osx.nsutil_proc_num_threads_sync(this.pid);
    return handleResult(r, cb, this);
};
__proto.openFiles = function(cb) {
    var r = _osx.nsutil_proc_open_files_sync(this.pid);
    return handleResult(r, cb, this);
};
__proto.connections = function(/*family_types, cb*/) {
    var family_types;
    var cb;
    if (arguments.length == 1) { 
        if (typeof arguments[0] == 'string') {
            family_types = arguments[0];
        } else if (typeof arguments[0] == 'function') {
            cb = arguments[0];
        }
    }

    if (arguments.length == 2) {
        family_types = arguments[0];
        cb = arguments[1];
    }

    family_types = family_types || 'all';
    var ft = _common.CONN_FAMILY_AND_TYPE[family_types];
    var family = ft[0];
    var types  = ft[1];
    var conns = _osx.nsutil_proc_connections_sync(this.pid, family, types);
    if (conns) {
        conns.forEach(function(v, i, a) {
            v.family = _common.CONN_FAMILY_STR[v.family];
            v.type = _common.CONN_TYPE_STR[v.type];
            v.state = _common.TCP_STATUSES[v.state] || v.state;
        });
    }
    return handleResult(conns, cb, this);
};
__proto.numFds = function(cb) {
    var r = _osx.nsutil_proc_num_fds_sync(this.pid);
    return handleResult(r, cb, this);
};
// TODO: __proto.wait

__proto.getNice = function(cb) {
    var r = _posix.getPriority(this.pid);
    return handleResult(r, cb, this);
};
__proto.setNice = function(nice, cb) {
    var r = _posix.setPriority(this.pid, nice);
    return handleResult(r, cb, this);
};
__proto.status = function(cb) {
    var st = _osx.nsutil_proc_status_sync(this.pid);
    var r = _common.PROC_STATUSES[st];
    return handleResult(r, cb, this);
};
__proto.threads = function(cb) {
    var r = _osx.nsutil_proc_threads_sync(this.pid);
    return handleResult(r, cb, this);
};
__proto.memMaps = function(cb) {
    var r = _osx.nsutil_proc_memory_maps_sync(this.pid);
    return handleResult(r, cb, this);
};


var Process = function(pid) {
    return new _process_class_(pid);
};


function swapMemory(cb) {
    if (cb && typeof cb == 'function') {
        _osx.nsutil_swap_mem_async(cb);
        return;
    }
    return _osx.nsutil_swap_mem_sync();
}


function cpuTimes(cb) {
    if (cb && typeof cb == 'function') {
        _osx.nsutil_cpu_times_async(cb);
        return;
    }
    return _osx.nsutil_cpu_times_sync();
}

function perCpuTimes(cb) {
    if (cb && typeof cb == 'function') {
        _osx.nsutil_per_cpu_times_async(cb);
        return;
    }
    return _osx.nsutil_per_cpu_times_sync();
}


function cpuCountLogical(cb) {
    var r = _osx.nsutil_cpu_count_logical_sync();
    return handleResult(r, cb, this);
}

function cpuCountPhys(cb) {
    var r = _osx.nsutil_cpu_count_phys_sync();
    return handleResult(r, cb, this);
}

function bootTime(cb) {
    var r = _osx.nsutil_boot_time_sync() * 1000;
    return handleResult(r, cb, this);
}

function diskPartitions(cb) {
    var r = _osx.nsutil_disk_partitions_sync();
    return handleResult(r, cb, this);
}

function users(cb) {
    var r = _osx.nsutil_users_sync();
    return handleResult(r, cb, this);
}

function pids(cb) {
    var r = _osx.nsutil_pids_sync();
    return handleResult(r, cb, this);
}

function netConnections(/*kind, cb*/) {
    
    var kind;
    var cb;
    if (arguments.length == 1) { 
        if (typeof arguments[0] == 'string') {
            kind = arguments[0];
        } else if (typeof arguments[0] == 'function') {
            cb = arguments[0];
        }
    }

    if (arguments.length == 2) {
        kind = arguments[0];
        cb = arguments[1];
    }

    kind = kind || "all";
    //var k = _common.CONN_FAMILY_AND_TYPE[kind];
    //var f = k[0];
    //var t = k[1];
    var conns;
    var conns_arr = [];
    var pids_arr = pids();
    var i, len;
    pids_arr.forEach(function(v, i, a) {
        conns = Process(v).connections(kind);
        if (conns) {
            for (i = 0, len = conns.length; i < len; i++) {
                conns[i].pid = v;
                conns_arr.push(conns);
            }
        }
    });

    return handleResult(conns_arr, cb, this);
}

function netIOCounters(cb) {
    var r = _osx.nsutil_net_io_counters_sync();
    return handleResult(r, cb, this);
}

function diskIOCounters(cb) {
    var r = _osx.nsutil_disk_io_counters_sync();
    return handleResult(r, cb, this);
}

function virtualMemory(cb) {
    var v_mem = _osx.nsutil_virtual_mem_sync();
    v_mem.avail = v_mem.inactive + v_mem.free;
    v_mem.used = v_mem.active + v_mem.inactive + v_mem.wire;
    //v_mem.percent = _common.usagePercent(v_mem.total - v_mem.avail, v_mem.total, 2);
    return handleResult(v_mem, cb, this);
}



module.exports = {
    Process: Process,
    virtualMemory: virtualMemory,
    swapMemory: swapMemory,
    cpuTimes: cpuTimes,
    perCpuTimes: perCpuTimes,
    cpuCountLogical: cpuCountLogical,
    cpuCountPhysical: cpuCountPhys,
    bootTime: bootTime,
    diskPartitions: diskPartitions,
    users: users,
    pids: pids,
    netConnections: netConnections,
    netIOCounters: netIOCounters,
    diskIOCounters: diskIOCounters
};


