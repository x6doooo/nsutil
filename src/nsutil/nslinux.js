var fs = require('fs');
var _common = require('./common.js');
var _posix = require('./nsposix.js');
var CONN = _common.CONN;

var os = require('os');
var _arch = os.arch();
var _linux = require('../../build/Release/' + _arch + '/nsutil_linux.node'); 

var CLOCK_TICKS = _linux.nsutil_sysconf('SC_CLK_TCK');
var PAGE_SIZE = _linux.nsutil_sysconf('SC_PAGE_SIZE');
var BOOT_TIME = null;

var encoding = 'utf8';

// 部分接口的异步和同步调用处理方式相同
// 此处统一处理
function _async_and_sync_get_handle_(file, method, contentHandle, cb) {
    if (cb && typeof cb == 'function') {
        // readFile、readdir、readlink的参数不同，区分处理
        var args = [file];
        if (method == 'readFile') {
            args.push(encoding);
        }
        args.push(function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = contentHandle(data);
            cb(null, r);
        });
        fs[method].apply(null, args);
        return;
    }
    // TODO: 异常处理
    var r = fs[method + 'Sync'](file, encoding);
    return contentHandle(r);
}

function __getCputimesFields_handle(f) {
    var values = f.split(/\n(?!$)/);
    var fields = ['user', 'nice', 'system', 'idle', 'iowait', 'irq', 'softirq'];
    var vlen = values.length;
    if (vlen >= 8) {
        //Linux >= 2.6.11
        fields.push('steal');
    }
    if (vlen >= 9) {
        //Linux >= 2.6.24
        fields.push('guest');
    }
    if (vlen >= 10) {
        //Linux >= 3.2.0
        fields.push('guest_nice');
    }
    return fields;
}

function getCputimesFields(cb) {
    var file = '/proc/stat';
    return _async_and_sync_get_handle_(file, 'readFile', __getCputimesFields_handle, cb);
    /*
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            var fields = __getCputimesFields_handle(data);
            cb(fields);
        });
        return;
    }
    var f = fs.readFileSync(file, encoding);
    return __getCputimesFields_handle(f);
    */
}

var cpuTimesFields = getCputimesFields();

function __virtualMemory_handle(data) {
    var sysinfo = _linux.nsutil_sysinfo();
    var lines = data.split(/\n(?!$)/);
    var cached;
    var active;
    var inactive;
    lines.forEach(function(ln) {
        if (ln.indexOf('Cached') == 0) {
            cached = ln.split(/\s+/)[1] * 1024;
        }
        if (ln.indexOf('Active') >= 0) {
            active = ln.split(/\s+/)[1] * 1024;
        }
        if (ln.indexOf('Inactive') >= 0) {
            inactive = ln.split(/\s+/)[1] * 1024;
        }
    });
    var avail = sysinfo.free + sysinfo.buffer + cached;
    var used = sysinfo.total - sysinfo.free;
    //var percent = _common.usagePercent(sysinfo.total - avail, sysinfo.total, 2); 
    return {
        total: sysinfo.total,
        avail: avail,
        //percent: percent,
        used: used,
        free: sysinfo.free,
        active: active,
        inactive: inactive,
        buffers: sysinfo.buffer,   // phymem buffers
        cached: cached //phymem cached
    };
}

/**
 *  virtualMemory
 */
function virtualMemory(cb) {

    var file = '/proc/meminfo';
    return _async_and_sync_get_handle_(file, 'readFile', __virtualMemory_handle, cb);

}

function __swapMemory_handle(f) {
    var sysinfo = _linux.nsutil_sysinfo();
    var used = sysinfo.total - sysinfo.free;
    var lines = f.split(/\n(?!$)/);
    var pswpin;
    var pswpout;
    lines.forEach(function(ln) {
        if (ln.indexOf('pswpin')) {
            pswpin = ln.split(/\s+/)[1] * 4 * 1024;
        }
        if (ln.indexOf('pswpout')) {
            pswpout = ln.split(/\s+/)[1] * 4 * 1024;
        }
    });
    return {
        total: sysinfo.total,
        used: used,
        free: sysinfo.free,
        sin: pswpin,
        sout: pswpout
    };
}

function swapMemory(cb) {
    var file = '/proc/vmstat';
    //var percent = _common.usagePercent(used, sysinfo.total, 2);
    return _async_and_sync_get_handle_(file, 'readFile', __swapMemory_handle, cb);
   /* 
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb (err, null);
                return;
            }
            var r = __swapMemory_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(file, encoding);
    var r = __swapMemory_handle(f);
    return r;
    */
}

function __cpuTimes_handle(f) {
    var the_line = f.split(/\n/)[0].split(/\s+/);
    var obj = {}; 
    var needs = ['user', 'system', 'nice', 'idle'];
    cpuTimesFields.forEach(function(v, i, a) {
        if (needs.indexOf(v) >= 0) {
            obj[v] = +the_line[i + 1] / CLOCK_TICKS;
        }
    });
    return obj;
}

function cpuTimes(cb) {

    var file = '/proc/stat';
    return _async_and_sync_get_handle_(file, 'readFile', __cpuTimes_handle, cb);

/*
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err, null);
                return;
            }
            var r = __cpuTimes_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(file, encoding);
    return __cpuTimes_handle(f);
    */
}


function __perCpuTimes_handle(f) {
    var lines = f.split(/\n(?!$)/);
    var cpus = [];
    var ln;
    var obj;
    var needs = ['user', 'system', 'nice', 'idle'];
    for (var i = 1, len = lines.length; i < len; i++) {
        ln = lines[i];
        obj = {};
        if (ln.indexOf('cpu') >= 0) {
            ln = ln.split(/\s+/);
            cpuTimesFields.forEach(function(v, idx) {
                if (needs.indexOf(v) >= 0) {
                    obj[v] = +ln[idx + 1] / CLOCK_TICKS;
                }
            });
            cpus.push(obj);
        }
    }
    return cpus;
}

function perCpuTimes(cb) {

    var file = '/proc/stat';
    return _async_and_sync_get_handle_(file, 'readFile', __perCpuTimes_handle, cb);

/*
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err, null);
                return;
            }
            var r = __perCpuTimes_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(file, encoding);
    return __perCpuTimes_handle(f);
    */
}


function cpuCountLogical(cb) {

    var file_cpuinfo = '/proc/cpuinfo';
    var file_stat = '/proc/stat';

    if (cb && typeof cb == 'function') {
        fs.readFile(file_cpuinfo, encoding, function(err, data) {
            if (err) {
                cb(err, null);
                return;
            }
            var n = data.match(/processor/gi) || [];
            if (n.length != 0) {
                cb (null, n.length);
                return;
            }
            fs.readFile(file_stat, encoding, function(err, data) {
                if (err) {
                    cb(err, null);
                    return;
                }
                n = data.match(/cpu\d/g) || [];
                cb(null, n.length);
            });
        }); 
        return;
    }

    var f = fs.readFileSync(file_cpuinfo, encoding);
    var n = f.match(/processor/gi) || [];
    if (n.length == 0) {
        f = fs.readFileSync(file_stat, encoding);
        n = f.match(/cpu\d/g) || [];
    }
    return n.length;
}


function __cpuCountPhysical_handle(f) {
    var n = f.match(/physical\sid\s*\:\s*\d+/gi);
    var o = {};
    var len = 0;
    if (n) {
        n.forEach(function(v) {
            if (!o[v]) {
                o[v] = true;
                len++;
            }
        });
    }
    return len;
}

function cpuCountPhysical(cb) {
    var file = '/proc/cpuinfo';
    return _async_and_sync_get_handle_(file, 'readFile', __cpuCountPhysical_handle, cb);

/*
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err, null);
                return;
            }
            var r = __cpuCountPhysical_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(file, encoding);
    return __cpuCountPhysical_handle(f);
    */
}


// 伪异步
function users(cb) {
    var _users = _linux.nsutil_users();
    var ret = [];
    _users.forEach(function(v) {
        if (v.user_proc) {
            if (v.hostname == ':0.0') {
                v.hostname = 'localhost';
            }
            ret.push({
                name: v.username,
                terminal: v.tty,
                host: v.host,
                start_time: v.start_time
            });
        }
    });

    if (cb && typeof cb == 'function') {
        cb(null, ret);
        return;
    }
    return ret;
}

function __bootTime_handle(f) {
    var t = f.match(/btime\s*(\d+)/gi);
    if (t) {
        t = +t[0].replace(/\D/g, '');
    }
    BOOT_TIME = t;
    return t * 1000;
}

function bootTime(cb) {
    var file = '/proc/stat';
    return _async_and_sync_get_handle_(file, 'readFile', __bootTime_handle, cb);
    /*
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err, null);
                return;
            }
            var r = __bootTime_handle(data);
            cb(null, r);
        });
        return;
    }
    var f = fs.readFileSync(file, encoding);
    return __bootTime_handle(f);
    */
}


function __pids_handle(f) {
    var s = [];
    f.forEach(function(v) {
        v = +v;
        if (!isNaN(v)) {
            s.push(v);
        }
    });
    s.sort(function(a, b) {
        return b - a;
    });
    return s;
}

function pids(cb) {
    var dir = '/proc';
    return _async_and_sync_get_handle_(dir, 'readdir', __pids_handle, cb);

/*
    if (cb && typeof cb == 'function') {
        fs.readdir(dir, function(err, data) {
            if (err) {
                cb(err, null);
                return;
            }
            var r = __pids_handle(data);
            cb(null, r);
        });
    }

    var f = fs.readdirSync(dir);
    return __pids_handle(f);
    */
}

function pidExists(pid, cb) {
    var r = _linux.nsutil_pid_exists(pid);
    if (cb && typeof cb == 'function') {
        cb(null, r);
        return;
    }
    return r;
}

function get_proc_inodes(pid, inodes) {
    var fds = fs.readdirSync('/proc/' + pid + '/fd');
    var inode;
    var fp;
    inodes = inodes || {};
    fds.forEach(function(fd) {
        fp = '/proc/' + pid + '/fd/' + fd;
        if (!fs.existsSync(fp)) return;
        inode = fs.readlinkSync(fp, 'utf8');
        if (inode.indexOf('socket:[') >= 0) {
            inode = inode.replace(/\D/g, '');
            if (!inodes[inode]) {
                inodes[inode] = [];
            }
            inodes[inode].push([pid, fd]);
        }
    });
    return inodes;
}
function get_all_inodes() {
    var inodes = {};
    var s = pids();
    for (var i = 0, len = s.length; i < len; i++) {
        get_proc_inodes(s[i], inodes); 
    }
    return inodes;
}

function decode_address(addr, family) {
    var arr = addr.split(':');
    var ip = arr[0];
    var port = null;
    if (arr.length == 2) {
        port = +('0x' + arr[1]);
    }

    //ip = +('0x' + ip);

    if (family == CONN.AF_INET) {    // ipv4
        ip = +('0x' + ip);
        var tt = [];
        tt[3] = (ip >>> 24) >>> 0;  
        tt[2] = ((ip << 8) >>> 24) >>> 0;  
        tt[1] = (ip << 16) >>> 24;  
        tt[0] = (ip << 24) >>> 24;  
        tt = tt.join('.')
    } else {    // ipv6
        //XXX: ipv6 ？？？
        var step = ip.length / 4;
        var tt = [];
        for (var i = 0; i < ip.length; i += step) {
            var str = ip.slice(i, i + step);
            tt.push((str >>> 16).toString(16));
            tt.push((str & 0x0000ffff).toString(16));
        }
        tt = tt.join(':')
    }

    return [tt, port]

}

//console.log(decode_address("0F02000A:0016", 'ipv4'));
//console.log(decode_address('0000000000000000FFFF00000100007F:9E49', 'ipv6'));

var tcp4 = ['tcp', CONN.AF_INET, CONN.SOCK_STREAM];
var tcp6 = ['tcp6', CONN.AF_INET6, CONN.SOCK_STREAM];
var udp4 = ['udp', CONN.AF_INET, CONN.SOCK_DGRAM];
var udp6 = ['udp6', CONN.AF_INET6, CONN.SOCK_DGRAM];
var unix = ['unix', CONN.AF_UNIX, null];

var tmap = {
    all: [tcp4, tcp6, udp4, udp6, unix],
    tcp: [tcp4, tcp6],
    tcp4: [tcp4],
    tcp6: [tcp6],
    udp: [udp4, udp6],
    udp4: [udp4],
    udp6: [udp6],
    unix: [unix],
    inet: [tcp4, tcp6, udp4, udp6],
    inet4: [tcp4, tcp6],
    inet6: [tcp6, udp6]
};

function process_inet(file, family, type_, inodes, filter_pid) {
    if (file.substr(-1, 1) == '6' || !fs.existsSync(file)) {
        // IPv6 not supported
        return;
    }
    f = fs.readFileSync(file, 'utf8');
    f = f.split(/\n(?!$)/);
    var line;
    var laddr, raddr, status, inode;
    var pid, fd;

    var ls = [];
    for (var i = 1, len = f.length; i < len; i++) {
        line = f[i].trim().split(/\s+/);
        laddr = line[1];
        raddr = line[2];
        status = line[3];
        inode = line[9];
        if (inodes[inode]) {
            if (inodes[inode].length > 1 && type_ != CONN.AF_UNIX) {
                throw "Value Error: ambiguos inode with multiple PIDs references"
            }
            pid = inodes[inode][0][0];
            fd = inodes[inode][0][1];
        } else {
            pid = null;
            fd = -1;
        }
        if (filter_pid && filter_pid != pid) {
            continue;
        } else {
            if (type_ == CONN.SOCK_STREAM) {
                status = _common.TCP_STATUSES_LINUX[status];
            } else {
                status = null;
            }
            laddr = decode_address(laddr, family);
            raddr = decode_address(raddr, family);
            ls.push({
                fd: fd,
                family: family,
                type: type_,
                laddr: laddr,
                raddr: raddr,
                status: status,
                pid: pid
            })
        }
    }
    return ls;
}

function process_unix(file, family, inodes, filter_pid) {
    var f = fs.readFileSync(file, 'utf8');
    f = f.split(/\n(?!$)/);
    var ln;
    var type_, inode;
    var pairs;
    var path;
    var pid, fd;
    var ls = [];
    for (var i = 1, len = f.length; i < len; i++) {
        ln = f[i].trim().split(/\s+/); 
        type_ = ln[4];
        inode = ln[6];
        if (inodes[inode]) {
            pairs = inodes[inode];
        } else {
            pairs = [[null, -1]];
        }
        for (var j = 0, lj = pairs.length; j < lj; j++) {
            pid = pairs[j][0];
            fd = pairs[j][1];
            if (filter_pid && filter_pid != pid) {
                continue;
            } else {
            
                if (ln.length == 8) {
                    path = ln[7];
                } else {
                    path = '';
                }
                raddr = null;
                status = null;
                ls.push({
                    fd: fd,
                    family: family,
                    type_: type_,
                    laddr: path,
                    raddr: raddr,
                    status: status,
                    pid: pid
                });
            }
        }
    }
    return ls;
}

function retrieve(kind, pid) {
    var f_t = tmap[kind];
    if(!f_t) {
        return;
    }
    var inodes;
    if (pid) {
        inodes = get_proc_inodes(pid);
    } else {
        inodes = get_all_inodes();
    }
    var f, family, type_;
    var tem;
    var ret = [];
    f_t.forEach(function(v) {
        f = v[0];
        family = v[1];
        type_ = v[2];
        //XXX-TODO
        var ls;
        if (family == CONN.AF_INET || family == CONN.AF_INET6) {
            ls = process_inet('/proc/net/' + f, family, type_, inodes, pid);
        } else {
            ls = process_unix('/proc/net/' + f, family, inodes, pid);
        }
        if (ls) {
            for(var i = 0, len = ls.length; i < len; i++) {
                ls[i].family = _common.CONN_FAMILY_STR[ls[i].family];
                ls[i].type = _common.CONN_TYPE_STR[ls[i].type];
                ret.push(ls[i]);
            }
        }
    });
    return ret;
}

// 伪异步
function net_connections(/*kind, cb*/) {
    var kind;
    var cb;
    if (arguments.length == 1) {
    
        if (typeof arguments[0] == 'string') {
            kind = arguments[0];
        }

        if (typeof arguments[0] == 'function') {
            cb = arguments[0];
        }

    } else if (arguments.length == 2) {
        kind = arguments[0];
        cb = arguments[1];
    }

    kind = kind || 'inet';
    var r = retrieve(kind);
    if (cb) {
        cb(null, r);
        return;
    }
    return r;
}

function __net_io_counters_handle(f) {
    f = f.split(/\n(?!$)/);
    var line;
    var ret = {};
    for (var i = 2, len = f.length; i < len; i++) {
        line = f[i].trim().split(/\s+/);
        ret[line[0]] = {
            ibytes: +line[1],
            ipkts: +line[2],
            ierrs: +line[3],
            idrops: +line[4],
            obytes: +line[9],
            opkts: +line[10],
            oerrs: +line[11],
            odrops: +line[12]
        };
    }
    return ret;
}

function net_io_counters(cb) {
    var file = '/proc/net/dev';
    return _async_and_sync_get_handle_(file, 'readFile', __net_io_counters_handle, cb);
/*
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err, null);
                return;
            }
            var r = __net_io_counters_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(file, encoding);
    return __net_io_counters_handle(f);
    */
}

function __disk_io_counters_handle_1(f) {
    f = f.split(/\n(?!$)/).reverse();
    var partitions = {};
    var lastName;
    var line;
    var name;
    for (var i = 0, len = f.length - 2; i < len; i++) {
        line = f[i].trim().split(/\s+/);
        name = line[line.length - 1];
        if (!isNaN(+name.substr(-1,1))) {
            partitions[name] = true;
            lastName = name;
        } else {
            if (!lastName || lastName.indexOf(name) < 0) {
                partitions[name] = true;
            }
        }
    }
    return partitions;
}
function __disk_io_counters_handle_2(f, partitions) {
    f = f.split(/\n(?!$)/);
    var name;
    var ret = {};
    var sector_size = 512;
    f.forEach(function(line) {
        line = line.trim().split(/\s+/);
        name = line[2];
        if (partitions[name]) {
            ret[name] = {
                reads: +line[3],
                writes: +line[7],
                read_bytes: +line[5] * sector_size,
                write_bytes: +line[9] * sector_size,
                read_time: +line[6],
                write_time: +line[10]
            };
        }
    });
    return ret;
}

function disk_io_counters(cb) {

    var file_1 = '/proc/partitions';
    var file_2 = '/proc/diskstats';

    if (cb && typeof cb == 'function') {
        fs.readFile(file_1, encoding, function(err, data) {
            if (err) {
                cb(err, null);
                return;
            }
            var partitions = __disk_io_counters_handle_1(data);
            fs.readFile(file_2, encoding, function(err2, data2) {
                if (err2) {
                    cb(err2, null);
                    return;
                }
                var r = __disk_io_counters_handle_2(data2, partitions);
                cb(null, r);
            });
        }); 
        return;
    }

    var f = fs.readFileSync(file_1, encoding);
    var partitions = __disk_io_counters_handle_1(f);
    //console.log(partitions);
    f = fs.readFileSync(file_2, encoding);
    return __disk_io_counters_handle_2(f, partitions);
}

function __diskPartitions_handle(f) {
    var phydevs = {};
    f = f.split(/\n(?!$)/);
    f.forEach(function(line) {
        if (line.indexOf('nodev') < 0) {
            phydevs[line.trim()] = true;
        }
    });
    var ret = [];
    var partitions = _linux.nsutil_disk_partitions();
    var device, mount_point, fs_type, options; 
    partitions.forEach(function(pt) {
        if (pt.device && phydevs[pt.fstype]) {
            ret.push({
                device: pt.device,
                fs_type: pt.fstype,
                mount_point: pt.mountpoint,
                options: pt.opts
            });
        }
    });
    return ret;
}
function diskPartitions(cb) {
    var file = '/proc/filesystems';
    return _async_and_sync_get_handle_(file, 'readFile', __diskPartitions_handle, cb);
   /* 
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __diskPartitions_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(file, encoding);
    return __diskPartitions_handle(f);
    */
}

var __class_proc__ = function(pid) {
    this.pid = pid;
};

var Process = function(pid) {
    return new __class_proc__(pid);
};

var proto = __class_proc__.prototype;

function __proto_name_handle(f) {
    return f.trim().split(/\s+/)[1].replace(/\(|\)/g, '');
}
proto.name = function(cb) {
    var self = this;
    var fname = '/proc/' + self.pid + '/stat';
    
    return _async_and_sync_get_handle_(fname, 'readFile', __proto_name_handle, cb);
    /*
    if (cb && typeof cb == 'function') {
        fs.readFile(fname, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __proto_name_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(fname, encoding);
    return __proto_name_handle(f);
    */
};

function __proto_exe_handle(_exe) {
    _exe = _exe.split('\x00')[0];
    if (_exe.indexOf(' (deleted)') >= 0 && !fs.existsSync(_exe)) {
        _exe = _exe.replace(' (deleted)', '');
    }
    return _exe;
}
proto.exe = function(cb) {
    var self = this;
    var basePath = '/proc/' + self.pid;
    var path = basePath + '/exe';

    if (cb && typeof cb == 'function') {
        fs.readlink(path, function(err, data) {
            if (err) {
                fs.readdir(basePath, function(err, data) {
                    if (err) {
                        cb(err);
                        return;
                    }
                    cb(null, '');
                });
                return;
            }
            var _exe = __proto_exe_handle(data);
            cb(null, _exe);
        });
        return;
    }

    var _exe;
    if (fs.existsSync(path)) {
        _exe = fs.readlinkSync(path);
    } else {
        if (fs.existsSync(basePath)) {
            return '';
        }
        throw "[err] no such process " + self.pid;
    }
    return __proto_exe_handle(_exe);
};

function __proto_cmdline_handle(f) {
    f = f.split('\x00').filter(function(v) {
        return v != '';
    });
    return f;
}
proto.cmdline = function(cb) {
    var self = this;
    var fname = '/proc/' + self.pid + '/cmdline';
    return _async_and_sync_get_handle_(fname, 'readFile', __proto_cmdline_handle, cb);
   /* 
    if (cb && typeof cb == 'function') {
        fs.readFile(fname, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __proto_cmdline_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(fname, encoding);
    return __proto_cmdline_handle(f);
    */
};


// 异步接口同时调用两个异步操作
// 用计数方式判断两个异步都结束后，再执行回调
function __proto_terminal_handle(tm, fc, cb) {
    if (!tm || !fc) return;
    var ttynr = fc.split(' ')[6];
    cb(null, tm[ttynr]);
}

proto.terminal = function(cb) {
    var self = this;
    var file = '/proc/' + self.pid + '/stat';

    if (cb && typeof cb == 'function') {

        var tm;
        var fc;
        _posix.getTerminalMap(function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            tm = data;
            __proto_terminal_handle(tm, fc, cb);
        });
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            fc = data;
            __proto_terminal_handle(tm, fc, cb);
        });
    
        return;
    }

    var tmap = _posix.getTerminalMap();
    var f = fs.readFileSync(file, encoding);
    var tty_nr = f.split(' ')[6];
    return tmap[tty_nr];
};


function __proto_ioCounters_handle(f) {
    f = f.split(/\n/);
    var ret = {};
    f.forEach(function(line) {
        if (line.indexOf('syscr') >= 0) {   
            ret.rcount = +line.split(/\s+/)[1];
        }
        if (line.indexOf('syscw') >= 0) {
            ret.wcount = +line.split(/\s+/)[1];
        }
        if (line.indexOf('read_bytes') >= 0) {
            ret.rbytes = +line.split(/\s+/)[1];
        }
        if (line.indexOf('write_bytes') >= 0) {
            ret.wbytes = +line.split(/\s+/)[1];
        }
    });
    return ret;
}
proto.ioCounters = function(cb) {
    var self = this;
    var fname = '/proc/' + self.pid + '/io';
    if (!fs.existsSync(fname)) {
        throw 'couldn\'t find ' + fname + ' (kernel too old?)';
    }

    return _async_and_sync_get_handle_(fname, 'readFile', __proto_ioCounters_handle, cb);
    /*
    if (cb && typeof cb == 'function') {
        fs.readFile(fname, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __proto_ioCounters_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(fname, encoding);
    return __proto_ioCounters_handle(f);
    */
};

function __proto_cpuTimes_handle(f) {
    f = f.trim().split(/\s+/);
    return {
        utime: f[13] / CLOCK_TICKS,
        stime: f[14] / CLOCK_TICKS
    };
}
proto.cpuTimes = function(cb) {
    var self = this;
    var file = '/proc/' + self.pid + '/stat';
    return _async_and_sync_get_handle_(file, 'readFile', __proto_cpuTimes_handle, cb);
    /*
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __proto_cpuTimes_handle(data);
            cb(null, r);
        });
        return;
    }
    var f = fs.readFileSync(file, encoding);
    return __proto_cpuTimes_handle(f);
    */
};

//TODO: proto.wait (like: psutil.Process.wait)

function __proto_createTime_handle(f) {
    f = f.trim().split(/\s+/)[21];
    f = f / CLOCK_TICKS + (BOOT_TIME || bootTime());
    return f * 1000;
}
proto.createTime = function(cb) {
    var self = this;
    var file = '/proc/' + self.pid + '/stat';
    return _async_and_sync_get_handle_(file, 'readFile', __proto_cpuTimes_handle, cb);
   /* 
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __proto_createTime_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(file, encoding);
    return __proto_createTime_handle(f);
    */
};

function __proto_memoryInfo_handle(f) {
    f = f.split(/\s+/);
    return {
        rss: f[1] * PAGE_SIZE,
        vms: f[0] * PAGE_SIZE
    };
}
proto.memoryInfo = function(cb) {
    var self = this;
    var file = '/proc/' + self.pid + '/statm';
    return _async_and_sync_get_handle_(file, 'readFile', __proto_memoryInfo_handle, cb);
/*
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __proto_memoryInfo_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(file, encoding);
    return __proto_memoryInfo_handle(f);
    */
};  

function __proto_memMaps_handle(f) {
    f = f.split(/\n/);
    
    var line;
    var arr = [];
    var tem;
    for (var i = 0, len = f.length; i < len; i++) {
        line = f[i];
        line = line.split(/\s+/);
        if (line[0].indexOf(':') < 0) {
            if (tem) {
                arr.push(tem);
            }
            tem = {};
            tem.pmmap_ext = line[0];
            tem.addr_perms = line[1];
            tem.path = line[5];
            continue;
        }
        if (!tem) continue;
        tem[line[0].replace(':', '')] = line[1] * 1024;
    }
    return arr;
}
proto.memMaps = function(cb) {
    var self = this;
    var fname = '/proc/' + self.pid + '/smaps';
    return _async_and_sync_get_handle_(fname, 'readFile', __proto_memMaps_handle, cb);
/*
    if (cb && typeof cb == 'function') {
        fs.readFile(fname, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __proto_memMaps_handle(data);
            cb(null, data);
        });
        return;
    }

    if (!fs.existsSync(fname)) {
        return;
    }
    var f = fs.readFileSync(fname, encoding);
    return __proto_memMaps_handle(f);
    */
};

function __proto_cwd_handle(f) {
    return f.replace('\x00', '');
}
proto.cwd = function(cb) {
    var self = this;
    var path = '/proc/' + self.pid + '/cwd';
    return _async_and_sync_get_handle_(path, 'readlink', __proto_cwd_handle, cb);
    /*
    if (cb && typeof cb == 'function') {
        fs.readlink(path, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __proto_cwd_handle(data);
            cb(null, r);
        });
        return;
    }
    var f = fs.readlinkSync(path);
    return __proto_cwd_handle(f);
    */
};

function __proto_numCtxSwitches_handle(f) {
    f = f.split(/\n/);
    var ret = {};
    f.forEach(function(v) {
        if (v.indexOf('nonvoluntary_ctxt_switches') >= 0) {
            ret.involuntary = +v.split(/\s+/)[1];
        }else if (v.indexOf('voluntary_ctxt_switches') >= 0) {
            ret.voluntary = +v.split(/\s+/)[1];
        }

    });
    return ret;
}
proto.numCtxSwitches = function(cb) {
    var self = this;
    var file = '/proc/' + self.pid + '/status';
    return _async_and_sync_get_handle_(file, 'readFile', __proto_numCtxSwitches_handle, cb);
    /*
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __proto_numCtxSwitches_handle(data);
            cb(null, r);
        });
        return;
    }
    var f = fs.readFileSync(file, encoding);
    return __proto_numCtxSwitches_handle(f);*/
};

function __proto_numThreads_handle(f) {
    f = f.split(/\n/);
    var ret;
    f.forEach(function(v) {
        if (v.indexOf('Threads:') >= 0) {
            ret = v.split(/\s+/)[1];
        }
    });
    return ret;
}
proto.numThreads = function(cb) {
    var self = this;
    var file = '/proc/' + self.pid + '/status';
    return _async_and_sync_get_handle_(file, 'readFile', __proto_numThreads_handle, cb);
    /*
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __proto_numThreads_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(file, encoding);
    return __proto_numThreads_handle(f);
    */
};

proto.threads = function(cb) {
    var self = this;

    var dir = '/proc/' + self.pid + '/task';

    if (cb && typeof cb == 'function') {
        fs.readdir(dir, function(err, ids) {
            if (err) {
                cb(err);
                return;
            }
            var count = ids.length;
            var arr = [];
            ids.forEach(function(v) {
                fs.readFile(dir + '/' + v + '/stat', encoding, function(err2, data) {
                    if (err2) {
                        err = err2;
                    } else {
                        data = data.trim().split(/\s+/);
                        arr.push({
                            idx: v * 1,
                            user: data[13] * 1,
                            sys: data[14] * 1
                        });
                    }
                    if (0 == --count) {
                        cb(err, arr);
                    }
                });
            });
        });
        return;
    }

    var ids = fs.readdirSync(dir);
    ids.sort(function(a, b) {
        return a - b;
    });
    var arr = [];
    ids.forEach(function(v) {
        var f = fs.readFileSync(dir + '/' + v + '/stat', encoding);
        f = f.trim().split(/\s+/);
        arr.push({
            idx: v * 1,
            user: f[13] * 1,
            sys: f[14] * 1
        });
    });
    return arr;
};

proto.getNice = function(cb) {
    var r = _posix.getPriority(this.pid);
    if (cb && typeof cb == 'function') {
        cb(null, r);
        return;
    }
    return r;
};
proto.setNice = function(nice, cb) {
    var r = _posix.setPriority(this.pid, nice);
    if (cb && typeof cb == 'function') {
        cb(null, r);
        return;
    }
    return r;
};

/* TODO
 *
 * cpu_affinity_get
 * cpu_affinity_set
 * ionice_get
 * ionice_set
 * rlimit
 *
 * */

function __proto_status_handle(f) {
    f = f.match(/State\:\s+[A-Z]/g);
    f = f[0].replace(/State\:\s+/, '');
    return _common.PROC_STATUSES_LINUX[f];
}
proto.status = function(cb) {
    var self = this;
    var file = '/proc/' + self.pid + '/status';
    return _async_and_sync_get_handle_(file, 'readFile', __proto_status_handle, cb);
    /*
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __proto_status_handle(data);
            cb(null, r);
        });
        return;
    }
    var f = fs.readFileSync(file, encoding);
    return __proto_status_handle(f);
    */
};

proto.openFiles = function(cb) {
    var self = this;
    var path = '/proc/' + self.pid + '/fd';
    var arr = [];

    if (cb && typeof cb == 'function') {
        fs.readdir(path, function(err, files) {
            if (err) {
                cb(err);
                return;
            }
            var count = files.length;
            files.forEach(function(v) {
                var link = path + '/' + v;
                fs.readlink(link, function(err2, data) {
                    if (!err2 && data.indexOf('/') >= 0 && fs.statSync(data).isFile) {
                        arr.push(data);
                    }
                    if (0 == --count) {
                        cb(null, arr);
                    }
                });
            });
        });
        return;
    }

    var f = fs.readdirSync(path);
    f.forEach(function(v) {
        var file = path + '/' + v;
        if (fs.existsSync(file)) {
            file = fs.readlinkSync(file);
            if (file.indexOf('/') >= 0 && fs.statSync(file).isFile()) {
                arr.push(file);
            }
        }
    });
    return arr;
};

proto.connections = function(/*kind, cb*/) {
    var kind;
    var cb;

    if (arguments.length == 1) {
        if (typeof arguments[0] == 'string') {
            kind = arguments[0]
        }
        if (typeof arguments[0] == 'function') {
            cb = arguments[0];
        }
    }
    if (arguments.length == 2) {
        kind = arguments[0];
        cb = arguments[1];
    }

    kind = kind || 'inet';
    var r = retrieve(kind, this.pid);
    if (cb && typeof cb == 'function') {
        cb(null, r);
        return;
    }
    return r;
};

proto.numFds = function(cb) {
    var dir = '/proc/' + this.pid + '/fd';
    return _async_and_sync_get_handle_(dir, 'readdir', function(d) {
        return d.length;
    }, cb);
    /*
    if (cb && typeof cb == 'function') {
        fs.readdir(dir, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            cb(null, data.length);
        });
        return;
    }
    return fs.readdirSync(dir).length;
    */
};

proto.ppid = function(cb) {
    var self = this;
    var file = '/proc/' + self.pid + '/status';

    return _async_and_sync_get_handle_(file, 'readFile', function(d) {
        return d.match(/PPid\:\s+\d+/g)[0].replace(/PPid\:\s+/, '');
    }, cb);
    /*
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            data = data.match(/PPid\:\s+\d+/g)[0].replace(/PPid\:\s+/, '');
            cb(null, data);
        });
        return;
    }
    var f = fs.readFileSync(file, encoding);
    f = f.match(/PPid\:\s+\d+/g)[0].replace(/PPid\:\s+/, '');
    return f;
    */
};

function __proto_uids_handle(f) {
    f = f.match(/Uid\:[\s\d]+\n/g);
    f = f[0].split(/\s+/);
    return {
        real: f[1],
        effective: f[2],
        saved: f[3]
    };
}
proto.uids = function(cb) {
    var self = this;
    var file = '/proc/' + self.pid + '/status';
    return _async_and_sync_get_handle_(file, 'readFile', __proto_uids_handle, cb);
    /*
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __proto_uids_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(file, encoding);
    return __proto_uids_handle(f);
    */
};

function __proto_gids_handle(f) {
    f = f.match(/Gid\:[\s\d]+\n/g);
    f = f[0].split(/\s+/);
    return {
        real: f[1],
        effective: f[2],
        saved: f[3]
    };
}
proto.gids = function(cb) {
    var self = this;
    var file = '/proc/' + self.pid + '/status';
    return _async_and_sync_get_handle_(file, 'readFile', __proto_gids_handle, cb);
   /* 
    if (cb && typeof cb == 'function') {
        fs.readFile(file, encoding, function(err, data) {
            if (err) {
                cb(err);
                return;
            }
            var r = __proto_gids_handle(data);
            cb(null, r);
        });
        return;
    }

    var f = fs.readFileSync(file, encoding);
    return __proto_gids_handle(f);
    */
};

module.exports = {
    //getCputimesFields: getCputimesFields,
    virtualMemory: virtualMemory,
    swapMemory: swapMemory,
    cpuTimes: cpuTimes,
    perCpuTimes: perCpuTimes,
    cpuCountLogical: cpuCountLogical,
    cpuCountPhysical: cpuCountPhysical,
    users: users,
    bootTime: bootTime,
    pids: pids,
    //pidExists: pidExists,
    //conn
    //get_proc_inodes: get_proc_inodes,
    //get_all_inodes: get_all_inodes
    //net
    netConnections: net_connections,
    netIOCounters: net_io_counters,
    //disk
    diskIOCounters: disk_io_counters,
    diskPartitions: diskPartitions,
    //Process
    Process: Process
};


