var assert = require('assert');
var ns = require('../src/nsutil/nsosx.js'); 

describe('[platform] osx', function() {

    it ('[method] virtualMemory', function() {
        var vMem = ns.virtualMemory();
        console.log(vMem);
    });

    it ('[method] swapMemory', function() {
        var sMem = ns.swapMemory();
        console.log(sMem);
        assert.equal(sMem.total, sMem.used + sMem.free);
    });

    it ('[method] cpuTimes & perCpuTime', function() {
        var c = ns.cpuTimes();
        console.log(c);
    })

    it ('[method] perCpuTimes', function() {
        var p = ns.perCpuTimes();
        console.log(p);
    });

    it ('[method] cpuCountLogical', function() {
        var n = ns.cpuCountLogical();
        console.log(n);
    })

    it ('[method] cpuCountPhys', function() {
        var n = ns.cpuCountPhys();
        console.log(n);
    })

    it ('[method] bootTime', function() {
        var t = ns.bootTime();
        console.log(t);
    });

    it ('[method] diskPartitions', function() {
        var d = ns.diskPartitions();
        console.log(d);
    })

    it ('[method] users', function() {
        var u = ns.users();
        console.log('[');
        u.forEach(function(v) {
            console.log(' ', JSON.stringify(v));
        });
        console.log(']');
    });

    it ('[method] pids', function() {
        var p = ns.pids();
        console.log(JSON.stringify(p));
    });

    it ('[method] netConnections', function() {
        var c = ns.netConnections();
        console.log('格式参见proc.connections');
    });

    it ('[method] netIOCounters', function() {
        var n = ns.netIOCounters();
        console.log(n);
    })

    it ('[method] diskIOCounters', function() {
        var d = ns.diskIOCounters();
        console.log(d);
    })

    describe('[Class] Process', function() {
        
        var pid = ns.pids()[0];
        var proc = ns.Process(pid);

        it ('[method] process.name', function() {
            var n = proc.name();
            console.log(n);
        })

        it ('[method] process.exe', function() {
            var e = proc.exe();
            console.log(e);
        })

        it ('[method] process.cmdline', function() {
            var c = proc.cmdline();
            console.log(c);
        })

        it ('[method] process.ppid', function() {
            var p = proc.ppid();
            console.log(p);
        })

        it ('[method] proc.cwd', function() {
            var c = proc.cwd();
            console.log(c);
        })

        it ('[method] proc.uids', function() {
            var u = proc.uids();
            console.log(u);
        })

        it ('[method] proc.gids', function() {
            var g = proc.gids();
            console.log(g);
        })

        it ('[method] proc.teminal', function() {
            var t = proc.terminal();
            console.log(t);
        })

        it ('[method] proc.memoryInfo', function() {
            var m = proc.memoryInfo();
            console.log(m);
        })

        it ('[method] proc.cpuTimes', function() {
            var c = proc.cpuTimes();
            console.log(c);
        })
        
        it ('[method] proc.createTime', function() {
            var t = proc.createTime();
            console.log(t);
        })

        it ('[method] proc.numCtxSwitches', function() {
            var c = proc.numCtxSwitches();
            console.log(c);
        })

        it ('[method] proc.numThreads', function() {
            var t = proc.numThreads();
            console.log(t);
        })
        
        it ('[method] proc.openFiles', function() {
            var f = proc.openFiles();
            console.log(f);
        })

        it ('[method] proc.connections', function() {
            var c = proc.connections();
           // c = ns.Process(1421).connections('inet');
            console.log(c);
        })

        it ('[method] proc.numFds', function() {
            var n = proc.numFds();
            console.log(n);
        })

        it ('[method] proc.getNice', function() {
            var n = proc.getNice();
            console.log(n);
        })

        it ('[method] proc.setNice', function() {
            proc.setNice(1);
            var n = proc.getNice();
            console.log(n);
        })

        it ('[method] proc.status', function() {
            var st = proc.status();
            console.log(st);
        })

        it ('[method] proc.threads', function() {
            var t = proc.threads();
            console.log(t);
        });

        it ('[method] proc.memMaps', function() {
            var m = proc.memMaps();
            console.log('[');
            m.forEach(function(v) {
                console.log(JSON.stringify(v));
            });
            console.log(']');
        })

    })

});
