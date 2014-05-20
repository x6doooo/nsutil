Nsutil.js
===
Node.js system utilities

[![NPM](https://nodei.co/npm/nsutil.png)](https://nodei.co/npm/nsutil/)

###Summary

* a fork of [psutil](https://code.google.com/p/psutil/), rebuild with Node.js
* support OSX ( linux support coming soon... )

###Install

    npm install nsutil

###Prebuilt binaries (node v0.10.28)

    OSX 64-bit

###Rebuild

```bash
node-gyp clean
node-gyp configure
node-gyp build
```

###Usage

```js
var ns = require('nsutil')
```

###APIs

```js
/**
 *  system methods
 */
ns.virtualMemory()
/* =>
{ total:    4294967296,
  active:   1476833280,
  inactive: 926130176,
  wire:     1203470336,
  free:     237256704,
  avail:    1163386880,
  used:     3606433792 }
 */

ns.swapMemory()
/* =>
{ total:    1073741824,
  used:     6815744,
  avail:    1066926080,
  pageins:  2724483072,
  pageouts: 20860928 }    
 */

ns.cpuTimes()
/* =>
{ user: 4294480, nice: 0, sys: 2571340, idle: 61515210 } // ms
 */

ns.perCpuTimes()
/*
[ { user: 1741890, nice: 0, sys: 1236480, idle: 14117300 },
  { user: 680530,  nice: 0, sys: 421590,  idle: 15993000 },
  { user: 1290530, nice: 0, sys: 625510,  idle: 15179090 },
  { user: 581530,  nice: 0, sys: 287760,  idle: 16225820 } ] 
 */

ns.cpuCountLogical() // => 4

ns.cpuCountPhys() // => 2

ns.bootTime()
/* =>
    1400543744000 // ms
    new Date(1400543744000) // Tue May 20 2014 07:55:44 GMT+0800 (CST)
 */

ns.diskPartitions()
/* =>
[ { device: '/dev/disk0s2',
    mount_type: '/',
    fs_type: 'hfs',
    options: 'rw,local,rootfs,dovolfs,journaled,multilabel' },
  { device: 'devfs',
    mount_type: '/dev',
    fs_type: 'devfs',
    options: 'rw,local,dontbrowse,multilabel' },
  ...
] 
 */

ns.users()
/* =>
[
  {"username":"Dx.Yang","tty":"console","host":"","startTime":1400548608},
  {"username":"Dx.Yang","tty":"ttys000","host":"","startTime":1400548608},
  {"username":"Dx.Yang","tty":"ttys001","host":"","startTime":1400548608}
]
 */

ns.pids()
/* =>
[6652,6651,6640,6639,6638,6633,6632,6615,6606...
 */

ns.netConnections()
/* =>
 [ { fd: 22,
    family: 'AF_INET',
    type: 'SOCK_STREAM',
    laddr_ip: '10.62.9.27',
    laddr_port: 50896,
    raddr_ip: '42.156.152.1',
    raddr_port: 16000,
    state: 'ESTABLISHED' },
    ...
 ]
 */

/**
 *  Class Process
 */

    TODO...

```








