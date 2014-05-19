var fs = require('fs');
var _common = require('./common.js');
var _linux = require('../../build/Release/nsutil_linux.node'); 

function getCputimesFields() {
    var f = fs.readFileSync('/proc/stat', 'utf8');
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


function virtualMemory() {

    var sysinfo = _linux.nsutil_sysinfo();
    var f = fs.readFileSync('/proc/meminfo', 'utf8');
    var lines = f.split(/\n/);
    var cached;
    var active;
    var inactive;
    lines.forEach(function(ln) {
        if (ln.indexOf('Cached')) {
            cached = ln.split(/\s+/)[1] * 1024;
        }
        if (ln.indexOf('Active')) {
            active = ln.split(/\s+/)[1] * 1024;
        }
        if (ln.indexOf('Inactive')) {
            inactive = ln.split(/\s+/)[1] * 1024;
        }
    });
    var avail = sysinfo.free + sysinfo.buffers + cached;
    var used = sysinfo.total - sysinfo.free;
    var percent = _common.usagePercent(sysinfo.total - avail, sysinfo.total, 2); 
    return {
        total: sysinfo.total,
        avail: avail,
        percent: percent,
        used: uesd,
        free: sysinfo.free,
        active: active,
        inactive: inactive,
        buffers: sysinfo.buffers,   // phymem buffers
        cached: cached //phymem cached
    };
    
}

function swapMemory() {
    var sysinfo = _linux.nsutil_sysinfo();
    var used = sysinfo.total - sysinfo.free;
    var percent = _common.usagePercent(used, sysinfo.total, 2);
    var f = fs.readFileSync('/proc/vmstat', 'utf8');
    var lines = f.split(/\n/);
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
        percent: percent,
        sin: pswpin,
        sout: pswpout
    };
}

function cpuTimes() {
    var f = fs.readFileSync('/proc/stat', 'utf8');
    console.log(f);
}

module.exports = {
    getCputimesFields: getCputimesFields,
    virtualMemory: virtualMemory,
    swapMemory: swapMemory,
    cpuTimes: cpuTimes
};


