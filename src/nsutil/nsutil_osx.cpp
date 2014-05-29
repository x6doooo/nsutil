#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#include <node.h>
#include <v8.h>
#include "nsutil_osx_funcs.h"
#include "nsutil_osx_funcs_async.h"

void Init(v8::Handle<v8::Object> exports) {
    exports->Set(v8::String::NewSymbol("nsutil_pid_exists_sync"), 
            v8::FunctionTemplate::New(nsutil_pid_exists_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_pids_sync"), 
            v8::FunctionTemplate::New(nsutil_pids_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_proc_cmdline_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_cmdline_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_name_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_name_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_cwd_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_cwd_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_exe_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_exe_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_ppid_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_ppid_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_uids_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_uids_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_gids_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_gids_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_tty_nr_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_tty_nr_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_memory_maps_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_memory_maps_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_cpu_times_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_cpu_times_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_create_time_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_create_time_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_memory_info_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_memory_info_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_num_threads_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_num_threads_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_num_ctx_switches_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_num_ctx_switches_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_status_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_status_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_threads_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_threads_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_open_files_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_open_files_sync)->GetFunction());
    
    exports->Set(v8::String::NewSymbol("nsutil_proc_num_fds_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_num_fds_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_cpu_count_logical_sync"), 
            v8::FunctionTemplate::New(nsutil_cpu_count_logical_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_cpu_count_phys_sync"), 
            v8::FunctionTemplate::New(nsutil_cpu_count_phys_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_virtual_mem_sync"), 
            v8::FunctionTemplate::New(nsutil_virtual_mem_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_swap_mem_sync"), 
            v8::FunctionTemplate::New(nsutil_swap_mem_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_cpu_times_sync"), 
            v8::FunctionTemplate::New(nsutil_cpu_times_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_per_cpu_times_sync"), 
            v8::FunctionTemplate::New(nsutil_per_cpu_times_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_boot_time_sync"), 
            v8::FunctionTemplate::New(nsutil_boot_time_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_disk_partitions_sync"), 
            v8::FunctionTemplate::New(nsutil_disk_partitions_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_proc_connections_sync"), 
            v8::FunctionTemplate::New(nsutil_proc_connections_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_net_io_counters_sync"), 
            v8::FunctionTemplate::New(nsutil_net_io_counters_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_disk_io_counters_sync"), 
            v8::FunctionTemplate::New(nsutil_disk_io_counters_sync)->GetFunction());

    /*
    exports->Set(v8::String::NewSymbol("nsutil_posix_getpriority_sync"), 
            v8::FunctionTemplate::New(nsutil_posix_getpriority_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_posix_setpriority_sync"), 
            v8::FunctionTemplate::New(nsutil_posix_setpriority_sync)->GetFunction());
    */
    exports->Set(v8::String::NewSymbol("nsutil_users_sync"), 
            v8::FunctionTemplate::New(nsutil_users_sync)->GetFunction());

    // async
    exports->Set(v8::String::NewSymbol("nsutil_cpu_times_async"), 
            v8::FunctionTemplate::New(nsutil_cpu_times_async)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_per_cpu_times_async"), 
            v8::FunctionTemplate::New(nsutil_per_cpu_times_async)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_swap_mem_async"), 
            v8::FunctionTemplate::New(nsutil_swap_mem_async)->GetFunction());

    


    /*
    exports->Set(v8::String::NewSymbol("nsutil_"), 
            v8::FunctionTemplate::New(nsutil_)->GetFunction());

            */
}

NODE_MODULE(nsutil_osx, Init);


