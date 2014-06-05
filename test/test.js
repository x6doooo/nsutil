var assert = require('assert');
var os = require('os');
var platform = os.platform();
var ns;
if (platform == 'darwin') {
    ns = require('../src/nsutil/nsosx.js'); 
}
if (platform == 'linux') {
    ns = require('../src/nsutil/nslinux.js');
}


var apiList = {
    functions: [
        'virtualMemory', 'swapMemory', 'cpuTimes', 'perCpuTimes',
        'cpuCountLogical', 'cpuCountPhysical', 'bootTime', 'diskPartitions',
        'users', 'pids', 'netConnections', 'netIOCounters', 'diskIOCounters',
        'diskUsage'
    ],
    process: [
        'name', 'exe', 'cmdline', 'ppid', 'cwd', 'uids', 'gids',
        'terminal', 'memoryInfo', 'cpuTimes', 'createTime', 'numCtxSwitches',
        'numThreads', 'openFiles', 'connections', 'numFds', 'getNice',
        'setNice', 'status', 'threads', 'memMaps'
    ]
};


describe('[platform] ' + platform, function() {

    // 测试接口是否能跑通，数据正确性不在这里测
    /**/
    apiList.functions.forEach(function(apiName) {
        var p;
        if (apiName == 'diskUsage') p = '/';
        it ('[function] ' + apiName, function() {
            if (p) {
                var r = ns[apiName](p);
            } else {
                var r = ns[apiName]();
            }
            if (typeof r == 'undefined') {
                throw new Error('result undefined');
            }
            //console.log(r);
        });
        it ('[function] ' + apiName + ' (async)', function(done) {
            if (p) {
                ns[apiName](p, function(err, res) {
                    if (err) {
                        throw new Error('result err');
                    }
                    if (typeof res == 'undefined') {
                        throw new Error('result undefined');
                    }
                   // console.log(res);
                    done();
                });
            } else {
                ns[apiName](function(err, res) {
                    if (err) {
                        throw new Error('result err');
                    }
                    if (typeof res == 'undefined') {
                        throw new Error('result undefined');
                    }
                   // console.log(res);
                    done();
                });
            }
        });
    });
    /**/
    describe('[Class] Process', function() {
        
        var pid = ns.pids()[0];
        //pid = pid[pid.length - 1];
        var proc = ns.Process(pid);
        /**/
        apiList.process.forEach(function(apiName) {
            it ('[method] proc.' + apiName, function() {
                var r;
                if (apiName == 'setNice') {
                    r = proc[apiName](1);
                } else {
                    r = proc[apiName]();
                }
                if (typeof r == 'undefined') {
                    throw new Error('result undefined');
                }
            });
            it ('[method] proc.' + apiName + ' (async)', function(done) {
                if (apiName == 'setNice') {
                    proc[apiName](1, function(err, res) {
                        if (err) {
                            throw new Error('result err');
                        }
                        if (typeof res == 'undefined') {
                            throw new Error('result undefined');
                        }
                        done();
                    });
                } else {
                    proc[apiName](function(err, res) {
                        if (err) {
                            throw new Error('result err');
                        }
                        if (typeof res == 'undefined') {
                            throw new Error('result undefined');
                        }
                        done();
                    });
                }
            });
        });
        /**/

    });
});
