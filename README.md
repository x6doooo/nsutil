Nsutil.js
===
Node.js system utilities

[![NPM](https://nodei.co/npm/nsutil.png)](https://nodei.co/npm/nsutil/)

###Summary

* a fork of [psutil](https://code.google.com/p/psutil/), rebuild with Node.js
* support OSX ( linux support coming soon... )

###Install

    npm install nsutil

###Usage

```js

var ns = require('nsutil')

```

###APIs

```js

ns.virtualMemory()
/*
    =>
    {
      total:     4294967296,
      active:    1476833280,
      inactive:  926130176,
      wire:      1203470336,
      free:      237256704,
      avail:     1163386880,
      used:      3606433792 
    }
 */

```








