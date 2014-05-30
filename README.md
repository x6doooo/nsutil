Nsutil.js
===
Node.js system utilities

[![NPM](https://nodei.co/npm/nsutil.png)](https://nodei.co/npm/nsutil/)

##Summary

* a fork of [psutil](https://code.google.com/p/psutil/), rebuilt with Node.js
* support OSX & Linux

##Install

    npm install nsutil

##Prebuilt binaries (node v0.10.28)

    OSX 64-bit
    Linux 64-bit
    Linux 32-bit

##Rebuild (if you need)

```bash
node-gyp configure build
```

##Usage

```js
var ns = require('nsutil')

// synchronous with return
res = ns.cpuTimes()

// callback (asynchronous or synchronous)
ns.cpuTimes(function(err, res) {
    if (!err) console.log(res);
})

// output => 
 { user: 40.76, nice: 118.53, system: 683.01, idle: 287470.94 }

```

##APIs

####CPU

#####[Function] cpuTimes
```js
// sync
res = ns.cpuTimes()

// callback
ns.cpuTimes(function(err, res) {
    if (err) return;
    console.log(res);
});

// output => 
{ user: 40.76, nice: 118.53, system: 683.01, idle: 287470.94 }

```

#####[Function] perCpuTimes
```js
// sync
res = ns.perCpuTimes()
    
// callback
ns.perCpuTimes(function(err, res) { ... })

// output =>
 [ { user: 174.18, nice: 0, sys: 123.64, idle: 1411.73 },
   { user: 68.05,  nice: 0, sys: 42.15,  idle: 1599.30 },
   { user: 129.05, nice: 0, sys: 62.55,  idle: 1517.90 },
   { user: 58.15,  nice: 0, sys: 28.77,  idle: 1622.58 } ] 
```

#####[Function] cpuCountLogical (cpuCountPhysical)
```js
// sync
res = ns.cpuCountLogical()

// callback
ns.cpuCountLogical(function(err, res) { ... })

// output => 2
```

####Memory

#####[Function] virtualMemory
```js
// sync
res = ns.virtualMemory()

// callback
ns.virtualMemory(function(err, res) { ... })

// output =>
 // OSX
 { total:    4294967296,
   active:   1476833280,
   avail:    1163386880,
   used:     3606433792,
   free:     237256704,
   inactive: 926130176,
   wire:     1203470336 }
 // Linux
 { total:    383238144,
   active:   66637824,
   avail:    315265024,
   used:     206503936,
   free:     176734208,
   inactive: 71880704,
   buffers:  25927680,
   cached:   112603136 }
```

#####[Function] swapMemory
```js
// sync
res = ns.swapMemory()

// callback
ns.swapMemory(function(err, res) { ... });

// output =>
 { total: 1073741824,
   used:  6815744,
   free:  1066926080,
   sin:   2724483072,
   sout:  20860928 }    
```

####Disks

#####[Function] diskPartitions
```js
// sync
res = ns.diskPartitions()

// callback
ns.diskPartitions(function(err, res) { ... })

// output =>
 [ { device: '/dev/disk0s2',
     mount_point: '/',
     fs_type: 'hfs',
     options: 'rw,local,rootfs,dovolfs,journaled,multilabel' },
   { device: 'devfs',
     mount_point: '/dev',
     fs_type: 'devfs',
     options: 'rw,local,dontbrowse,multilabel' },
     ...] 
```

#####[Function] diskIOCounters
```js
// sync
res = ns.diskIOCounters()

// callback
ns.diskIOCounters(function(err, res) { ... })

// output =>
 { disk0: 
   { reads: 1170484,
     writes: 668337,
     read_bytes: 18236701696,
     write_bytes: 21262711296,
     read_time: 825561,
     write_time: 540807 },
   disk1:
   { reads: 11704,
     writes: 6683,
     read_bytes: 182367016,
     write_bytes: 212627112,
     read_time: 8255,
     write_time: 5408 }
   ...}      
```

####Network

#####[Function] netConnections
```js
// sync
res = ns.netConnections()

// callback
ns.netConnections(function(err, res) { ... })

// output =>
 [ { fd: 22,
     family: 'AF_INET',
     type: 'SOCK_STREAM',
     laddr: ['X.X.X.X', XXXX],
     raddr: ['X.X.X.X', XXXX],
     state: 'ESTABLISHED' },
    ...]
```

#####[Function] netIOCounters
```js
// sync
res = ns.netIOCounters()

// callback
ns.netIOCounters(function(err, res) { ... })

// output => 
 { bridge0: 
   { obytes: 684,
     ibytes: 0,
     opkts: 2,
     ipkts: 0,
     oerrs: 0,
     ierrs: 0,
     iqdrops: 0 },
  en0: 
   { obytes: 160304770,
     ibytes: 2578607670,
     opkts: 1614175,
     ipkts: 2421909,
     oerrs: 0,
     ierrs: 0,
     iqdrops: 0 },
  ...}
```

####Other system info

#####[Function] bootTime
```js
// sync
res = ns.bootTime()

// callback
ns.bootTime(function(err, res) { ... })

// output =>
 1400543744000 // timestamp ms
```

#####[Function] users
```js
// sync
res = ns.users()

// callback
ns.users(function(err, res) { ... })

// output =>
 [
  {"username":"Dx.Yang", "tty":"console", "host":"", "startTime":1400548608},
  {"username":"Dx.Yang", "tty":"ttys000", "host":"", "startTime":1400548608},
  {"username":"Dx.Yang", "tty":"ttys001", "host":"", "startTime":1400548608}
 ]
```

####Process management

#####[Function] pids
```js
// sync
res = ns.pids()

// callback
ns.pids(function(err, res) { ... })

// output =>
 [6652,6651,6640,6639,6638,6633,6632,6615,6606...]
```

####[Class] Process
```js
// only sync
proc = ns.Process(6652) // arguments[0] is a pid
// return a instance of Process
```

#####[Method] proc.name
```js
// sync
res = proc.name()

// callback
proc.name(function(err, res) { ... })

// output =>
 'node'
```

#####[Method] proc.exe
```js
// sync
res = proc.exe()

// callback
proc.exe(function(err, res) { ... })

// output =>
 '/usr/local/bin/node'
```

#####[Method] proc.cmdline
```js
// sync
res = proc.cmdline()

// callback
proc.cmdline(function(err, res) { ... })

// output =>
 [ 'node',
   '/usr/local/lib/node_modules/mocha/bin/_mocha',
   'test_osx.js',
   '-R',
   'spec' ]
```

#####[Method] proc.ppid
```js
// sync
res = proc.ppid()

// callback
proc.ppid(function(err, res) { ... })

// output =>
 6651  //parent process id    
```

#####[Method] proc.cwd
```js
// sync
res = proc.cwd()

// callback
proc.cwd(function(err, res) { ... })

// output =>
 '/Users/node_modules/nsutil/test'
```

#####[Method] proc.uids
```js
// sync
res = proc.uids()

// callback
proc.uids(function(err, res) { ... })

// output =>
 { real: 501, effective: 501, saved: 501 }
```

#####[Method] proc.gids
```js
// sync
res = proc.gids()

// callback
proc.gids(function(err, res) { ... })

// output =>
 { real: 20, effective: 20, saved: 20 }
```

#####[Method] proc.terminal
```js
// sync
res = proc.terminal()

// callback
proc.terminal(funciton(err, res) { ... })

// output =>
 '/dev/ttys004'
```

#####[Method] proc.memoryInfo
```js
// sync
res = proc.memoryInfo()   

// callback
proc.memoryInfo(function(err, res) { ... })

// output =>
 { rss: 18440192, vms: 3119169536 }
```

#####[Method] proc.cpuTimes
```js
// sync
res = proc.cpuTimes()

// callback
proc.cpuTimes(function(err, res) { ... })

// output =>
 { user: 0.139774113, sys: 0.027113125 }
```

#####[Method] proc.createTime
```js
// sync
proc.createTime()

// callback
proc.createTime(function(err, res) { ... })

// output => 
 1400565545000 // timestamp ms
```

#####[Method] proc.numCtxSwitches
```js
// sync
res = proc.numCtxSwitches()

// callback
proc.numCtxSwitches(function(err, res) { ... })

// output =>
 { voluntary: 32, involuntary: 4 } 
```

#####[Method] proc.numThreads
```js
// sync
res = proc.numThreads()

// callback
proc.numThreads(function(err, res) { ... })

// output =>
 4
```

#####[Method] proc.openFiles
```js
// sync
res = proc.openFiles()

// callback
proc.openFiles(function(err, res) { ... })

// output =>
 [ { path: '/dev/ttys004', fd: 0 },
   { path: '/dev/ttys004', fd: 1 },
   { path: '/dev/ttys004', fd: 2 },
  ... ]
```

#####[Method] proc.connections
```js
// sync
res = proc.connections()    // default is 'all'
// or
res = proc.connections('inet') // or tcp、tcp4、tcp6、udp、udp4、udp6、unix、inet、inet4、inet6

// callback
proc.connections(function(err, res) { ... })
// or
proc.connections('inet', function(err, res) { ... })

// output =>
 [ { fd: 22,
     family: 'AF_INET',
     type: 'SOCK_STREAM',
     laddr: ['X.X.X.X', XXXX],
     raddr: ['X.X.X.X', XXXX],
     state: 'ESTABLISHED' },
    ...] 
```

#####[Method] proc.numFds
```js
// sync
res = proc.numFds()

// callback
proc.numFds(function(err, res) { ... })

// output =>
 12
```

#####[Method] proc.getNice
```js
// sync
res = proc.getNice()

// callback
proc.getNice(function(err, res) { ... })

// output =>
 0
```

#####[Method] proc.setNice
```js
// sync
res = proc.setNice(10) // arguments[0] is nice value

// callback
proc.setNice(10, function(err, res) { ... });

// output =>
     0 // success
    -1 // fail
```

#####[Method] proc.status
```js
// sync
res = proc.status()

// callback
proc.status(function(err, res) { ... })

// output =>
 'running'
```

#####[Method] proc.threads
```js
// sync
res = proc.threads()

// callback
proc.threads(function(err, res) { ... })

// output =>
 [ { idx: 1, 
     user: 0.14695000648498535, 
     sys: 0.02574799954891205 },
   { idx: 2,
     user: 0.00007100000220816582,
     sys: 0.00008600000001024455 },
   { idx: 3,
     user: 0.00002499999936844688,
     sys: 0.00008499999967170879 },
   { idx: 4,
     user: 0.0006169999833218753,
     sys: 0.0019920000340789557 } ]
```

#####[Method] proc.memMaps
```js
// sync
res = proc.memMaps()

// callback
proc.memMaps(function(err, res) { ... })

// output =>
 // OSX
 [
    {
        "pmmap_ext" : "0000000100000000-0000000100617000",
        "addr_perms" : "r-x/rwx",
        "path":"/usr/local/bin/node",
        "rss" : 45056,
        "private" : 0,
        "swapped" : 0,
        "dirtied" : 0,
        "ref_count" : 7,
        "shadow_depth" : 1
    },
    ...]
 // Linux
 [
    {  
        "pmmap_ext": "7f1517342000-7f151734e000",
        "addr_perms": "r-xp",
        "path": "/lib/x86_64-linux-gnu/libnss_files-2.17.so",
        "Size": 49152,
        "Rss": 0, 
        "Pss": 0, 
        "Shared_Clean": 0, 
        "Shared_Dirty": 0, 
        "Private_Clean": 0,
        "Private_Dirty": 0,
        "Referenced": 0,
        "Anonymous": 0,
        "AnonHugePages": 0,
        "Swap": 0,
        "KernelPageSize": 4096,
        "MMUPageSize": 4096,
        "Locked": 0
    },
    ...]
```

#####[Method] proc.ioCounters (Linux only)
```js
// sync
res = proc.ioCounters()

// callback
proc.ioCounters(function(err, res) { ... })

// output=>
 { rcount: 20474, wcount: 14600, rbytes: 2109440, wbytes: 8192 }
```



