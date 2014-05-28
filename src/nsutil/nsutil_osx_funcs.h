#include <node.h>
#include <v8.h>
#include "../sutil/osx/process_info.h"
//include "../sutil/sutil_posix.h"
#include "../sutil/sutil_osx.h"



v8::Handle<v8::Value>
nsutil_pid_exists_sync(const v8::Arguments &args);
/*
v8::Handle<v8::Value>
nsutil_posix_getpriority_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_posix_setpriority_sync(const v8::Arguments &args);
*/
v8::Handle<v8::Value> 
nsutil_pids_sync(const v8::Arguments &args); 

/// process

v8::Handle<v8::Value> 
nsutil_proc_cmdline_sync(const v8::Arguments &args);

v8::Handle<v8::Value> 
nsutil_proc_name_sync(const v8::Arguments &args);

v8::Handle<v8::Value> 
nsutil_proc_cwd_sync(const v8::Arguments &args); 

v8::Handle<v8::Value> 
nsutil_proc_exe_sync(const v8::Arguments &args); 

v8::Handle<v8::Value> 
nsutil_proc_ppid_sync(const v8::Arguments &args);

v8::Handle<v8::Value> 
nsutil_proc_uids_sync(const v8::Arguments &args);

v8::Handle<v8::Value> 
nsutil_proc_gids_sync(const v8::Arguments &args);

v8::Handle<v8::Value> 
nsutil_proc_tty_nr_sync(const v8::Arguments &args);

v8::Handle<v8::Value> 
nsutil_proc_memory_maps_sync(const v8::Arguments &args);

v8::Handle<v8::Value> 
nsutil_proc_cpu_times_sync(const v8::Arguments &args);

v8::Handle<v8::Value> 
nsutil_proc_create_time_sync(const v8::Arguments &args); 

v8::Handle<v8::Value>
nsutil_proc_memory_info_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_proc_num_threads_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_proc_num_ctx_switches_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_proc_status_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_proc_threads_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_proc_open_files_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_proc_num_fds_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_proc_connections_sync(const v8::Arguments &args);

/// system
v8::Handle<v8::Value>
nsutil_cpu_count_logical_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_cpu_count_phys_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_virtual_mem_sync(const v8::Arguments &args); 

v8::Handle<v8::Value>
nsutil_swap_mem_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_cpu_times_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_per_cpu_times_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_boot_time_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_disk_partitions_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_net_io_counters_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_disk_io_counters_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_users_sync(const v8::Arguments &args);

