#include "sutil_linux.h"

using namespace std;

#if SUTIL_HAVE_PRLIMIT
    #define _FILE_OFFSET_BITS 64
    #include <time.h>
    #include <sys/resource.h>
#endif


#if SUTIL_HAVE_IOPRIO
enum {
    IOPRIO_WHO_PROCESS = 1,
};

static inline int
ioprio_get(int which, int who)
{
    return syscall(__NR_ioprio_get, which, who);
}

static inline int
ioprio_set(int which, int who, int ioprio)
{
    return syscall(__NR_ioprio_set, which, who, ioprio);
}

#define IOPRIO_CLASS_SHIFT 13
#define IOPRIO_PRIO_MASK ((1UL << IOPRIO_CLASS_SHIFT) - 1)

#define IOPRIO_PRIO_CLASS(mask) ((mask) >> IOPRIO_CLASS_SHIFT)
#define IOPRIO_PRIO_DATA(mask) ((mask) & IOPRIO_PRIO_MASK)
#define IOPRIO_PRIO_VALUE(class, data) (((class) << IOPRIO_CLASS_SHIFT) | data)

/*
 * Return a (ioclass, iodata) Python tuple representing process I/O priority.
 */
int
sutil_proc_ioprio_get(const int32_t &pid, int* &proc_ioprio)
{
    int ioprio, ioclass, iodata;
    ioprio = ioprio_get(IOPRIO_WHO_PROCESS, pid);
    if (ioprio == -1) {
        //PyErr_SetFromErrno(PyExc_OSError);
        return -1;
    }
    ioclass = IOPRIO_PRIO_CLASS(ioprio);
    iodata = IOPRIO_PRIO_DATA(ioprio);
    proc_ioprio[0] = ioclass;
    proc_ioprio[1] = iodata;
    return 0;
}

/*
 * A wrapper around ioprio_set(); sets process I/O priority.
 * ioclass can be either IOPRIO_CLASS_RT, IOPRIO_CLASS_BE, IOPRIO_CLASS_IDLE
 * or 0. iodata goes from 0 to 7 depending on ioclass specified.
 */
int
sutil_proc_ioprio_set(const int32_t &pid, const int &ioclass, const int &iodata)
{
    int ioprio = IOPRIO_PRIO_VALUE(ioclass, iodata);
    int retval = ioprio_set(IOPRIO_WHO_PROCESS, pid, ioprio);
    if (retval == -1) {
        //return PyErr_SetFromErrno(PyExc_OSError);
        return -1;
    }
    return 0;
}


#endif


#if SUTIL_HAVE_PRLIMIT
/*
 * A wrapper around prlimit(2); sets process resource limits.
 * This can be used for both get and set, in which case extra
 * 'soft' and 'hard' args must be provided.
 */
int
sutil_linux_prlimit(const pid_t &pid, 
                    enum __rlimit_resource resource,
                    int64_t *&result,
                    int64_t *soft = nullptr, 
                    int64_t *hard = nullptr)
{
    int ret;
    struct rlimit old, anew;
    struct rlimit *newp = NULL;

    // get
    if (soft == nullptr && hard == nullptr) {
        ret = prlimit(pid, resource, NULL, &old);
        if (ret == -1)
            return -1;
            //return PyErr_SetFromErrno(PyExc_OSError);
#if defined(SUTIL_HAVE_LONG_LONG)
        if (sizeof(old.rlim_cur) > sizeof(long)) {
            result[0] = old.rlim_cur;
            result[1] = old.rlim_max;
            return 0;
        }
#endif
        result[0] = old.rlim_cur;
        result[1] = old.rlim_max;
        return 0;
        //return Py_BuildValue("ll", (long)old.rlim_cur, (long)old.rlim_max);
    }

    // set
    else {
#if defined(SUTIL_HAVE_LARGEFILE_SUPPORT)
        anew.rlim_cur = *soft;
        if (anew.rlim_cur == (rlim_t) - 1)
            return 0;
        anew.rlim_max = *hard; //PyLong_AsLongLong(hard);
        if (anew.rlim_max == (rlim_t) - 1)
            return 0;
#else
        anew.rlim_cur = *soft; //PyLong_AsLong(soft);
        if (anew.rlim_cur == (rlim_t) - 1)
            return 0;
        anew.rlim_max = *hard;//PyLong_AsLong(hard);
        if (anew.rlim_max == (rlim_t) - 1)
            return 0;
#endif
        newp = &anew;
        ret = prlimit(pid, resource, newp, &old);
        if (ret == -1)
            //return PyErr_SetFromErrno(PyExc_OSError);
            return -1;
        return 0;
    }
}
#endif

bool
sutil_pid_exists(const int32_t &pid)
{

    int kill_ret;

    // save some time if it's an invalid PID
    if (pid < 0) {
        return false;
    }

    // if kill returns success of permission denied we know it's a valid PID
    kill_ret = kill(pid , 0);
    if ( (0 == kill_ret) || (EPERM == errno) ) {
        return true;
    }

    // otherwise return 0 for PID not found
    return false;
}



/*
 * return disk mounted partitions as a list of tuples including device,
 * mount point and filesystem type
 */
int
sutil_disk_partitions(vector<vector<string>> &devlist)
{
    FILE *file = NULL;
    struct mntent *entry;
    vector<string> tuple(4);


    // mounted constant comes from mntent.h and it's == '/etc/mtab'
    file = setmntent(MOUNTED, "r");
    if ((file == 0) || (file == NULL)) {
        //pyerr_setfromerrnowithfilename(pyexc_oserror, mounted);
        goto error;
    }

    while ((entry = getmntent(file))) {
        if (entry == NULL) {
            //pyerr_format(pyexc_runtimeerror, "getmntent() failed");
            goto error;
        }
        tuple.clear();
        tuple.push_back(entry->mnt_fsname);  // device
        tuple.push_back(entry->mnt_dir);     // mount point
        tuple.push_back(entry->mnt_type);    // fs type
        tuple.push_back(entry->mnt_opts);   // options
        devlist.push_back(tuple);
    }
    endmntent(file);
    return 0;

error:
    if (file != NULL)
        endmntent(file);
    return -1;
}


/*
 * A wrapper around sysinfo(), return system memory usage statistics.
 */
int
sutil_linux_sysinfo(uint64_t* &info)
{
    struct sysinfo ifo;
    if (sysinfo(&ifo) != 0) {
        //return PyErr_SetFromErrno(PyExc_OSError);
        return -1;
    }
    // note: boot time might also be determined from here
    //return Py_BuildValue(
    //    "(KKKKKK)",
    //
    info[0] = (uint64_t)ifo.totalram  * ifo.mem_unit;   // total
    info[1] = (uint64_t)ifo.freeram   * ifo.mem_unit;   // free
    info[2] = (uint64_t)ifo.bufferram * ifo.mem_unit;   // buffer
    info[3] = (uint64_t)ifo.sharedram * ifo.mem_unit;   // shared
    info[4] = (uint64_t)ifo.totalswap * ifo.mem_unit;   // swap tot
    info[5] = (uint64_t)ifo.freeswap  * ifo.mem_unit;  // swap free
    return 0;
}

/*
 * Return process CPU affinity as a Python long (the bitmask)
 */
int
sutil_proc_cpu_affinity_get(const int32_t pid, uint32_t &mask)
{
    unsigned int len = sizeof(mask);

    if (sched_getaffinity(pid, len, (cpu_set_t *)&mask) < 0) {
        //return PyErr_SetFromErrno(PyExc_OSError);
        return -1;
    }
    //return Py_BuildValue("l", mask);
    return 0;
}

/*
 * Set process CPU affinity; expects a bitmask
 */
int
sutil_proc_cpu_affinity_set(const int32_t &pid, vector<int32_t> &cpu_set_list)
{
    cpu_set_t cpu_set;
    size_t len;
    int i;//, seq_len;

    //if (!PySequence_Check(py_cpu_set)) {
        // does not work on Python 2.4
        // PyErr_Format(PyExc_TypeError, "sequence argument expected, got %s",
        //              Py_TYPE(py_cpu_set)->tp_name);
        //PyErr_Format(PyExc_TypeError, "sequence argument expected");
        //goto error;
    //}

    //py_cpu_seq = PySequence_Fast(py_cpu_set, "expected a sequence or integer");
    //if (!py_cpu_seq) {
    //    goto error;
    //}
    //seq_len = PySequence_Fast_GET_SIZE(py_cpu_seq);
    CPU_ZERO(&cpu_set);
    for (i = 0; i < cpu_set_list.size(); i++) {
        //PyObject *item = PySequence_Fast_GET_ITEM(py_cpu_seq, i);
        int32_t value = cpu_set_list[i];
        if (value == -1) {
            goto error;
        }
        CPU_SET(value, &cpu_set);
    }

    len = sizeof(cpu_set);
    if (sched_setaffinity(pid, len, &cpu_set)) {
        //PyErr_SetFromErrno(PyExc_OSError);
        goto error;
    }

    return 0;

error:
    return -1;
}

/*
 * Return currently connected users as a list of tuples.
 */
int
sutil_users(vector<sutil_user_info> &user_list)
{
    //PyObject *ret_list = PyList_New(0);
    //PyObject *tuple = NULL;
    //PyObject *user_proc = NULL;
    bool user_proc;
    sutil_user_info a_user;
    struct utmp *ut;

    //if (ret_list == NULL)
    //    return NULL;
    setutent();
    while (NULL != (ut = getutent())) {
        if (ut->ut_type == USER_PROCESS)
            user_proc = true;
        else
            user_proc = false;

        //tuple = Py_BuildValue(
        //    "(sssfO)",
        a_user.username = ut->ut_user;              // username
        a_user.tty = ut->ut_line;              // tty
        a_user.host = ut->ut_host;              // hostname
        a_user.start_time = (float)ut->ut_tv.tv_sec;  // tstamp
        a_user.user_proc = user_proc;                 // (bool) user process
        //);
        user_list.push_back(a_user);
    }
    endutent();
    return 0;

error:
    endutent();
    return -1;
}


int sutil_sysconf(std::string &which) 
{
    if (which == "SC_CLK_TCK") {
        return sysconf(_SC_CLK_TCK);
    }
    if (which == "SC_PAGE_SIZE") {
        return sysconf(_SC_PAGE_SIZE);
    }
}


