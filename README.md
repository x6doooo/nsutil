Nsutil.js
===
Node.js system utilities

[![NPM](https://nodei.co/npm/nsutil.png)](https://nodei.co/npm/nsutil/)

###Summary

* a fork of [psutil](https://code.google.com/p/psutil/), rebuilt with Node.js
* support OSX ( linux support coming soon... )

###Install

    npm install nsutil

###Prebuilt binaries (node v0.10.28)

    OSX 64-bit

###Rebuild (if you need)

```bash
node-gyp configure build
```

###Usage

```js
var ns = require('nsutil')
```

###APIs

####system methods

ns.virtualMemory()
```js
 { total:    4294967296,
   active:   1476833280,
   inactive: 926130176,
   wire:     1203470336,
   free:     237256704,
   avail:    1163386880,
   used:     3606433792 }
```

ns.swapMemory()
```js
 { total:    1073741824,
   used:     6815744,
   free:    1066926080,
   sin:  2724483072,
   sout: 20860928 }    
``` 

ns.cpuTimes()
```js    
 { user: 4294480, nice: 0, sys: 2571340, idle: 61515210 } // ms
``` 

ns.perCpuTimes()
```js
 [ { user: 1741890, nice: 0, sys: 1236480, idle: 14117300 },
   { user: 680530,  nice: 0, sys: 421590,  idle: 15993000 },
   { user: 1290530, nice: 0, sys: 625510,  idle: 15179090 },
   { user: 581530,  nice: 0, sys: 287760,  idle: 16225820 } ] 
``` 

ns.cpuCountLogical()
```js
 4
```

ns.cpuCountPhys()
```js
 2
```

ns.bootTime()
```js
 1400543744000 // timestamp ms
```

ns.diskPartitions()
```js
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

ns.users()
```js
 [
  {"username":"Dx.Yang","tty":"console","host":"","startTime":1400548608},
  {"username":"Dx.Yang","tty":"ttys000","host":"","startTime":1400548608},
  {"username":"Dx.Yang","tty":"ttys001","host":"","startTime":1400548608}
 ]
```

ns.pids()
```js
 [6652,6651,6640,6639,6638,6633,6632,6615,6606...]
```

ns.netConnections()
```js
 [ { fd: 22,
    family: 'AF_INET',
    type: 'SOCK_STREAM',
    laddr: ['X.X.X.X', XXXX],
    raddr: ['X.X.X.X', XXXX],
    state: 'ESTABLISHED' },
    ...]
```

ns.netIOCounters()
```js
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

ns.diskIOCounters()
```js
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


#####Class Process

var proc = ns.Process(pid)

proc.name()
```js
 'node'
```

proc.exe()
```js
 '/usr/local/bin/node'
```

proc.cmdline()
```js
 [ 'node',
  '/usr/local/lib/node_modules/mocha/bin/_mocha',
  'test_osx.js',
  '-R',
  'spec' ]
```

proc.ppid()
```js
 6651  //parent process id    
```

proc.cwd()
```js
 '/Users/node_modules/nsutil/test'
```

proc.uids()
```js
 { real: 501, effective: 501, saved: 501 }
```

proc.gids()
```js
 { real: 20, effective: 20, saved: 20 }
```

proc.terminal()
```js
 /dev/ttys004
```

proc.memoryInfo()   
```js
 { rss: 18440192, vms: 3119169536, faults: 19517440, pageins: 0 }
```

proc.cpuTimes()
```js
 { user: 0.139774113, sys: 0.027113125 }    // s
```

proc.createTime()
```js
 1400565545000 // timestamp ms
```

proc.numCtxSwitches()
```js    
 271
```

proc.numThreads()
```js
 4
```

proc.openFiles()
```js
 [ { path: '/dev/ttys004', fd: 0 },
  { path: '/dev/ttys004', fd: 1 },
  { path: '/dev/ttys004', fd: 2 },
  ... ]
```

proc.connections('inet')    // default is 'all'
```js
 [ { fd: 22,
    family: 'AF_INET',
    type: 'SOCK_STREAM',
    laddr: ['X.X.X.X', XXXX],
    raddr: ['X.X.X.X', XXXX],
    state: 'ESTABLISHED' },
    ...] 
```

proc.numFds()
```js
 12
```

proc.getNice()
```js
 0
```

proc.setNice(niceValue)

proc.status()
```js
 running
```

proc.threads()
```js
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

proc.memMaps()
```js
 [
    {
        "pmmap_ext" : "0000000100000000-0000000100617000",
        "addr_perms" : "r-x/rwx","path":"/usr/local/bin/node",
        "rss" : 45056,
        "private" : 0,
        "swapped" : 0,
        "dirtied" : 0,
        "ref_count" : 7,
        "shadow_depth" : 1
    },
    ...]
```



