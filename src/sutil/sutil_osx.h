#ifndef _SUTIL_OSX_H_
#define _SUTIL_OSX_H_

#include <assert.h>
#include <limits.h>  // for INT_MAX
#include <signal.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <utmpx.h>
#include <sys/sysctl.h>
#include <sys/vmmeter.h>
#include <libproc.h>
#include <sys/proc_info.h>
#include <netinet/tcp_fsm.h>
#include <arpa/inet.h>
#include <net/if_dl.h>
#include <pwd.h>

#include <mach/mach.h>
#include <mach/task.h>
#include <mach/mach_init.h>
#include <mach/host_info.h>
#include <mach/mach_host.h>
#include <mach/mach_traps.h>
#include <mach/mach_vm.h>
#include <mach/shared_region.h>

#include <mach-o/loader.h>

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/storage/IOBlockStorageDriver.h>
#include <IOKit/storage/IOMedia.h>
#include <IOKit/IOBSD.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "osx/process_info.h"

struct proc_mem_map_grouped {
    std::string pmmap_ext;
    std::string addr_perms;
    std::string path;
    uint16_t rss;
    uint16_t private_v;
    uint16_t swapped;
    uint16_t dirtied;
    uint16_t ref_count;
    short shadow_depth;
};

struct proc_open_file {
    std::string path;
    int fd;
};

static int SUTIL_CONN_NONE = 128;

struct proc_conn {
    int fd = -1;
    int family = -1;
    int type = -1;
    // family => 1: AF_UNIX 
    std::string addr = "";
    std::string caddr = "";
    int conn_none = SUTIL_CONN_NONE;
    // family => 2 || 10 : AF_INET || AF_INET6
    std::string laddr_ip = "";
    int laddr_port = -1;
    std::string raddr_ip = "";
    int raddr_port = -1;
    int state = -1;
};

struct sutil_user_info {
    std::string username;
    std::string tty;
    std::string host;
    float start_time;
};

int sutil_pids(std::vector<int32_t> &pids);
std::string sutil_proc_name(const int32_t &pid);
std::string sutil_proc_cwd(const int32_t &pid);
std::string sutil_proc_exe(const int32_t &pid);
int sutil_proc_cmdline(const int32_t &pid, std::vector<std::string> &proc_cmdline);
int32_t sutil_proc_ppid(const int32_t &pid);
int sutil_proc_uids(const int32_t &pid, int32_t *&uids);
int sutil_proc_gids(const int32_t &pid, int32_t *&group_ids);
int sutil_proc_tty_nr(const int32_t &pid);
int sutil_proc_memory_maps(const int32_t &pid, std::vector<proc_mem_map_grouped> &proc_mem_maps);
int sutil_proc_cpu_times(const int32_t &pid, double *&t);
int32_t sutil_proc_create_time(const int32_t &pid);
int sutil_proc_memory_info(const int32_t &pid, uint64_t *&proc_mem);
uint32_t sutil_proc_num_threads(const int32_t &pid);
uint32_t sutil_proc_num_ctx_switches(const int32_t &pid);
int sutil_proc_status(const int32_t &pid);
int sutil_proc_threads(const int32_t &pid, std::vector<std::vector<float>> &threads);
int sutil_proc_connections(const int32_t &pid,
        const std::vector<int> &af_filter,
        const std::vector<int> &type_filter,
        std::vector<proc_conn> &proc_conn_list);
int sutil_proc_num_fds(const int32_t &pid);

int sutil_cpu_count_logical(void);
int sutil_cpu_count_phys(void);
int sutil_virtual_mem(uint64_t *&vir_mem);
int sutil_swap_mem(uint64_t *&swap_mem);
int sutil_cpu_times(double *&cpu_times);
int sutil_per_cpu_times(std::vector<std::vector<double>> &per_cpu_times);
float sutil_boot_time(void);
int sutil_disk_partitions(std::vector<std::vector<std::string>> &disk_partitions);
int sutil_proc_open_files(const int32_t &pid, std::vector<proc_open_file> &proc_open_files);
int sutil_net_io_counters(std::vector<std::vector<uint64_t>> &net_io_counters);
int sutil_disk_io_counters(std::map<std::string, std::vector<uint64_t>> &disk_io_counters);
int sutil_users(std::vector<sutil_user_info> &user_list);
/*

*/
#endif
