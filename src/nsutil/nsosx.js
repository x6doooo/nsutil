var fs = require('fs');

var _osx = require('../../build/Release/nsutil_osx.node');
var _common = require('./common.js');
var _posix = require('./nsposix.js');


var _process_class_ = function(pid) {
    this.pid = pid;
};

var __proto = _process_class_.prototype;

__proto.name = function() {
    return _osx.nsutil_proc_name_sync(this.pid);
};
__proto.exe = function() {
    return _osx.nsutil_proc_exe_sync(this.pid);
};
__proto.cmdline = function() {
    return _osx.nsutil_proc_cmdline_sync(this.pid);
};
__proto.ppid = function() {
    return _osx.nsutil_proc_ppid_sync(this.pid);
};
__proto.cwd = function() {
    return _osx.nsutil_proc_cwd_sync(this.pid);
};
__proto.uids = function() {
    var u = _osx.nsutil_proc_uids_sync(this.pid);
    return {
        real: u[0],
        effective: u[1],
        saved: u[2]
    };
};
__proto.gids = function() {
    var g = _osx.nsutil_proc_gids_sync(this.pid);
    return {
        real: g[0],
        effective: g[1],
        saved: g[2]
    };
};
__proto.terminal = function() {
    var tty_nr = _osx.nsutil_proc_tty_nr_sync(this.pid);
    var terminalMap = _posix.getTerminalMap();
    return terminalMap[tty_nr];
};
__proto.memoryInfo = function() {
    return _osx.nsutil_proc_memory_info_sync(this.pid);
};
__proto.cpuTimes = function() {
    return _osx.nsutil_proc_cpu_times_sync(this.pid); 
};
__proto.createTime = function() {
    return _osx.nsutil_proc_create_time_sync(this.pid) * 1000;
};
__proto.numCtxSwitches = function() {
    return _osx.nsutil_proc_num_ctx_switches_sync(this.pid);
};
__proto.numThreads = function() {
    return _osx.nsutil_proc_num_threads_sync(this.pid);
};
__proto.openFiles = function() {
    return _osx.nsutil_proc_open_files_sync(this.pid);
};
__proto.connections = function(family_types) {
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
    return conns;
};
__proto.numFds = function() {
    return _osx.nsutil_proc_num_fds_sync(this.pid);
};
// TODO: __proto.wait

__proto.getNice = function() {
    return _osx.nsutil_posix_getpriority_sync(this.pid);
};
__proto.setNice = function(nice) {
    _osx.nsutil_posix_setpriority_sync(this.pid, nice);
};
__proto.status = function() {
    var st = _osx.nsutil_proc_status_sync(this.pid);
    return _common.PROC_STATUSES[st];
};
__proto.threads = function() {
    return _osx.nsutil_proc_threads_sync(this.pid);
};
__proto.memMaps = function() {
    return _osx.nsutil_proc_memory_maps_sync(this.pid);
};


var Process = function(pid) {
    return new _process_class_(pid);
}


function swapMemory() {
    var s_mem = _osx.nsutil_swap_mem_sync();
    //s_mem.percent = (s_mem.used / s_mem.total, s_mem.total, 2);
    return s_mem;
}

function cpuTimes() {
    var t = _osx.nsutil_cpu_times_sync();
    for (var k in t) {
        t[k] *= 1000;
    }
    return t;
}

function perCpuTimes() {
    var t = _osx.nsutil_per_cpu_times_sync();
    var k;
    t.forEach(function(v, i, a) {
        for (k in v) {
            v[k] *= 1000;
        }
    });
    return t;
}

function cpuCountLogical() {
    return _osx.nsutil_cpu_count_logical_sync();
}

function cpuCountPhys() {
    return _osx.nsutil_cpu_count_phys_sync();
}

function bootTime() {
    return _osx.nsutil_boot_time_sync() * 1000;
}

function diskPartitions() {
    return _osx.nsutil_disk_partitions_sync();
}

function users() {
    return _osx.nsutil_users_sync();
}

function pids() {
    return _osx.nsutil_pids_sync();
}

function netConnections(kind) {
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
    return conns_arr;
}

function netIOCounters() {
    return _osx.nsutil_net_io_counters_sync();
}

function diskIOCounters() {
    return _osx.nsutil_disk_io_counters_sync();
}

function virtualMemory() {
    var v_mem = _osx.nsutil_virtual_mem_sync();
    v_mem.avail = v_mem.inactive + v_mem.free;
    v_mem.used = v_mem.active + v_mem.inactive + v_mem.wire;
    //v_mem.percent = _common.usagePercent(v_mem.total - v_mem.avail, v_mem.total, 2);
    return v_mem;
}



module.exports = {
    Process: Process,
    virtualMemory: virtualMemory,
    swapMemory: swapMemory,
    cpuTimes: cpuTimes,
    perCpuTimes: perCpuTimes,
    cpuCountLogical: cpuCountLogical,
    cpuCountPhys: cpuCountPhys,
    bootTime: bootTime,
    diskPartitions: diskPartitions,
    users: users,
    pids: pids,
    netConnections: netConnections,
    netIOCounters: netIOCounters,
    diskIOCounters: diskIOCounters
};


