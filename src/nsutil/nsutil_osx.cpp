#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#include <node.h>
#include <nan.h>
#include <v8.h>
#include "nsutil_osx_funcs.h"
#include "nsutil_osx_funcs_async.h"

using v8::FunctionTemplate;
using v8::String;

void Init(v8::Handle<v8::Object> exports) {
    exports->Set(NanNew<String>("nsutil_pid_exists_sync"), 
            NanNew<FunctionTemplate>(nsutil_pid_exists_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_pids_sync"), 
            NanNew<FunctionTemplate>(nsutil_pids_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_proc_cmdline_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_cmdline_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_name_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_name_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_cwd_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_cwd_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_exe_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_exe_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_ppid_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_ppid_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_uids_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_uids_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_gids_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_gids_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_tty_nr_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_tty_nr_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_memory_maps_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_memory_maps_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_cpu_times_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_cpu_times_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_create_time_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_create_time_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_memory_info_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_memory_info_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_num_threads_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_num_threads_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_num_ctx_switches_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_num_ctx_switches_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_status_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_status_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_threads_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_threads_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_open_files_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_open_files_sync)->GetFunction());
    
    exports->Set(NanNew<String>("nsutil_proc_num_fds_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_num_fds_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_cpu_count_logical_sync"), 
            NanNew<FunctionTemplate>(nsutil_cpu_count_logical_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_cpu_count_phys_sync"), 
            NanNew<FunctionTemplate>(nsutil_cpu_count_phys_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_virtual_mem_sync"), 
            NanNew<FunctionTemplate>(nsutil_virtual_mem_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_swap_mem_sync"), 
            NanNew<FunctionTemplate>(nsutil_swap_mem_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_cpu_times_sync"), 
            NanNew<FunctionTemplate>(nsutil_cpu_times_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_per_cpu_times_sync"), 
            NanNew<FunctionTemplate>(nsutil_per_cpu_times_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_boot_time_sync"), 
            NanNew<FunctionTemplate>(nsutil_boot_time_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_disk_partitions_sync"), 
            NanNew<FunctionTemplate>(nsutil_disk_partitions_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_proc_connections_sync"), 
            NanNew<FunctionTemplate>(nsutil_proc_connections_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_net_io_counters_sync"), 
            NanNew<FunctionTemplate>(nsutil_net_io_counters_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_disk_io_counters_sync"), 
            NanNew<FunctionTemplate>(nsutil_disk_io_counters_sync)->GetFunction());

    /*
    exports->Set(NanNew<String>("nsutil_posix_getpriority_sync"), 
            NanNew<FunctionTemplate>(nsutil_posix_getpriority_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_posix_setpriority_sync"), 
            NanNew<FunctionTemplate>(nsutil_posix_setpriority_sync)->GetFunction());
    */
    exports->Set(NanNew<String>("nsutil_users_sync"), 
            NanNew<FunctionTemplate>(nsutil_users_sync)->GetFunction());

    // async
    exports->Set(NanNew<String>("nsutil_cpu_times_async"), 
            NanNew<FunctionTemplate>(nsutil_cpu_times_async)->GetFunction());

    exports->Set(NanNew<String>("nsutil_per_cpu_times_async"), 
            NanNew<FunctionTemplate>(nsutil_per_cpu_times_async)->GetFunction());

    exports->Set(NanNew<String>("nsutil_swap_mem_async"), 
            NanNew<FunctionTemplate>(nsutil_swap_mem_async)->GetFunction());

    


    /*
    exports->Set(NanNew<String>("nsutil_"), 
            NanNew<FunctionTemplate>(nsutil_)->GetFunction());

            */
}

NODE_MODULE(nsutil_osx, Init);


