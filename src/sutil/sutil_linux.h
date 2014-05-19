#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include <errno.h>
#include <stdlib.h>
#include <mntent.h>
#include <features.h>
#include <utmp.h>
#include <sched.h>
#include <linux/version.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <sys/resource.h>

#include <vector>
#include <string>

//enum __rlimit_resource
//{
    /* Maximum amount of available cpu time (in seconds!).  */
//    RLIMIT_CPU = 1,
//#define RLIMIT_CPU RLIMIT_CPU
    /* Maximum size of physical memory the process should get.  */
//    RLIMIT_RSS = 2,
//#define RLIMIT_RSS RLIMIT_RSS
    /* Maximum size of data memory.  */
//    RLIMIT_DATA = 3,
//#define RLIMIT_DATA RLIMIT_DATA
    /* Maximum stack size.  */
//    RLIMIT_STACK = 4,
//#define RLIMIT_STACK RLIMIT_STACK
    /* Maximum file size the process can create.  */
//    RLIMIT_FSIZE = 5,
//#define RLIMIT_FSIZE RLIMIT_FSIZE
    /* Maximum coredump size.  */
//    RLIMIT_CORE = 6,
//#define RLIMIT_CORE RLIMIT_CORE
    /* Maximum number of files the process can open.  */
//    RLIMIT_OPEN_FILES = 7
//#define RLIMIT_OPEN_FILES RLIMIT_OPEN_FILES
//};


// Linux >= 2.6.13
#define SUTIL_HAVE_IOPRIO defined(__NR_ioprio_get) && defined(__NR_ioprio_set)

// Linux >= 2.6.36 (supposedly) and glibc >= 13
#define SUTIL_HAVE_PRLIMIT \
    (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 36)) && \
    (__GLIBC__ >= 2 && __GLIBC_MINOR__ >= 13) && \
    defined(__NR_prlimit64)



struct sutil_user_info {
    std::string username;
    std::string tty;
    std::string host;
    float start_time;
    bool user_proc;
};

int
sutil_proc_ioprio_get(const int32_t &pid, int* &proc_ioprio);

int
sutil_proc_ioprio_set(const int32_t &pid, const int &ioclass, const int &iodata);

int
sutil_linux_prlimit(const pid_t &pid, 
        enum __rlimit_resource resource,
        int64_t *&result,
        int64_t *soft, 
        int64_t *hard);

int
sutil_disk_partitions(std::vector<std::vector<std::string>> &devlist);

int
sutil_linux_sysinfo(uint64_t* &info);

int
sutil_proc_cpu_affinity_get(const int32_t pid, uint32_t &mask);

int
sutil_proc_cpu_affinity_set(const int32_t &pid, std::vector<int32_t> &cpu_set_list);

int
sutil_users(std::vector<sutil_user_info> &user_list);







