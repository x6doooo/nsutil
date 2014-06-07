var CONN = {};

const AF_INET     = CONN.AF_INET     = 2;
const AF_INET6    = CONN.AF_INET6    = 10;
const AF_UNIX     = CONN.AF_UNIX     = 1;
const SOCK_STREAM = CONN.SOCK_STREAM = 1;
const SOCK_DGRAM  = CONN.SOCK_DGRAM  = 2;

const CONN_FAMILY_STR = {
    '1':  'AF_UNIX',
    '2':  'AF_INET',
    '10': 'AF_INET6'
};

const CONN_TYPE_STR = {
    '1': 'SOCK_STREAM',
    '2': 'SOCK_DGRAM'
};

const CONN_FAMILY_AND_TYPE = {
    all:   [ [ AF_INET, AF_INET6, AF_UNIX ], [ SOCK_STREAM, SOCK_DGRAM ] ],
    tcp:   [ [ AF_INET, AF_INET6 ],          [ SOCK_STREAM ] ],
    tcp4:  [ [ AF_INET ],                    [ SOCK_STREAM ] ],
    tcp6:  [ [ AF_INET6 ],                   [ SOCK_STREAM ] ],
    udp:   [ [ AF_INET, AF_INET6 ],          [ SOCK_DGRAM ] ],
    udp4:  [ [ AF_INET ],                    [ SOCK_DGRAM ] ],
    udp6:  [ [ AF_INET6 ],                   [ SOCK_DGRAM ] ],
    inet:  [ [ AF_INET, AF_INET6 ],          [ SOCK_STREAM, SOCK_DGRAM ] ],
    inet4: [ [ AF_INET ],                    [ SOCK_STREAM, SOCK_DGRAM ] ],
    inet6: [ [ AF_INET6 ],                   [ SOCK_STREAM, SOCK_DGRAM ] ],
    unix:  [ [ AF_UNIX ],                    [ SOCK_STREAM, SOCK_DGRAM ] ]
};


const STATUS_RUNNING      = "running";
const STATUS_SLEEPING     = "sleeping";
const STATUS_DISK_SLEEP   = "disk-sleep";
const STATUS_STOPPED      = "stopped";
const STATUS_TRACING_STOP = "tracing-stop";
const STATUS_ZOMBIE       = "zombie";
const STATUS_DEAD         = "dead";
const STATUS_WAKE_KILL    = "wake-kill";
const STATUS_WAKING       = "waking";
const STATUS_IDLE         = "idle";  //# BSD
const STATUS_LOCKED       = "locked";  //# BSD
const STATUS_WAITING      = "waiting";  //# BSD

const CONN_ESTABLISHED = "ESTABLISHED";
const CONN_SYN_SENT    = "SYN_SENT";
const CONN_SYN_RECV    = "SYN_RECV";
const CONN_FIN_WAIT1   = "FIN_WAIT1";
const CONN_FIN_WAIT2   = "FIN_WAIT2";
const CONN_TIME_WAIT   = "TIME_WAIT";
const CONN_CLOSE       = "CLOSE";
const CONN_CLOSE_WAIT  = "CLOSE_WAIT";
const CONN_LAST_ACK    = "LAST_ACK";
const CONN_LISTEN      = "LISTEN";
const CONN_CLOSING     = "CLOSING";
const CONN_NONE        = "NONE";

const TCP_STATUSES = {
    4: CONN_ESTABLISHED,
    2: CONN_SYN_SENT,
    3: CONN_SYN_RECV,
    6: CONN_FIN_WAIT1,
    9: CONN_FIN_WAIT2,
   10: CONN_TIME_WAIT,
    0: CONN_CLOSE,
    5: CONN_CLOSE_WAIT,
    8: CONN_LAST_ACK,
    1: CONN_LISTEN,
    7: CONN_CLOSING,
    none: CONN_NONE
};

const PROC_STATUSES = {
    1: STATUS_IDLE,
    2: STATUS_RUNNING,
    3: STATUS_SLEEPING,
    4: STATUS_STOPPED,
    5: STATUS_ZOMBIE
};

const TCP_STATUSES_LINUX = {
    '01': CONN_ESTABLISHED,
    '02': CONN_SYN_SENT,
    '03': CONN_SYN_RECV,
    '04': CONN_FIN_WAIT1,
    '05': CONN_FIN_WAIT2,
    '06': CONN_TIME_WAIT,
    '07': CONN_CLOSE,
    '08': CONN_CLOSE_WAIT,
    '09': CONN_LAST_ACK,
    '0A': CONN_LISTEN,
    '0B': CONN_CLOSING,
    none: CONN_NONE
};

const PROC_STATUSES_LINUX = {
    R: STATUS_RUNNING,
    S: STATUS_SLEEPING,
    D: STATUS_DISK_SLEEP,
    T: STATUS_STOPPED,
    t: STATUS_TRACING_STOP,
    Z: STATUS_ZOMBIE,
    X: STATUS_DEAD,
    x: STATUS_DEAD,
    K: STATUS_WAKE_KILL,
    W: STATUS_WAKING
};

// 准备废弃
function usagePercent(used, total, round) {

    var percent = used / total * 100;
    
    if (round) {
        percent.toFixed(round);
    }

    return percent;

}

exports.CONN = CONN;
exports.CONN_FAMILY_STR = CONN_FAMILY_STR;
exports.CONN_TYPE_STR = CONN_TYPE_STR;
exports.CONN_FAMILY_AND_TYPE = CONN_FAMILY_AND_TYPE;
exports.TCP_STATUSES = TCP_STATUSES;
exports.PROC_STATUSES = PROC_STATUSES;
exports.TCP_STATUSES_LINUX = TCP_STATUSES_LINUX;
exports.PROC_STATUSES_LINUX = PROC_STATUSES_LINUX;
exports.usagePercent = usagePercent;


