#include "sutil_osx.h"

using namespace std;

int
sutil_sys_vminfo(vm_statistics_data_t *vmstat)
{
    kern_return_t ret;
    mach_msg_type_number_t count = sizeof(*vmstat) / sizeof(integer_t);
    mach_port_t mport = mach_host_self();

    ret = host_statistics(mport, HOST_VM_INFO, (host_info_t)vmstat, &count);
    if (ret != KERN_SUCCESS) {
        string err_desc = "RuntimeError: host_statistics() failed: ";
        err_desc += mach_error_string(ret);
        sutil_set_err(err_desc);
        return 0;
    }
    mach_port_deallocate(mach_task_self(), mport);
    return 1;
}


/*
 * Return vector<int> of all the PIDs running on the system.
 */
int
sutil_pids(vector<int32_t> &pids)
{
    kinfo_proc *proclist = NULL;
    kinfo_proc *orig_address = NULL;
    size_t num_processes;
    size_t idx;

    if (sutil_get_proc_list(&proclist, &num_processes) != 0) {
        sutil_set_err("RuntimeError: failed to retrieve process list.");
        goto error;
    }

    if (num_processes > 0) {
        // save the address of proclist so we can free it later
        orig_address = proclist;
        for (idx = 0; idx < num_processes; idx++) {
            pids.push_back(proclist->kp_proc.p_pid);
            proclist++;
        }
        free(orig_address);
    }
    return 0;

error:
    if (orig_address != NULL)
        free(orig_address);
    return -1;
}


/*
 * Return process name from kinfo_proc as a string.
 */
string
sutil_proc_name(const int32_t &pid)
{
    struct kinfo_proc kp;
    if (sutil_get_kinfo_proc(pid, &kp) == -1) {
        return "";
    }
    return kp.kp_proc.p_comm;
}

/*
 * Return process current working directory as a string.
 */
string
sutil_proc_cwd(const int32_t &pid)
{
    struct proc_vnodepathinfo pathinfo;

    if (! sutil_proc_pidinfo(pid, PROC_PIDVNODEPATHINFO, &pathinfo,
                              sizeof(pathinfo)))
    {
        return "";
    }
    return pathinfo.pvi_cdir.vip_path;
}

/*
 * Return path of the process executable.
 */
string
sutil_proc_exe(const int32_t &pid)
{
    char buf[PATH_MAX];
    int ret;

    ret = proc_pidpath(pid, &buf, sizeof(buf));
    if (ret == 0) {
        if (!sutil_pid_exists(pid)) {
            NoSuchProcess();
            return "";
        }
        else {
            AccessDenied();
            return "";
        }
    }
    return buf;
}


/*
 * proc_cmdline => process cmdline arguments
 */
int
sutil_proc_cmdline(const int32_t &pid, vector<string> &proc_cmdline)
{
    // get the commandline, defined in arch/osx/process_info.c
    return sutil_get_arg_list(pid, proc_cmdline);
}


/*
 * Return process parent pid from kinfo_proc as a integer.
 */
int32_t
sutil_proc_ppid(const int32_t &pid)
{
    struct kinfo_proc kp;
    if (sutil_get_kinfo_proc(pid, &kp) == -1) {
        return -1;
    }
    return (int32_t)kp.kp_eproc.e_ppid;
}

/*
 * Return process real uid from kinfo_proc as a integer.
 */
int
sutil_proc_uids(const int32_t &pid, int32_t *&uids)
{
    struct kinfo_proc kp;
    if (sutil_get_kinfo_proc(pid, &kp) == -1) {
        return -1;
    }
    uids[0] = (long)kp.kp_eproc.e_pcred.p_ruid;
    uids[1] = (long)kp.kp_eproc.e_ucred.cr_uid;
    uids[2] = (long)kp.kp_eproc.e_pcred.p_svuid;
    return 0;
}

/*
 * Return process real group id from ki_comm as a integer.
 */
int sutil_proc_gids(const int32_t &pid, int32_t *&group_ids)
{
    struct kinfo_proc kp;
    if (sutil_get_kinfo_proc(pid, &kp) == -1) {
        return -1;
    }
    group_ids[0] = (long)kp.kp_eproc.e_pcred.p_rgid;
    group_ids[1] = (long)kp.kp_eproc.e_ucred.cr_groups[0];
    group_ids[2] = (long)kp.kp_eproc.e_pcred.p_svgid;
    return 0;
}

/*
 * Return process controlling terminal number as an integer.
 */
int 
sutil_proc_tty_nr(const int32_t &pid)
{
    struct kinfo_proc kp;
    if (sutil_get_kinfo_proc(pid, &kp) == -1) {
        return -1;
    }
    return kp.kp_eproc.e_tdev;
}

/*
 * Return a list for every process memory maps.
 * 'procstat' cmdline utility has been used as an example.
 */
int 
sutil_proc_memory_maps(const int32_t &pid, vector<proc_mem_map_grouped> &proc_mem_maps)
{
    char buf[PATH_MAX];
    char addr_str[34];
    char perms[8];
    int pagesize = getpagesize();
    kern_return_t err = KERN_SUCCESS;
    mach_port_t task = MACH_PORT_NULL;
    uint32_t depth = 1;
    vm_address_t address = 0;
    vm_size_t size = 0;

    proc_mem_map_grouped mmap;

    err = task_for_pid(mach_task_self(), pid, &task);

    if (err != KERN_SUCCESS) {
        if (!sutil_pid_exists(pid)) {
            NoSuchProcess();
        }
        else {
            // pid exists, so return AccessDenied error since task_for_pid()
            // failed
            AccessDenied();
        }
        goto error;
    }

    while (1) {
        struct vm_region_submap_info_64 info;
        mach_msg_type_number_t count = VM_REGION_SUBMAP_INFO_COUNT_64;

        err = vm_region_recurse_64(task, &address, &size, &depth,
                                   (vm_region_info_64_t)&info, &count);

        if (err == KERN_INVALID_ADDRESS) {
            break;
        }

        if (info.is_submap) {
            depth++;
        }
        else {
            // Free/Reset the char[]s to avoid weird paths
            memset(buf, 0, sizeof(buf));
            memset(addr_str, 0, sizeof(addr_str));
            memset(perms, 0, sizeof(perms));

            sprintf(addr_str, "%016lx-%016lx", address, address + size);
            sprintf(perms, "%c%c%c/%c%c%c",
                    (info.protection & VM_PROT_READ) ? 'r' : '-',
                    (info.protection & VM_PROT_WRITE) ? 'w' : '-',
                    (info.protection & VM_PROT_EXECUTE) ? 'x' : '-',
                    (info.max_protection & VM_PROT_READ) ? 'r' : '-',
                    (info.max_protection & VM_PROT_WRITE) ? 'w' : '-',
                    (info.max_protection & VM_PROT_EXECUTE) ? 'x' : '-');

            err = proc_regionfilename(pid, address, buf, sizeof(buf));

            if (info.share_mode == SM_COW && info.ref_count == 1) {
                // Treat single reference SM_COW as SM_PRIVATE
                info.share_mode = SM_PRIVATE;
            }

            if (strlen(buf) == 0) {
                switch (info.share_mode) {
                /*
                case SM_LARGE_PAGE:
                    // Treat SM_LARGE_PAGE the same as SM_PRIVATE
                    // since they are not shareable and are wired.
                */
                case SM_COW:
                    strcpy(buf, "[cow]");
                    break;
                case SM_PRIVATE:
                    strcpy(buf, "[prv]");
                    break;
                case SM_EMPTY:
                    strcpy(buf, "[nul]");
                    break;
                case SM_SHARED:
                case SM_TRUESHARED:
                    strcpy(buf, "[shm]");
                    break;
                case SM_PRIVATE_ALIASED:
                    strcpy(buf, "[ali]");
                    break;
                case SM_SHARED_ALIASED:
                    strcpy(buf, "[s/a]");
                    break;
                default:
                    strcpy(buf, "[???]");
                }
            }

            mmap.pmmap_ext = addr_str;
            mmap.addr_perms = perms;
            mmap.path = buf;
            mmap.rss = info.pages_resident * pagesize;
            mmap.private_v = info.pages_shared_now_private * pagesize;
            mmap.swapped = info.pages_swapped_out * pagesize;
            mmap.dirtied = info.pages_dirtied * pagesize;
            mmap.ref_count = info.ref_count;
            mmap.shadow_depth = info.shadow_depth;
            proc_mem_maps.push_back(mmap);
        }

        // increment address for the next map/file
        address += size;
    }

    if (task != MACH_PORT_NULL)
        mach_port_deallocate(mach_task_self(), task);

    return 0;

error:
    if (task != MACH_PORT_NULL)
        mach_port_deallocate(mach_task_self(), task);
    return -1;
}

/*
 * Return {user_time, kernel_time} (unit: s)
 */
int sutil_proc_cpu_times(const int32_t &pid, double *&t)
{
    struct proc_taskinfo pti;
    if (! sutil_proc_pidinfo(pid, PROC_PIDTASKINFO, &pti, sizeof(pti))) {
        return -1;
    }
    
    t[0] = (double)pti.pti_total_user / 1000000000.0;
    t[1] = (double)pti.pti_total_system / 1000000000.0;
    return 0;
}

#define TV2DOUBLE(t)    ((t).tv_sec + (t).tv_usec / 1000000.0)
/*
 * Return number the process create time expressed in
 * seconds since the epoch.
 */
int32_t sutil_proc_create_time(const int32_t &pid)
{
    struct kinfo_proc kp;
    if (sutil_get_kinfo_proc(pid, &kp) == -1) {
        return -1;
    }
    return TV2DOUBLE(kp.kp_proc.p_starttime);
}

/*
 * Return extended memory info about a process.
 */
int sutil_proc_memory_info(const int32_t &pid, uint64_t *&proc_mem)
{
    struct proc_taskinfo pti;
    if (! sutil_proc_pidinfo(pid, PROC_PIDTASKINFO, &pti, sizeof(pti))) {
        return -1;
    }
    int pagesize = getpagesize();

    // Note: determining other memory stats on OSX is a mess:
    // http://www.opensource.apple.com/source/top/top-67/libtop.c?txt
    // I just give up... XXX
    // struct proc_regioninfo pri;
    // psutil_proc_pidinfo(pid, PROC_PIDREGIONINFO, &pri, sizeof(pri))
    proc_mem[0] = pti.pti_resident_size;  // resident memory size (rss)
    proc_mem[1] = pti.pti_virtual_size;  // virtual memory size (vms)
    proc_mem[2] = pti.pti_faults * pagesize;         // number of page faults (pages)
    proc_mem[3] = pti.pti_pageins * pagesize;         // number of actual pageins (pages)
    return 0;
}

/*
 * Return number of threads used by process as a integer.
 */
uint32_t sutil_proc_num_threads(const int32_t &pid)
{
    struct proc_taskinfo pti;
    if (! sutil_proc_pidinfo(pid, PROC_PIDTASKINFO, &pti, sizeof(pti))) {
        return 0;
    }
    return pti.pti_threadnum;
}

/*
 * Return the number of context switches performed by process.
 */
uint32_t sutil_proc_num_ctx_switches(const int32_t &pid)
{
    struct proc_taskinfo pti;
    if (! sutil_proc_pidinfo(pid, PROC_PIDTASKINFO, &pti, sizeof(pti))) {
        return 0;
    }
    // unvoluntary value seems not to be available;
    // pti.pti_csw probably refers to the sum of the two (getrusage()
    // numbers seems to confirm this theory).
    return pti.pti_csw;
}

/*
 * Return process status as a integer.
 */
int sutil_proc_status(const int32_t &pid)
{
    struct kinfo_proc kp;
    if (sutil_get_kinfo_proc(pid, &kp) == -1) {
        return -1;
    }
    return (int)kp.kp_proc.p_stat;
}

/*
 * Return process threads
 */
int sutil_proc_threads(const int32_t &pid, vector<vector<float>> &threads)
{
    int err, ret;
    kern_return_t kr;
    unsigned int info_count = TASK_BASIC_INFO_COUNT;
    mach_port_t task = MACH_PORT_NULL;
    struct task_basic_info tasks_info;
    thread_act_port_array_t thread_list = NULL;
    thread_info_data_t thinfo_basic;
    thread_basic_info_t basic_info_th;
    mach_msg_type_number_t thread_count, thread_info_count;

    vector<float> thr(3);

    // task_for_pid() requires special privileges
    err = task_for_pid(mach_task_self(), pid, &task);
    if (err != KERN_SUCCESS) {
        if (! sutil_pid_exists(pid)) {
            NoSuchProcess();
        }
        else {
            AccessDenied();
        }
        goto error;
    }

    info_count = TASK_BASIC_INFO_COUNT;
    err = task_info(task, TASK_BASIC_INFO, (task_info_t)&tasks_info,
                    &info_count);
    if (err != KERN_SUCCESS) {
        // errcode 4 is "invalid argument" (access denied)
        if (err == 4) {
            AccessDenied();
        }
        else {
            // otherwise throw a runtime error with appropriate error code
            sutil_set_err("RuntimeError: task_info(TASK_BASIC_INFO) failed");
        }
        goto error;
    }

    err = task_threads(task, &thread_list, &thread_count);
    if (err != KERN_SUCCESS) {
        sutil_set_err("RuntimeError: task_threads() failed");
        goto error;
    }
    
    uint16_t j;
    for (j = 0; j < thread_count; j++) {
        thr.clear();
        thread_info_count = THREAD_INFO_MAX;
        kr = thread_info(thread_list[j], THREAD_BASIC_INFO,
                         (thread_info_t)thinfo_basic, &thread_info_count);
        if (kr != KERN_SUCCESS) {
            sutil_set_err("RuntimeError: thread_info() with flag THREAD_BASIC_INFO failed");
            goto error;
        }

        basic_info_th = (thread_basic_info_t)thinfo_basic;
        thr.push_back((float)(j + 1));
        thr.push_back((float)basic_info_th->user_time.microseconds / 1000000.0);
        thr.push_back((float)basic_info_th->system_time.microseconds / 1000000.0);
        threads.push_back(thr);
    }

    ret = vm_deallocate(task, (vm_address_t)thread_list,
                        thread_count * sizeof(int));
    if (ret != KERN_SUCCESS) {
        sutil_set_err("RuntimeWarning: vm_deallocate() failed");
    }

    mach_port_deallocate(mach_task_self(), task);

    return 0;

error:
    if (task != MACH_PORT_NULL)
        mach_port_deallocate(mach_task_self(), task);
    if (thread_list != NULL) {
        ret = vm_deallocate(task, (vm_address_t)thread_list,
                            thread_count * sizeof(int));
        if (ret != KERN_SUCCESS) {
            sutil_set_err("RuntimeWarning: vm_deallocate() failed");
        }
    }
    return -1;
}

/*
 * Return process open files
 * References:
 * - lsof source code: http://goo.gl/SYW79 and http://goo.gl/m78fd
 * - /usr/include/sys/proc_info.h
 */
int
sutil_proc_open_files(const int32_t &pid, vector<proc_open_file> &proc_open_files)
{
    int pidinfo_result;
    int iterations;
    int i;
    int nb;

    struct proc_fdinfo *fds_pointer = NULL;
    struct proc_fdinfo *fdp_pointer;
    struct vnode_fdinfowithpath vi;

    proc_open_file opfs;

    pidinfo_result = proc_pidinfo(pid, PROC_PIDLISTFDS, 0, NULL, 0);
    if (pidinfo_result <= 0) {
        // may be be ignored later if errno != 0
        sutil_set_err("RuntimeError: proc_pidinfo(PROC_PIDLISTFDS) failed");
        goto error;
    }

    fds_pointer = (struct proc_fdinfo*)malloc(pidinfo_result);
    if (fds_pointer == NULL) {
        sutil_set_err("error: no memory");
        goto error;
    }
    pidinfo_result = proc_pidinfo(pid, PROC_PIDLISTFDS, 0, fds_pointer,
                                  pidinfo_result);
    if (pidinfo_result <= 0) {
        // may be be ignored later if errno != 0
        sutil_set_err("RuntimeError: proc_pidinfo(PROC_PIDLISTFDS) failed");
        goto error;
    }

    iterations = (pidinfo_result / PROC_PIDLISTFD_SIZE);

    for (i = 0; i < iterations; i++) {
        fdp_pointer = &fds_pointer[i];

        if (fdp_pointer->proc_fdtype == PROX_FDTYPE_VNODE)
        {
            nb = proc_pidfdinfo(pid,
                                fdp_pointer->proc_fd,
                                PROC_PIDFDVNODEPATHINFO,
                                &vi,
                                sizeof(vi));

            // --- errors checking
            if (nb <= 0) {
                if ((errno == ENOENT) || (errno == EBADF)) {
                    // no such file or directory or bad file descriptor;
                    // let's assume the file has been closed or removed
                    continue;
                }
                // may be be ignored later if errno != 0
                sutil_set_err("RuntimeError: proc_pidinfo(PROC_PIDFDVNODEPATHINFO) failed");
                goto error;
            }
            if (nb < sizeof(vi)) {
                sutil_set_err("RuntimeError: proc_pidinfo(PROC_PIDFDVNODEPATHINFO) failed (buffer mismatch)");
                goto error;
            }
            // --- /errors checking

            opfs.path = vi.pvip.vip_path;
            opfs.fd = (int)fdp_pointer->proc_fd;
            proc_open_files.push_back(opfs);
        }
    }

    free(fds_pointer);
    return 0;

error:
    if (fds_pointer != NULL) {
        free(fds_pointer);
    }
    if (errno != 0) {
        sutil_set_err("os error");
    }
    else if (! sutil_pid_exists(pid)) {
        NoSuchProcess();
    }
        // exception has already been set earlier
    return -1;
}

/*
 * Return number of file descriptors opened by process.
 */
int
sutil_proc_num_fds(const int32_t &pid)
{
    int pidinfo_result;
    int num;
    struct proc_fdinfo *fds_pointer;

    pidinfo_result = proc_pidinfo(pid, PROC_PIDLISTFDS, 0, NULL, 0);
    if (pidinfo_result <= 0) {
        sutil_set_err("os error");
        return -1;
    }

    fds_pointer = (struct proc_fdinfo*)malloc(pidinfo_result);
    if (fds_pointer == NULL) {
        sutil_set_err("error: no memory");
        return -1;
    }
    pidinfo_result = proc_pidinfo(pid, PROC_PIDLISTFDS, 0, fds_pointer,
                                  pidinfo_result);
    if (pidinfo_result <= 0) {
        free(fds_pointer);
        sutil_set_err("os error");
        return -1;
    }

    num = (pidinfo_result / PROC_PIDLISTFD_SIZE);
    free(fds_pointer);
    return num;
}

//XXX//////////////////// system ///////////////////////XXX

/*
 * Return the number of logical CPUs in the system.
 * XXX this could be shared with BSD.
 */
int sutil_cpu_count_logical(void)
{
    int mib[2];
    int ncpu;
    size_t len;

    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    len = sizeof(ncpu);

    if (sysctl(mib, 2, &ncpu, &len, NULL, 0) == -1) {
        return -1;
    }
    return ncpu;
}

/*
 * Return the number of physical CPUs in the system.
 */
int sutil_cpu_count_phys(void)
{
    int num;
    size_t size = sizeof(int);
    if (sysctlbyname("hw.physicalcpu", &num, &size, NULL, 0)) {
        return -1;
    }
    return num;
}

/*
 * Return system virtual memory stats
 */
int sutil_virtual_mem(uint64_t *&vir_mem)
{

    int      mib[2];
    uint64_t total;
    size_t   len = sizeof(total);
    vm_statistics_data_t vm;
    int pagesize = getpagesize();

    // physical mem
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    if (sysctl(mib, 2, &total, &len, NULL, 0)) {
        if (errno != 0)
            sutil_set_err("os error");
        else
            sutil_set_err("RuntimeError: sysctl(HW_MEMSIZE) failed");
        return -1;
    }

    // vm
    if (!sutil_sys_vminfo(&vm)) {
        return -1;
    }

    vir_mem[0] = total;
    vir_mem[1] = (uint64_t) vm.active_count * pagesize;
    vir_mem[2] = (uint64_t) vm.inactive_count * pagesize;
    vir_mem[3] = (uint64_t) vm.wire_count * pagesize;
    vir_mem[4] = (uint64_t) vm.free_count * pagesize;
    return 0;
}

/*
 * Return stats about swap memory.
 */
int sutil_swap_mem(uint64_t *&swap_mem)
{
    int mib[2];
    size_t size;
    struct xsw_usage totals;
    vm_statistics_data_t vmstat;
    int pagesize = getpagesize();

    mib[0] = CTL_VM;
    mib[1] = VM_SWAPUSAGE;
    size = sizeof(totals);
    if (sysctl(mib, 2, &totals, &size, NULL, 0) == -1) {
        if (errno != 0)
            sutil_set_err("os error");
        else
            sutil_set_err("RuntimeError: sysctl(VM_SWAPUSAGE) failed");
        return -1;
    }
    if (!sutil_sys_vminfo(&vmstat)) {
        return -1;
    }

    swap_mem[0] = totals.xsu_total;
    swap_mem[1] = totals.xsu_used;
    swap_mem[2] = totals.xsu_avail;
    swap_mem[3] = (unsigned long long)vmstat.pageins * pagesize;
    swap_mem[4] = (unsigned long long)vmstat.pageouts * pagesize;
    return 0;
}


/*
 * Return user, kernel and idle CPU times (unit: s)
 */
int sutil_cpu_times(double *&cpu_times)
{
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    kern_return_t error;
    host_cpu_load_info_data_t r_load;

    mach_port_t host_port = mach_host_self();
    error = host_statistics(host_port, HOST_CPU_LOAD_INFO,
                            (host_info_t)&r_load, &count);
    if (error != KERN_SUCCESS) {
        sutil_set_err("RuntimeError: Error in host_statistics(): " + string(mach_error_string(error)));
        return -1;
    }
    mach_port_deallocate(mach_task_self(), host_port);

    cpu_times[0] = (double)r_load.cpu_ticks[CPU_STATE_USER] / CLK_TCK;
    cpu_times[1] = (double)r_load.cpu_ticks[CPU_STATE_NICE] / CLK_TCK;
    cpu_times[2] = (double)r_load.cpu_ticks[CPU_STATE_SYSTEM] / CLK_TCK;
    cpu_times[3] = (double)r_load.cpu_ticks[CPU_STATE_IDLE] / CLK_TCK;
    return 0;
}

/*
 * Return per-cpu times
 */
int sutil_per_cpu_times(vector<vector<double>> &per_cpu_times)
{
    natural_t cpu_count;
    processor_info_array_t info_array;
    mach_msg_type_number_t info_count;
    kern_return_t error;
    processor_cpu_load_info_data_t *cpu_load_info = NULL;
    int i, ret;
    vector<double> a_cpu(4);

    mach_port_t host_port = mach_host_self();
    error = host_processor_info(host_port, PROCESSOR_CPU_LOAD_INFO,
                                &cpu_count, &info_array, &info_count);
    if (error != KERN_SUCCESS) {
        sutil_set_err("RuntimeError: Error in host_processor_info(): " + string(mach_error_string(error)));
        goto error;
    }
    mach_port_deallocate(mach_task_self(), host_port);

    cpu_load_info = (processor_cpu_load_info_data_t *) info_array;

    for (i = 0; i < cpu_count; i++) {
        a_cpu.clear();

        a_cpu.push_back((double)cpu_load_info[i].cpu_ticks[CPU_STATE_USER] / CLK_TCK);
        a_cpu.push_back((double)cpu_load_info[i].cpu_ticks[CPU_STATE_NICE] / CLK_TCK);
        a_cpu.push_back((double)cpu_load_info[i].cpu_ticks[CPU_STATE_SYSTEM] / CLK_TCK);
        a_cpu.push_back((double)cpu_load_info[i].cpu_ticks[CPU_STATE_IDLE] / CLK_TCK);
        per_cpu_times.push_back(a_cpu);
    }

    ret = vm_deallocate(mach_task_self(), (vm_address_t)info_array,
                        info_count * sizeof(int));
    if (ret != KERN_SUCCESS) {
        sutil_set_err("RuntimeWarning vm_deallocate() failed");
    }
    return 0;

error:
    if (cpu_load_info != NULL) {
        ret = vm_deallocate(mach_task_self(), (vm_address_t)info_array,
                            info_count * sizeof(int));
        if (ret != KERN_SUCCESS) {
            sutil_set_err("RuntimeWarning: vm_deallocate() failed");
        }
    }
    return -1;
}


/*
 * Return a float indicating the system boot time expressed in
 * seconds since the epoch.
 */
float sutil_boot_time(void)
{
    // fetch sysctl "kern.boottime"
    static int request[2] = { CTL_KERN, KERN_BOOTTIME };
    struct timeval result;
    size_t result_len = sizeof result;
    time_t boot_time = 0;

    if (sysctl(request, 2, &result, &result_len, NULL, 0) == -1) {
        sutil_set_err("os error");
        return -1;
    }
    boot_time = result.tv_sec;
    return (float)boot_time;
}

/*
 * Return a list of tuples including device, mount point and fs type
 * for all partitions mounted on the system.
 */
int sutil_disk_partitions(vector<vector<string>> &disk_partitions)
{
    int num;
    int i;
    long len;
    uint64_t flags;
    char opts[400];
    struct statfs *fs = NULL;
    vector<string> dp(4);

    // get the number of mount points
    num = getfsstat(NULL, 0, MNT_NOWAIT);
    if (num == -1) {
        sutil_set_err("os error");
        goto error;
    }

    len = sizeof(*fs) * num;
    fs = (struct statfs*)malloc(len);
    if (fs == NULL) {
        sutil_set_err("error: no memory");
        goto error;
    }

    num = getfsstat(fs, len, MNT_NOWAIT);
    if (num == -1) {
        sutil_set_err("os error");
        goto error;
    }

    for (i = 0; i < num; i++) {
        opts[0] = 0;
        flags = fs[i].f_flags;

        // see sys/mount.h
        if (flags & MNT_RDONLY)
            strlcat(opts, "ro", sizeof(opts));
        else
            strlcat(opts, "rw", sizeof(opts));
        if (flags & MNT_SYNCHRONOUS)
            strlcat(opts, ",sync", sizeof(opts));
        if (flags & MNT_NOEXEC)
            strlcat(opts, ",noexec", sizeof(opts));
        if (flags & MNT_NOSUID)
            strlcat(opts, ",nosuid", sizeof(opts));
        if (flags & MNT_UNION)
            strlcat(opts, ",union", sizeof(opts));
        if (flags & MNT_ASYNC)
            strlcat(opts, ",async", sizeof(opts));
        if (flags & MNT_EXPORTED)
            strlcat(opts, ",exported", sizeof(opts));
        if (flags & MNT_QUARANTINE)
            strlcat(opts, ",quarantine", sizeof(opts));
        if (flags & MNT_LOCAL)
            strlcat(opts, ",local", sizeof(opts));
        if (flags & MNT_QUOTA)
            strlcat(opts, ",quota", sizeof(opts));
        if (flags & MNT_ROOTFS)
            strlcat(opts, ",rootfs", sizeof(opts));
        if (flags & MNT_DOVOLFS)
            strlcat(opts, ",dovolfs", sizeof(opts));
        if (flags & MNT_DONTBROWSE)
            strlcat(opts, ",dontbrowse", sizeof(opts));
        if (flags & MNT_IGNORE_OWNERSHIP)
            strlcat(opts, ",ignore-ownership", sizeof(opts));
        if (flags & MNT_AUTOMOUNTED)
            strlcat(opts, ",automounted", sizeof(opts));
        if (flags & MNT_JOURNALED)
            strlcat(opts, ",journaled", sizeof(opts));
        if (flags & MNT_NOUSERXATTR)
            strlcat(opts, ",nouserxattr", sizeof(opts));
        if (flags & MNT_DEFWRITE)
            strlcat(opts, ",defwrite", sizeof(opts));
        if (flags & MNT_MULTILABEL)
            strlcat(opts, ",multilabel", sizeof(opts));
        if (flags & MNT_NOATIME)
            strlcat(opts, ",noatime", sizeof(opts));
        if (flags & MNT_UPDATE)
            strlcat(opts, ",update", sizeof(opts));
        if (flags & MNT_RELOAD)
            strlcat(opts, ",reload", sizeof(opts));
        if (flags & MNT_FORCE)
            strlcat(opts, ",force", sizeof(opts));
        if (flags & MNT_CMDFLAGS)
            strlcat(opts, ",cmdflags", sizeof(opts));

        dp.clear();
        dp.push_back(fs[i].f_mntfromname); // device
        dp.push_back(fs[i].f_mntonname); // mount point
        dp.push_back(fs[i].f_fstypename); // fs type
        dp.push_back(opts); // options

        disk_partitions.push_back(dp);
    }

    free(fs);
    return 0;

error:
    if (fs != NULL)
        free(fs);
    return -1;
}


// a signaler for connections without an actual status

/*
 * Return process TCP and UDP connections as a list.
 * References:
 * - lsof source code: http://goo.gl/SYW79 and http://goo.gl/wNrC0
 * - /usr/include/sys/proc_info.h
 */
int
sutil_proc_connections(const int32_t &pid,
        const vector<int> &af_filter,
        const vector<int> &type_filter,
        vector<proc_conn> &proc_conn_list)
{
    int pidinfo_result;
    int iterations;
    int i;
    int nb;

    struct proc_fdinfo *fds_pointer = NULL;
    struct proc_fdinfo *fdp_pointer;
    struct socket_fdinfo si;

    struct proc_conn *pcn = nullptr;

    if (pid == 0) {
        return 0;
    }

    pidinfo_result = proc_pidinfo(pid, PROC_PIDLISTFDS, 0, NULL, 0);
    if (pidinfo_result <= 0) {
        goto error;
    }

    fds_pointer = (struct proc_fdinfo*)malloc(pidinfo_result);
    if (fds_pointer == NULL) {
        sutil_set_err("Error: no memory");
        goto error;
    }
    pidinfo_result = proc_pidinfo(pid, PROC_PIDLISTFDS, 0, fds_pointer,
                                  pidinfo_result);

    if (pidinfo_result <= 0) {
        goto error;
    }

    iterations = (pidinfo_result / PROC_PIDLISTFD_SIZE);

    for (i = 0; i < iterations; i++) {
        pcn = new proc_conn;
        errno = 0;
        fdp_pointer = &fds_pointer[i];

        if (fdp_pointer->proc_fdtype == PROX_FDTYPE_SOCKET)
        {
            nb = proc_pidfdinfo(pid, fdp_pointer->proc_fd,
                                PROC_PIDFDSOCKETINFO, &si, sizeof(si));

            // --- errors checking
            if (nb <= 0) {
                if (errno == EBADF) {
                    // let's assume socket has been closed
                    continue;
                }
                if (errno != 0) {
                    sutil_set_err("os error");
                }
                else {
                    sutil_set_err("RuntimeError: proc_pidinfo(PROC_PIDFDVNODEPATHINFO) failed");
                }
                goto error;
            }
            if (nb < sizeof(si)) {
                sutil_set_err("RuntimeError: proc_pidinfo(PROC_PIDFDVNODEPATHINFO) failed (buffer mismatch)");
                goto error;
            }
            // --- /errors checking

            //
            int fd, family, type, lport, rport, state;
            char lip[200], rip[200];
            int inseq;

            fd = (int)fdp_pointer->proc_fd;
            family = si.psi.soi_family;
            type = si.psi.soi_type;

            inseq = find(af_filter.begin(), af_filter.end(), family) != af_filter.end(); 

            if (inseq == 0) {
                continue;
            }
            inseq = find(type_filter.begin(), type_filter.end(), type) != type_filter.end(); 
            if (inseq == 0) {
                continue;
            }

            if (errno != 0) {
                sutil_set_err("os error");
                goto error;
            }

            if ((family == AF_INET) || (family == AF_INET6)) {
                if (family == AF_INET) {
                    inet_ntop(AF_INET,
                              &si.psi.soi_proto.pri_tcp.tcpsi_ini. \
                                  insi_laddr.ina_46.i46a_addr4,
                              lip,
                              sizeof(lip));
                    inet_ntop(AF_INET,
                              &si.psi.soi_proto.pri_tcp.tcpsi_ini.insi_faddr. \
                                  ina_46.i46a_addr4,
                              rip,
                              sizeof(rip));
                }
                else {
                    inet_ntop(AF_INET6,
                              &si.psi.soi_proto.pri_tcp.tcpsi_ini. \
                                  insi_laddr.ina_6,
                              lip, sizeof(lip));
                    inet_ntop(AF_INET6,
                              &si.psi.soi_proto.pri_tcp.tcpsi_ini. \
                                  insi_faddr.ina_6,
                              rip, sizeof(rip));
                }

                // check for inet_ntop failures
                if (errno != 0) {
                    sutil_set_err("os error");
                    goto error;
                }

                lport = ntohs(si.psi.soi_proto.pri_tcp.tcpsi_ini.insi_lport);
                rport = ntohs(si.psi.soi_proto.pri_tcp.tcpsi_ini.insi_fport);
                if (type == SOCK_STREAM) {
                    state = (int)si.psi.soi_proto.pri_tcp.tcpsi_state;
                }
                else {
                    state = SUTIL_CONN_NONE;
                }

                pcn->fd = fd;
                pcn->family = family;
                pcn->type = type;
                pcn->laddr_ip = lip;
                pcn->laddr_port = lport;
                if (rport != 0) {
                    pcn->raddr_ip = rip;
                    pcn->raddr_port = rport;
                }
                pcn->state = state;
                proc_conn_list.push_back(*pcn);
            }
            else if (family == AF_UNIX) {
                // construct the python list
                pcn->fd = fd;
                pcn->family = family;
                pcn->type = type;
                pcn->addr = si.psi.soi_proto.pri_un.unsi_addr.ua_sun.sun_path;
                pcn->caddr = si.psi.soi_proto.pri_un.unsi_caddr.ua_sun.sun_path;
                proc_conn_list.push_back(*pcn);
            }
        }
        delete pcn;
        pcn = nullptr;
    }

    free(fds_pointer);
    return 0;

error:

    if (fds_pointer != NULL) {
        free(fds_pointer);
    }
    if (pcn != nullptr) {
        delete pcn;
        pcn = nullptr;
    }
    if (errno != 0) {
        sutil_set_err("os error");
    }
    else if (! sutil_pid_exists(pid) ) {
        NoSuchProcess();
    }
    else {
        sutil_set_err("RuntimeError: proc_pidinfo(PROC_PIDLISTFDS) failed");
    }
    return -1;
}



/*
 * Return overall network I/O information
 */
int
sutil_net_io_counters(map<string, vector<uint64_t>> &net_io_counters)
{
    char *buf = NULL, *lim, *next;
    struct if_msghdr *ifm;
    int mib[6];
    size_t len;

    vector<uint64_t> nic_info(7);

    mib[0] = CTL_NET;          // networking subsystem
    mib[1] = PF_ROUTE;         // type of information
    mib[2] = 0;                // protocol (IPPROTO_xxx)
    mib[3] = 0;                // address family
    mib[4] = NET_RT_IFLIST2;   // operation
    mib[5] = 0;

    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
        sutil_set_err("os error");
        goto error;
    }

    buf = (char*)malloc(len);
    if (buf == NULL) {
        sutil_set_err("Error: no memory");
        goto error;
    }

    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
        sutil_set_err("os error");
        goto error;
    }

    lim = buf + len;

    for (next = buf; next < lim; ) {
        ifm = (struct if_msghdr *)next;
        next += ifm->ifm_msglen;
        if (ifm->ifm_type == RTM_IFINFO2) {
            nic_info.clear();
            struct if_msghdr2 *if2m = (struct if_msghdr2 *)ifm;
            struct sockaddr_dl *sdl = (struct sockaddr_dl *)(if2m + 1);
            char ifc_name[32];

            strncpy(ifc_name, sdl->sdl_data, sdl->sdl_nlen);
            ifc_name[sdl->sdl_nlen] = 0;

            nic_info.push_back(if2m->ifm_data.ifi_obytes);
            nic_info.push_back(if2m->ifm_data.ifi_ibytes);
            nic_info.push_back(if2m->ifm_data.ifi_opackets);
            nic_info.push_back(if2m->ifm_data.ifi_ipackets);
            nic_info.push_back(if2m->ifm_data.ifi_oerrors);
            nic_info.push_back(if2m->ifm_data.ifi_ierrors);
            nic_info.push_back(if2m->ifm_data.ifi_iqdrops);

            net_io_counters[ifc_name] = nic_info;
        }
    }

    free(buf);
    return 0;

error:
    if (buf != NULL)
        free(buf);
    return -1;
}


/*
 * Return a map for disk I/O information
 */
int
sutil_disk_io_counters(map<string, vector<uint64_t>> &disk_io_counters)
{
    CFDictionaryRef parent_dict;
    CFDictionaryRef props_dict;
    CFDictionaryRef stats_dict;
    io_registry_entry_t parent;
    io_registry_entry_t disk;
    io_iterator_t disk_list;
    
    vector<uint64_t> disk_info(6);

    // Get list of disks
    if (IOServiceGetMatchingServices(kIOMasterPortDefault,
                                     IOServiceMatching(kIOMediaClass),
                                     &disk_list) != kIOReturnSuccess) {
        sutil_set_err("RuntimeError: unable to get the list of disks.");
        goto error;
    }

    // Iterate over disks
    while ((disk = IOIteratorNext(disk_list)) != 0) {
        disk_info.clear();
        parent_dict = NULL;
        props_dict = NULL;
        stats_dict = NULL;

        if (IORegistryEntryGetParentEntry(disk, kIOServicePlane, &parent)
                != kIOReturnSuccess) {
            sutil_set_err("RuntimeError: unable to get the disk's parent.");
            IOObjectRelease(disk);
            goto error;
        }

        if (IOObjectConformsTo(parent, "IOBlockStorageDriver")) {
            if (IORegistryEntryCreateCFProperties(
                    disk,
                    (CFMutableDictionaryRef *) &parent_dict,
                    kCFAllocatorDefault,
                    kNilOptions
                ) != kIOReturnSuccess)
            {
                sutil_set_err("RuntimeError: unable to get the parent's properties.");
                IOObjectRelease(disk);
                IOObjectRelease(parent);
                goto error;
            }

            if (IORegistryEntryCreateCFProperties(
                    parent,
                    (CFMutableDictionaryRef *) &props_dict,
                    kCFAllocatorDefault,
                    kNilOptions
                ) != kIOReturnSuccess)
            {
                sutil_set_err("RuntimeError: unable to get the disk properties.");
                CFRelease(props_dict);
                IOObjectRelease(disk);
                IOObjectRelease(parent);
                goto error;
            }

            const int kMaxDiskNameSize = 64;
            CFStringRef disk_name_ref = (CFStringRef)CFDictionaryGetValue(
                parent_dict, CFSTR(kIOBSDNameKey));
            char disk_name[kMaxDiskNameSize];

            CFStringGetCString(disk_name_ref,
                               disk_name,
                               kMaxDiskNameSize,
                               CFStringGetSystemEncoding());

            stats_dict = (CFDictionaryRef)CFDictionaryGetValue(
                props_dict, CFSTR(kIOBlockStorageDriverStatisticsKey));

            if (stats_dict == NULL) {
                sutil_set_err("RuntimeError: Unable to get disk stats.");
                goto error;
            }

            CFNumberRef number;
            int64_t reads = 0;
            int64_t writes = 0;
            int64_t read_bytes = 0;
            int64_t write_bytes = 0;
            int64_t read_time = 0;
            int64_t write_time = 0;

            // Get disk reads/writes
            if ((number = (CFNumberRef)CFDictionaryGetValue(
                    stats_dict,
                    CFSTR(kIOBlockStorageDriverStatisticsReadsKey))))
            {
                CFNumberGetValue(number, kCFNumberSInt64Type, &reads);
            }
            if ((number = (CFNumberRef)CFDictionaryGetValue(
                    stats_dict,
                    CFSTR(kIOBlockStorageDriverStatisticsWritesKey))))
            {
                CFNumberGetValue(number, kCFNumberSInt64Type, &writes);
            }

            // Get disk bytes read/written
            if ((number = (CFNumberRef)CFDictionaryGetValue(
                    stats_dict,
                    CFSTR(kIOBlockStorageDriverStatisticsBytesReadKey))))
            {
                CFNumberGetValue(number, kCFNumberSInt64Type, &read_bytes);
            }
            if ((number = (CFNumberRef)CFDictionaryGetValue(
                    stats_dict,
                    CFSTR(kIOBlockStorageDriverStatisticsBytesWrittenKey))))
            {
                CFNumberGetValue(number, kCFNumberSInt64Type, &write_bytes);
            }

            // Get disk time spent reading/writing (nanoseconds)
            if ((number = (CFNumberRef)CFDictionaryGetValue(
                    stats_dict,
                    CFSTR(kIOBlockStorageDriverStatisticsTotalReadTimeKey))))
            {
                CFNumberGetValue(number, kCFNumberSInt64Type, &read_time);
            }
            if ((number = (CFNumberRef)CFDictionaryGetValue(
                    stats_dict,
                    CFSTR(kIOBlockStorageDriverStatisticsTotalWriteTimeKey))))
            {
                CFNumberGetValue(number, kCFNumberSInt64Type, &write_time);
            }

            // Read/Write time on OS X comes back in nanoseconds and in psutil
            // we've standardized on milliseconds so do the conversion.
            disk_info.push_back(reads);
            disk_info.push_back(writes);
            disk_info.push_back(read_bytes);
            disk_info.push_back(write_bytes);
            disk_info.push_back(read_time / 1000 / 1000);
            disk_info.push_back(write_time / 1000 / 1000);

            disk_io_counters[disk_name] = disk_info;
            CFRelease(parent_dict);
            IOObjectRelease(parent);
            CFRelease(props_dict);
            IOObjectRelease(disk);
        }
    }

    IOObjectRelease (disk_list);

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
    struct utmpx *utx;
    sutil_user_info a_user;

    while ((utx = getutxent()) != NULL) {
        if (utx->ut_type != USER_PROCESS)
            continue;
        a_user.username = utx->ut_user;
        a_user.tty = utx->ut_line;
        a_user.host = utx->ut_host;
        a_user.start_time = utx->ut_tv.tv_sec;
        user_list.push_back(a_user);
    }

    endutxent();
    return 0;
}


