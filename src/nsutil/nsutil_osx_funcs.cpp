#include "nsutil_osx_funcs.h"

using namespace std;
using namespace v8;


// handle osx/sutil_process_info
Handle<Value>
nsutil_pid_exists_sync(const Arguments &args)
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    int32_t pid = args[0]->Int32Value();
    return scope.Close(v8::Boolean::New(sutil_pid_exists(pid)));
}


/*
// handle sutil_posix
Handle<Value>
nsutil_posix_getpriority_sync(const Arguments &args)
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    int32_t pid = args[0]->Int32Value();
    int priority;
    if(sutil_posix_getpriority(pid, priority) == -1) {
        return scope.Close(Undefined());
    }

    return scope.Close(Number::New(priority));
}

Handle<Value>
nsutil_posix_setpriority_sync(const Arguments &args)
{
    HandleScope scope;
    if (args.Length() != 2) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    int32_t pid = args[0]->Int32Value();
    int priority = args[0]->IntegerValue();
    if(sutil_posix_setpriority(pid, priority) == -1) {
        ThrowException(Exception::TypeError(String::New("SetPriority Error")));
    }

    return scope.Close(Undefined());
}
*/

// handle sutil_osx
Handle<Value> 
nsutil_pids_sync(const Arguments &args) 
{
    HandleScope scope;

    vector<int32_t> pids;

    sutil_pids(pids);

    if (pids.empty()) {
        return scope.Close(Undefined());
    }

    Local<Array> pids_arr = Array::New(pids.size());

    size_t i = 0;
    for (auto &pid : pids) {
        pids_arr->Set(i, Integer::New(pid));
        i++;
    }

    return scope.Close(pids_arr);
}

Handle<Value> 
nsutil_proc_cmdline_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int32_t pid = args[0]->Int32Value();
    vector<string> proc_cmdline;
    sutil_proc_cmdline(pid, proc_cmdline);

    if (proc_cmdline.empty()) {
        return scope.Close(Undefined());
    }

    Local<Array> args_arr = Array::New(proc_cmdline.size());

    int i = 0;
    for (auto &c : proc_cmdline) {
        args_arr->Set(i, String::New(c.c_str()));
        i++;
    }

    return scope.Close(args_arr);
}

Handle<Value> 
nsutil_proc_name_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int32_t pid = args[0]->Int32Value();
    string proc_name = sutil_proc_name(pid);

    return scope.Close(String::New(proc_name.c_str()));
}


Handle<Value> 
nsutil_proc_cwd_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int32_t pid = args[0]->Int32Value();
    string proc_cwd = sutil_proc_cwd(pid);

    return scope.Close(String::New(proc_cwd.c_str()));
}



Handle<Value> 
nsutil_proc_exe_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int32_t pid = args[0]->Int32Value();
    string proc_exe = sutil_proc_exe(pid);

    return scope.Close(String::New(proc_exe.c_str()));
}


Handle<Value> 
nsutil_proc_ppid_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int32_t pid = args[0]->Int32Value();
    int32_t ppid = sutil_proc_ppid(pid);

    return scope.Close(Integer::New(ppid));
}

Handle<Value> 
nsutil_proc_uids_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int32_t pid = args[0]->Int32Value();

    int32_t *uids = new int32_t[3];

    if (sutil_proc_uids(pid, uids) == -1) {
        return scope.Close(Undefined());
    }
    Local<Array> uids_arr = Array::New(3);

    for (size_t i = 0; i != 3; i++) {
        uids_arr->Set(i, Integer::New(uids[i]));
    }

    delete[] uids;
    uids = nullptr;

    return scope.Close(uids_arr);
}


Handle<Value> 
nsutil_proc_gids_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int32_t pid = args[0]->Int32Value();

    int32_t *gids = new int32_t[3];

    if (sutil_proc_gids(pid, gids) == -1) {
        return scope.Close(Undefined());
    }
    Local<Array> gids_arr = Array::New(3);

    for (size_t i = 0; i != 3; i++) {
        gids_arr->Set(i, Integer::New(gids[i]));
    }

    delete[] gids;
    gids = nullptr;

    return scope.Close(gids_arr);
}

Handle<Value> 
nsutil_proc_tty_nr_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int32_t pid = args[0]->Int32Value();

    int tty_nr = sutil_proc_tty_nr(pid);

    return scope.Close(Number::New(tty_nr));
}

Handle<Value> 
nsutil_proc_memory_maps_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int32_t pid = args[0]->Int32Value();
    
    vector<proc_mem_map_grouped> proc_mem_map_grouped_list;

    if (sutil_proc_memory_maps(pid, proc_mem_map_grouped_list) == -1) {
        return scope.Close(Undefined());
    }

    Local<Array> m_arr = Array::New(proc_mem_map_grouped_list.size());

    //Local<Object> obj = Object::New();
    //obj->Set(String::NewSymbol("msg"), args[0]->ToString());
    Local<String> pmmap_ext_sym    = String::NewSymbol("pmmap_ext");
    Local<String> addr_perms_sym   = String::NewSymbol("addr_perms");
    Local<String> path_sym         = String::NewSymbol("path");
    Local<String> rss_sym          = String::NewSymbol("rss");
    Local<String> private_sym      = String::NewSymbol("private");
    Local<String> swapped_sym      = String::NewSymbol("swapped");
    Local<String> dirtied_sym      = String::NewSymbol("dirtied");
    Local<String> ref_count_sym    = String::NewSymbol("ref_count");
    Local<String> shadow_depth_sym = String::NewSymbol("shadow_depth");

    size_t i = 0;
    for (auto &mmap : proc_mem_map_grouped_list) {
        Local<Object> tem_obj = Object::New();

        tem_obj->Set(pmmap_ext_sym, 
                String::New(mmap.pmmap_ext.c_str()));

        tem_obj->Set(addr_perms_sym,
                String::New(mmap.addr_perms.c_str()));

        tem_obj->Set(path_sym,
                String::New(mmap.path.c_str()));

        tem_obj->Set(rss_sym,
                Number::New(mmap.rss));

        tem_obj->Set(private_sym,
                Number::New(mmap.private_v));

        tem_obj->Set(swapped_sym,
                Number::New(mmap.swapped));

        tem_obj->Set(dirtied_sym,
                Number::New(mmap.dirtied));

        tem_obj->Set(ref_count_sym,
                Number::New(mmap.ref_count));

        tem_obj->Set(shadow_depth_sym,
                Number::New(mmap.shadow_depth));

        m_arr->Set(i, tem_obj);
        i++;
    }

    return scope.Close(m_arr);
}

Handle<Value> 
nsutil_proc_cpu_times_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int32_t pid = args[0]->Int32Value();

    double *times = new double[2];
    if (sutil_proc_cpu_times(pid, times) == -1) {
        return scope.Close(Undefined());
    }

    Local<Object> t_obj = Object::New();

    t_obj->Set(String::NewSymbol("user"), Number::New(times[0]));
    t_obj->Set(String::NewSymbol("sys"), Number::New(times[1]));

    return scope.Close(t_obj);
}

Handle<Value> 
nsutil_proc_create_time_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int32_t pid = args[0]->Int32Value();
    
    int32_t t = sutil_proc_create_time(pid);

    return scope.Close(Integer::New(t));
}

Handle<Value>
nsutil_proc_memory_info_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    int32_t pid = args[0]->Int32Value();
    uint64_t *mem_info = new uint64_t[4];
    if (sutil_proc_memory_info(pid, mem_info) == -1) {
        return scope.Close(Undefined());
    }
    Local<Object> mem_obj = Object::New(); 
    mem_obj->Set(String::NewSymbol("rss"), Number::New(mem_info[0]));
    mem_obj->Set(String::NewSymbol("vms"), Number::New(mem_info[1]));
    mem_obj->Set(String::NewSymbol("faults"), Number::New(mem_info[2]));
    mem_obj->Set(String::NewSymbol("pageins"), Number::New(mem_info[3]));
    delete[] mem_info;
    mem_info = nullptr;
    return scope.Close(mem_obj);
}


Handle<Value>
nsutil_proc_num_threads_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    int32_t pid = args[0]->Int32Value();
    uint32_t num_thread = sutil_proc_num_threads(pid);
    return scope.Close(Integer::NewFromUnsigned(num_thread));
}


Handle<Value>
nsutil_proc_num_ctx_switches_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    int32_t pid = args[0]->Int32Value();
    uint32_t ctx_switches = sutil_proc_num_ctx_switches(pid);
    return scope.Close(Integer::NewFromUnsigned(ctx_switches));
}

Handle<Value>
nsutil_proc_status_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    int32_t pid = args[0]->Int32Value();
    int st = sutil_proc_status(pid);
    return scope.Close(Integer::New(st));
}


//int sutil_proc_threads(const int32_t &pid, vector<vector<float>> &threads)
Handle<Value>
nsutil_proc_threads_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    int32_t pid = args[0]->Int32Value();
    vector<vector<float>> threads;
    if (sutil_proc_threads(pid, threads) == -1) {
        return scope.Close(Undefined());
    }

    Local<Array> ths_arr = Array::New(threads.size());
    size_t i = 0;
    for (auto &th : threads) {
        //cout << th[1] << " | " << th[2] << endl;
        Local<Object> tem_obj = Object::New();
        tem_obj->Set(String::NewSymbol("idx"),
                Number::New(th[0]));
        tem_obj->Set(String::NewSymbol("user"),
                Number::New(th[1]));
        tem_obj->Set(String::NewSymbol("sys"),
                Number::New(th[2]));
        ths_arr->Set(i, tem_obj);
        i++;
    }
    return scope.Close(ths_arr); 
}

//int sutil_proc_open_files(const int32_t &pid, vector<proc_open_file> &proc_open_files)
Handle<Value>
nsutil_proc_open_files_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    int32_t pid = args[0]->Int32Value();
    vector<proc_open_file> proc_open_files;
    if (sutil_proc_open_files(pid, proc_open_files) == -1) {
        return scope.Close(Undefined());
    }
    Local<Array> fs_arr = Array::New(proc_open_files.size());
    size_t i = 0;
    for (auto &ofs : proc_open_files) {
        Local<Object> fs_obj = Object::New();
        fs_obj->Set(String::NewSymbol("path"),
                String::New(ofs.path.c_str()));
        fs_obj->Set(String::NewSymbol("fd"),
                Integer::New(ofs.fd));
        fs_arr->Set(i, fs_obj);
        i++;
    }
    return scope.Close(fs_arr);
}

Handle<Value>
nsutil_proc_num_fds_sync(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() == 0) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    int32_t pid = args[0]->Int32Value();
    return scope.Close(Integer::New(sutil_proc_num_fds(pid)));
}

Handle<Value>
nsutil_cpu_count_logical_sync(const Arguments &args)
{
    HandleScope scope;
    return scope.Close(Number::New(sutil_cpu_count_logical()));
}


Handle<Value>
nsutil_cpu_count_phys_sync(const Arguments &args)
{
    HandleScope scope;
    return scope.Close(Number::New(sutil_cpu_count_phys()));
}

//int sutil_virtual_mem(uint64_t *&vir_mem)
Handle<Value>
nsutil_virtual_mem_sync(const Arguments &args) 
{
    HandleScope scope;
    uint64_t *vir_mem = new uint64_t[5];
    if (sutil_virtual_mem(vir_mem) == -1) {
        return scope.Close(Undefined());
    }
    
    Local<Object> vir_obj = Object::New();

    vir_obj->Set(String::NewSymbol("total"),
            Number::New(vir_mem[0]));
    vir_obj->Set(String::NewSymbol("active"), 
            Number::New(vir_mem[1]));
    vir_obj->Set(String::NewSymbol("inactive"), 
            Number::New(vir_mem[2]));
    vir_obj->Set(String::NewSymbol("wire"), 
            Number::New(vir_mem[3]));
    vir_obj->Set(String::NewSymbol("free"), 
            Number::New(vir_mem[4]));

    delete[] vir_mem;
    vir_mem = nullptr;
    return scope.Close(vir_obj);

}

Handle<Value>
nsutil_swap_mem_sync(const Arguments &args)
{
    HandleScope scope;
    uint64_t *swap_mem = new uint64_t[5];
    if (sutil_swap_mem(swap_mem) == -1) {
        return scope.Close(Undefined());
    }

    Local<Object> swap_obj = Object::New();
    
    swap_obj->Set(String::NewSymbol("total"),
            Number::New(swap_mem[0]));
    swap_obj->Set(String::NewSymbol("used"),
            Number::New(swap_mem[1]));
    swap_obj->Set(String::NewSymbol("avail"),
            Number::New(swap_mem[2]));
    swap_obj->Set(String::NewSymbol("pageins"),
            Number::New(swap_mem[3]));
    swap_obj->Set(String::NewSymbol("pageouts"),
            Number::New(swap_mem[4]));

    delete[] swap_mem;
    swap_mem = nullptr;
    return scope.Close(swap_obj);
}

Handle<Value>
nsutil_cpu_times_sync(const Arguments &args)
{
    HandleScope scope;

    double *cpu_times = new double[4];

    if (sutil_cpu_times(cpu_times) == -1) {
        return scope.Close(Undefined());
    }

    Local<Object> cpu_obj = Object::New();

    cpu_obj->Set(String::NewSymbol("user"),
            Number::New(cpu_times[0]));
    cpu_obj->Set(String::NewSymbol("nice"),
            Number::New(cpu_times[1]));
    cpu_obj->Set(String::NewSymbol("sys"),
            Number::New(cpu_times[2]));
    cpu_obj->Set(String::NewSymbol("idle"),
            Number::New(cpu_times[3]));

    delete[] cpu_times;
    cpu_times = nullptr;
    return scope.Close(cpu_obj);

}

Handle<Value>
nsutil_per_cpu_times_sync(const Arguments &args)
{
    HandleScope scope;

    vector<vector<double>> per_cpu_times;

    if (sutil_per_cpu_times(per_cpu_times) == -1) {
        return scope.Close(Undefined());
    }

    Local<Array> per_cpu_arr = Array::New(per_cpu_times.size());

    size_t i = 0;
    for (auto &cpu_times : per_cpu_times) {
        Local<Object> cpu_obj = Object::New();
        cpu_obj->Set(String::NewSymbol("user"),
                Number::New(cpu_times[0]));
        cpu_obj->Set(String::NewSymbol("nice"),
                Number::New(cpu_times[1]));
        cpu_obj->Set(String::NewSymbol("sys"),
                Number::New(cpu_times[2]));
        cpu_obj->Set(String::NewSymbol("idle"),
                Number::New(cpu_times[3]));

        per_cpu_arr->Set(i, cpu_obj);
        i++;

    }

    return scope.Close(per_cpu_arr);

}

Handle<Value>
nsutil_boot_time_sync(const Arguments &args)
{
    HandleScope scope;
    return scope.Close(Number::New(sutil_boot_time()));
}

Handle<Value>
nsutil_disk_partitions_sync(const Arguments &args) 
{
    HandleScope scope;

    vector<vector<string>> disk_partitions;
    if (sutil_disk_partitions(disk_partitions) == -1) {
        return scope.Close(Undefined());
    }

    Local<Array> dps_arr = Array::New(disk_partitions.size());
    size_t i = 0;

    for (auto &dps : disk_partitions) {
        Local<Object> dps_obj = Object::New();
        
        dps_obj->Set(String::NewSymbol("device"),
                String::New(dps[0].c_str()));
        dps_obj->Set(String::NewSymbol("mount_point"),
                String::New(dps[1].c_str()));
        dps_obj->Set(String::NewSymbol("fs_type"),
                String::New(dps[2].c_str()));
        dps_obj->Set(String::NewSymbol("options"),
                String::New(dps[3].c_str()));
        dps_arr->Set(i, dps_obj);
        i++;
    }

    return scope.Close(dps_arr);

}

//sutil_proc_connections(const int32_t &pid,
Handle<Value>
nsutil_proc_connections_sync(const Arguments &args)
{
    HandleScope scope;
    if (args.Length() != 3) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    if (!args[1]->IsArray()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    if (!args[2]->IsArray()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int32_t pid = args[0]->Int32Value();

    Local<Object> af_filter_obj = args[1]->ToObject();
    uint32_t len = af_filter_obj->Get(String::NewSymbol("length"))->ToObject()->Uint32Value();
    vector<int> af_filter(len);
    for (uint32_t i = 0; i < len; i++) {
        af_filter.push_back(af_filter_obj->Get(i)->ToObject()->Uint32Value());
    }

    Local<Object> type_filter_obj = args[2]->ToObject();
    len = type_filter_obj->Get(String::NewSymbol("length"))->ToObject()->Uint32Value();
    vector<int> type_filter(len);
    for (uint32_t i = 0; i < len; i++) {
        type_filter.push_back(type_filter_obj->Get(i)->ToObject()->Uint32Value());
    }
    
    vector<proc_conn> proc_conn_list;
    sutil_proc_connections(pid, af_filter, type_filter, proc_conn_list);

    if (proc_conn_list.empty()) {
        return scope.Close(Undefined());
    }

    Local<Array> proc_conn_list_arr = Array::New(proc_conn_list.size());

    size_t i = 0;
    for (auto &conn : proc_conn_list) {
        Local<Object> conn_obj = Object::New();
        conn_obj->Set(String::NewSymbol("fd"),
                Integer::New(conn.fd));
        conn_obj->Set(String::NewSymbol("family"),
                Integer::New(conn.family));
        conn_obj->Set(String::NewSymbol("type"),
                Integer::New(conn.type));
        if (conn.family == 1) {
            conn_obj->Set(String::NewSymbol("laddr"),
                String::New(conn.addr.c_str()));
            conn_obj->Set(String::NewSymbol("rcaddr"),
                String::New(conn.caddr.c_str()));
            conn_obj->Set(String::NewSymbol("conn_none"),
                Integer::New(conn.conn_none));
        } else if (conn.family == 2 || conn.family == 10) {
            Local<Array> laddr = Array::New();
            laddr->Set(0, String::New(conn.laddr_ip.c_str()));
            laddr->Set(1, Integer::New(conn.laddr_port));
            conn_obj->Set(String::NewSymbol("laddr"), laddr);
            Local<Array> raddr = Array::New();
            raddr->Set(0, String::New(conn.raddr_ip.c_str()));
            raddr->Set(1, Integer::New(conn.raddr_port));
            conn_obj->Set(String::NewSymbol("raddr"), laddr);
            /*
            conn_obj->Set(String::NewSymbol("laddr_ip"),
                String::New(conn.laddr_ip.c_str()));
            conn_obj->Set(String::NewSymbol("laddr_port"),
                Integer::New(conn.laddr_port));
            conn_obj->Set(String::NewSymbol("raddr_ip"),
                String::New(conn.raddr_ip.c_str()));
            conn_obj->Set(String::NewSymbol("raddr_port"),
                Integer::New(conn.raddr_port));
            conn_obj->Set(String::NewSymbol("state"),
                Integer::New(conn.state));
                */
        }
        proc_conn_list_arr->Set(i, conn_obj);
        i++;
    }

    return scope.Close(proc_conn_list_arr);
}


Handle<Value>
nsutil_net_io_counters_sync(const Arguments &args)
{
    HandleScope scope;
    map<string, vector<uint64_t>> net_io_counters;
    if (sutil_net_io_counters(net_io_counters) == -1) {
        return scope.Close(Undefined());
    }

    Local<Object> net_io_objs = Object::New();

    for (auto &net_io : net_io_counters) {
        
        Local<Object> io_obj = Object::New();
        io_obj->Set(String::NewSymbol("obytes"), 
                Number::New(net_io.second[0]));
        io_obj->Set(String::NewSymbol("ibytes"), 
                Number::New(net_io.second[1]));
        io_obj->Set(String::NewSymbol("opkts"), 
                Number::New(net_io.second[2]));
        io_obj->Set(String::NewSymbol("ipkts"), 
                Number::New(net_io.second[3]));
        io_obj->Set(String::NewSymbol("oerrs"), 
                Number::New(net_io.second[4]));
        io_obj->Set(String::NewSymbol("ierrs"), 
                Number::New(net_io.second[5]));
        io_obj->Set(String::NewSymbol("iqdrops"), 
                Number::New(net_io.second[6]));

        net_io_objs->Set(String::NewSymbol(net_io.first.c_str()), io_obj);

    }

    return scope.Close(net_io_objs);
}

Handle<Value>
nsutil_disk_io_counters_sync(const Arguments &args)
{
    HandleScope scope;
    map<string, vector<uint64_t>> disk_io_counters;
    if (sutil_disk_io_counters(disk_io_counters) == -1) {
        return scope.Close(Undefined()); 
    }

    Local<Object> disks_list = Object::New();
    for (auto &disk : disk_io_counters) {
        Local<Object> disk_obj = Object::New();
        disk_obj->Set(String::NewSymbol("reads"),
                Number::New(disk.second[0]));
        disk_obj->Set(String::NewSymbol("writes"),
                Number::New(disk.second[1]));
        disk_obj->Set(String::NewSymbol("read_bytes"),
                Number::New(disk.second[2]));
        disk_obj->Set(String::NewSymbol("write_bytes"),
                Number::New(disk.second[3]));
        disk_obj->Set(String::NewSymbol("read_time"),
                Number::New(disk.second[4]));
        disk_obj->Set(String::NewSymbol("write_time"),
                Number::New(disk.second[5]));
        disks_list->Set(String::NewSymbol(disk.first.c_str()), disk_obj);
    }

    return scope.Close(disks_list);
}

Handle<Value>
nsutil_users_sync(const Arguments &args)
{
    HandleScope scope;
    vector<sutil_user_info> user_list;
    if (sutil_users(user_list) == -1) {
        return scope.Close(Undefined());
    }

    Local<Array> user_arr = Array::New(user_list.size());

    size_t i = 0;
    for (auto &user : user_list) {
        Local<Object> user_obj = Object::New();
        user_obj->Set(String::NewSymbol("username"), 
                String::New(user.username.c_str()));
        user_obj->Set(String::NewSymbol("tty"), 
                String::New(user.tty.c_str()));
        user_obj->Set(String::NewSymbol("host"), 
                String::New(user.host.c_str()));
        user_obj->Set(String::NewSymbol("startTime"), 
                Number::New(user.start_time));
        user_arr->Set(i, user_obj);
        i++;
    }
    return scope.Close(user_arr);

}













