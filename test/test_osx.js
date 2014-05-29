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
        'users', 'pids', 'netConnections', 'netIOCounters', 'diskIOCounters'
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
        it ('[function] ' + apiName, function() {
            var r = ns[apiName]();
            //console.log(r);
        });
        it ('[function async] ' + apiName, function(done) {
            ns[apiName](function(err, res) {
                done();
            });
        });
    });
    /**/
    describe('[Class] Process', function() {
        
        var pid = ns.pids()[0];
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
            });
            it ('[method async] proc.' + apiName, function(done) {
                if (apiName == 'setNice') {
                    proc[apiName](1, function(err, res) {
                        done();
                    });
                } else {
                    proc[apiName](function(err, res) {
                        done();
                    });
                }
            });
        });
        /**/

    })

});
