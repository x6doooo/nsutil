#include "nsutil_osx_funcs.h"

using namespace std;
using namespace v8;


// handle osx/sutil_process_info
//Handle<Value>
//nsutil_pid_exists_sync(const Arguments &args)
NAN_METHOD(nsutil_pid_exists_sync)
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }
    int32_t pid = args[0]->Int32Value();
    NanReturnValue(NanNew<v8::Boolean>(sutil_pid_exists(pid)));
}



// handle sutil_osx
//Handle<Value> 
//nsutil_pids_sync(const Arguments &args) 
NAN_METHOD(nsutil_pids_sync) 
{
    NanScope();

    vector<int32_t> pids;

    sutil_pids(pids);

    if (pids.empty()) {
        NanReturnUndefined();
    }

    Local<Array> pids_arr = NanNew<Array>(pids.size());

    size_t i = 0;
    for (auto &pid : pids) {
        pids_arr->Set(i, NanNew<Integer>(pid));
        i++;
    }

    NanReturnValue(pids_arr);
}

//Handle<Value> 
//nsutil_proc_cmdline_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_cmdline_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    int32_t pid = args[0]->Int32Value();
    vector<string> proc_cmdline;
    sutil_proc_cmdline(pid, proc_cmdline);

    if (proc_cmdline.empty()) {
        NanReturnUndefined();
    }

    Local<Array> args_arr = NanNew<Array>(proc_cmdline.size());

    int i = 0;
    for (auto &c : proc_cmdline) {
        args_arr->Set(i, NanNew(c.c_str()));
        i++;
    }

    NanReturnValue(args_arr);
}

//Handle<Value> 
//nsutil_proc_name_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_name_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    int32_t pid = args[0]->Int32Value();
    string proc_name = sutil_proc_name(pid);

    NanReturnValue(NanNew(proc_name.c_str()));
}


//Handle<Value> 
//nsutil_proc_cwd_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_cwd_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    int32_t pid = args[0]->Int32Value();
    string proc_cwd = sutil_proc_cwd(pid);

    NanReturnValue(NanNew(proc_cwd.c_str()));
}



//Handle<Value> 
//nsutil_proc_exe_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_exe_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    int32_t pid = args[0]->Int32Value();
    string proc_exe = sutil_proc_exe(pid);

    NanReturnValue(NanNew(proc_exe.c_str()));
}


//Handle<Value> 
//nsutil_proc_ppid_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_ppid_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    int32_t pid = args[0]->Int32Value();
    int32_t ppid = sutil_proc_ppid(pid);

    NanReturnValue(NanNew<Integer>(ppid));
}

//Handle<Value> 
//nsutil_proc_uids_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_uids_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    int32_t pid = args[0]->Int32Value();

    int32_t *uids = new int32_t[3];

    if (sutil_proc_uids(pid, uids) == -1) {
        NanReturnUndefined();
    }
    Local<Array> uids_arr = NanNew<Array>(3);

    for (size_t i = 0; i != 3; i++) {
        uids_arr->Set(i, NanNew<Integer>(uids[i]));
    }

    delete[] uids;
    uids = nullptr;

    NanReturnValue(uids_arr);
}


//Handle<Value> 
//nsutil_proc_gids_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_gids_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    int32_t pid = args[0]->Int32Value();

    int32_t *gids = new int32_t[3];

    if (sutil_proc_gids(pid, gids) == -1) {
        NanReturnUndefined();
    }
    Local<Array> gids_arr = NanNew<Array>(3);

    for (size_t i = 0; i != 3; i++) {
        gids_arr->Set(i, NanNew<Integer>(gids[i]));
    }

    delete[] gids;
    gids = nullptr;

    NanReturnValue(gids_arr);
}

//Handle<Value> 
//nsutil_proc_tty_nr_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_tty_nr_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    int32_t pid = args[0]->Int32Value();

    int tty_nr = sutil_proc_tty_nr(pid);

    NanReturnValue(NanNew<Number>(tty_nr));
}

//Handle<Value> 
//nsutil_proc_memory_maps_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_memory_maps_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    int32_t pid = args[0]->Int32Value();
    
    vector<proc_mem_map_grouped> proc_mem_map_grouped_list;

    if (sutil_proc_memory_maps(pid, proc_mem_map_grouped_list) == -1) {
        NanReturnUndefined();
    }

    Local<Array> m_arr = NanNew<Array>(proc_mem_map_grouped_list.size());

    //Local<Object> obj = NanNew<Object>();
    //obj->Set(NanNew("msg"), args[0]->ToString());
    Local<String> pmmap_ext_sym    = NanNew("pmmap_ext");
    Local<String> addr_perms_sym   = NanNew("addr_perms");
    Local<String> path_sym         = NanNew("path");
    Local<String> rss_sym          = NanNew("rss");
    Local<String> private_sym      = NanNew("private");
    Local<String> swapped_sym      = NanNew("swapped");
    Local<String> dirtied_sym      = NanNew("dirtied");
    Local<String> ref_count_sym    = NanNew("ref_count");
    Local<String> shadow_depth_sym = NanNew("shadow_depth");

    size_t i = 0;
    Local<Object> tem_obj = NanNew<Object>();

    for (auto &mmap : proc_mem_map_grouped_list) {

        tem_obj->Set(pmmap_ext_sym, 
                NanNew(mmap.pmmap_ext.c_str()));

        tem_obj->Set(addr_perms_sym,
                NanNew(mmap.addr_perms.c_str()));

        tem_obj->Set(path_sym,
                NanNew(mmap.path.c_str()));

        tem_obj->Set(rss_sym,
                NanNew<Number>(mmap.rss));

        tem_obj->Set(private_sym,
                NanNew<Number>(mmap.private_v));

        tem_obj->Set(swapped_sym,
                NanNew<Number>(mmap.swapped));

        tem_obj->Set(dirtied_sym,
                NanNew<Number>(mmap.dirtied));

        tem_obj->Set(ref_count_sym,
                NanNew<Number>(mmap.ref_count));

        tem_obj->Set(shadow_depth_sym,
                NanNew<Number>(mmap.shadow_depth));

        m_arr->Set(i, tem_obj->Clone());
        i++;
    }

    NanReturnValue(m_arr);
}

//Handle<Value> 
//nsutil_proc_cpu_times_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_cpu_times_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    int32_t pid = args[0]->Int32Value();

    double *times = new double[2];
    if (sutil_proc_cpu_times(pid, times) == -1) {
        NanReturnUndefined();
    }

    Local<Object> t_obj = NanNew<Object>();

    t_obj->Set(NanNew("user"), NanNew<Number>(times[0]));
    t_obj->Set(NanNew("sys"), NanNew<Number>(times[1]));

    NanReturnValue(t_obj);
}

//Handle<Value> 
//nsutil_proc_create_time_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_create_time_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    int32_t pid = args[0]->Int32Value();
    
    int32_t t = sutil_proc_create_time(pid);

    NanReturnValue(NanNew<Integer>(t));
}

//Handle<Value>
//nsutil_proc_memory_info_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_memory_info_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }
    int32_t pid = args[0]->Int32Value();
    uint64_t *mem_info = new uint64_t[4];
    if (sutil_proc_memory_info(pid, mem_info) == -1) {
        NanReturnUndefined();
    }
    Local<Object> mem_obj = NanNew<Object>(); 
    mem_obj->Set(NanNew("rss"), NanNew<Number>(mem_info[0]));
    mem_obj->Set(NanNew("vms"), NanNew<Number>(mem_info[1]));
    //mem_obj->Set(NanNew("faults"), NanNew<Number>(mem_info[2]));
    //mem_obj->Set(NanNew("pageins"), NanNew<Number>(mem_info[3]));
    delete[] mem_info;
    mem_info = nullptr;
    NanReturnValue(mem_obj);
}


//Handle<Value>
//nsutil_proc_num_threads_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_num_threads_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }
    int32_t pid = args[0]->Int32Value();
    uint32_t num_thread = sutil_proc_num_threads(pid);
    NanReturnValue(NanNew<Integer>(num_thread));
}


//Handle<Value>
//nsutil_proc_num_ctx_switches_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_num_ctx_switches_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }
    int32_t pid = args[0]->Int32Value();
    uint32_t ctx_switches = sutil_proc_num_ctx_switches(pid);
    NanReturnValue(NanNew<Integer>(ctx_switches));
}

//Handle<Value>
//nsutil_proc_status_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_status_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }
    int32_t pid = args[0]->Int32Value();
    int st = sutil_proc_status(pid);
    NanReturnValue(NanNew<Integer>(st));
}


//int sutil_proc_threads(const int32_t &pid, vector<vector<float>> &threads)
//Handle<Value>
//nsutil_proc_threads_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_threads_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }
    int32_t pid = args[0]->Int32Value();
    vector<vector<float>> threads;
    if (sutil_proc_threads(pid, threads) == -1) {
        NanReturnUndefined();
    }

    Local<Array> ths_arr = NanNew<Array>(threads.size());
    size_t i = 0;
    Local<Object> tem_obj = NanNew<Object>();
    for (auto &th : threads) {
        //cout << th[1] << " | " << th[2] << endl;
        tem_obj->Set(NanNew("idx"),
                NanNew<Number>(th[0]));
        tem_obj->Set(NanNew("user"),
                NanNew<Number>(th[1]));
        tem_obj->Set(NanNew("sys"),
                NanNew<Number>(th[2]));
        ths_arr->Set(i, tem_obj->Clone());
        i++;
    }
    NanReturnValue(ths_arr); 
}

//int sutil_proc_open_files(const int32_t &pid, vector<proc_open_file> &proc_open_files)
//Handle<Value>
//nsutil_proc_open_files_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_open_files_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }
    int32_t pid = args[0]->Int32Value();
    vector<proc_open_file> proc_open_files;
    if (sutil_proc_open_files(pid, proc_open_files) == -1) {
        NanReturnUndefined();
    }
    Local<Array> fs_arr = NanNew<Array>(proc_open_files.size());
    size_t i = 0;
    Local<Object> fs_obj = NanNew<Object>();
    for (auto &ofs : proc_open_files) {
        fs_obj->Set(NanNew("path"),
                NanNew(ofs.path.c_str()));
        fs_obj->Set(NanNew("fd"),
                NanNew<Integer>(ofs.fd));
        fs_arr->Set(i, fs_obj->Clone());
        i++;
    }
    NanReturnValue(fs_arr);
}

//Handle<Value>
//nsutil_proc_num_fds_sync(const Arguments &args) 
NAN_METHOD(nsutil_proc_num_fds_sync) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }
    int32_t pid = args[0]->Int32Value();
    NanReturnValue(NanNew<Integer>(sutil_proc_num_fds(pid)));
}

//Handle<Value>
//nsutil_cpu_count_logical_sync(const Arguments &args)
NAN_METHOD(nsutil_cpu_count_logical_sync)
{
    NanScope();
    NanReturnValue(NanNew<Number>(sutil_cpu_count_logical()));
}


//Handle<Value>
//nsutil_cpu_count_phys_sync(const Arguments &args)
NAN_METHOD(nsutil_cpu_count_phys_sync)
{
    NanScope();
    NanReturnValue(NanNew<Number>(sutil_cpu_count_phys()));
}

//int sutil_virtual_mem(uint64_t *&vir_mem)
//Handle<Value>
//nsutil_virtual_mem_sync(const Arguments &args) 
NAN_METHOD(nsutil_virtual_mem_sync) 
{
    NanScope();
    uint64_t *vir_mem = new uint64_t[5];
    if (sutil_virtual_mem(vir_mem) == -1) {
        NanReturnUndefined();
    }
    
    Local<Object> vir_obj = NanNew<Object>();

    vir_obj->Set(NanNew("total"),
            NanNew<Number>(vir_mem[0]));
    vir_obj->Set(NanNew("active"), 
            NanNew<Number>(vir_mem[1]));
    vir_obj->Set(NanNew("inactive"), 
            NanNew<Number>(vir_mem[2]));
    vir_obj->Set(NanNew("wire"), 
            NanNew<Number>(vir_mem[3]));
    vir_obj->Set(NanNew("free"), 
            NanNew<Number>(vir_mem[4]));

    delete[] vir_mem;
    vir_mem = nullptr;
    NanReturnValue(vir_obj);

}

//Handle<Value>
//nsutil_swap_mem_sync(const Arguments &args)
NAN_METHOD(nsutil_swap_mem_sync)
{
    NanScope();
    uint64_t *swap_mem = new uint64_t[5];
    if (sutil_swap_mem(swap_mem) == -1) {
        NanReturnUndefined();
    }

    Local<Object> swap_obj = NanNew<Object>();
    
    swap_obj->Set(NanNew("total"),
            NanNew<Number>(swap_mem[0]));
    swap_obj->Set(NanNew("used"),
            NanNew<Number>(swap_mem[1]));
    swap_obj->Set(NanNew("free"),
            NanNew<Number>(swap_mem[2]));
    swap_obj->Set(NanNew("sin"),
            NanNew<Number>(swap_mem[3]));
    swap_obj->Set(NanNew("sout"),
            NanNew<Number>(swap_mem[4]));

    delete[] swap_mem;
    swap_mem = nullptr;
    NanReturnValue(swap_obj);
}


//Handle<Value>
//nsutil_cpu_times_sync(const Arguments &args)
NAN_METHOD(nsutil_cpu_times_sync)
{
    NanScope();

    double *cpu_times = new double[4];

    if (sutil_cpu_times(cpu_times) == -1) {
        NanReturnUndefined();
    }

    Local<Object> cpu_obj = NanNew<Object>();

    cpu_obj->Set(NanNew("user"),
            NanNew<Number>(cpu_times[0]));
    cpu_obj->Set(NanNew("nice"),
            NanNew<Number>(cpu_times[1]));
    cpu_obj->Set(NanNew("sys"),
            NanNew<Number>(cpu_times[2]));
    cpu_obj->Set(NanNew("idle"),
            NanNew<Number>(cpu_times[3]));

    delete[] cpu_times;
    cpu_times = nullptr;

    NanReturnValue(cpu_obj);
}

//Handle<Value>
//nsutil_per_cpu_times_sync(const Arguments &args)
NAN_METHOD(nsutil_per_cpu_times_sync)
{
    NanScope();

    vector<vector<double>> per_cpu_times;

    if (sutil_per_cpu_times(per_cpu_times) == -1) {
        NanReturnUndefined();
    }

    Local<Array> per_cpu_arr = NanNew<Array>(per_cpu_times.size());

    size_t i = 0;
    Local<Object> cpu_obj = NanNew<Object>();
    for (auto &cpu_times : per_cpu_times) {
        cpu_obj->Set(NanNew("user"),
                NanNew<Number>(cpu_times[0]));
        cpu_obj->Set(NanNew("nice"),
                NanNew<Number>(cpu_times[1]));
        cpu_obj->Set(NanNew("sys"),
                NanNew<Number>(cpu_times[2]));
        cpu_obj->Set(NanNew("idle"),
                NanNew<Number>(cpu_times[3]));

        per_cpu_arr->Set(i, cpu_obj->Clone());
        i++;

    }

    NanReturnValue(per_cpu_arr);

}

//Handle<Value>
//nsutil_boot_time_sync(const Arguments &args)
NAN_METHOD(nsutil_boot_time_sync)
{
    NanScope();
    NanReturnValue(NanNew<Number>(sutil_boot_time()));
}

//Handle<Value>
//nsutil_disk_partitions_sync(const Arguments &args) 
NAN_METHOD(nsutil_disk_partitions_sync) 
{
    NanScope();

    vector<vector<string>> disk_partitions;
    if (sutil_disk_partitions(disk_partitions) == -1) {
        NanReturnUndefined();
    }

    Local<Array> dps_arr = NanNew<Array>(disk_partitions.size());
    size_t i = 0;
    Local<Object> dps_obj = NanNew<Object>();

    for (auto &dps : disk_partitions) {
        
        dps_obj->Set(NanNew("device"),
                NanNew(dps[0].c_str()));
        dps_obj->Set(NanNew("mount_point"),
                NanNew(dps[1].c_str()));
        dps_obj->Set(NanNew("fs_type"),
                NanNew(dps[2].c_str()));
        dps_obj->Set(NanNew("options"),
                NanNew(dps[3].c_str()));
        dps_arr->Set(i, dps_obj->Clone());
        i++;
    }

    NanReturnValue(dps_arr);

}

//sutil_proc_connections(const int32_t &pid,
//Handle<Value>
//nsutil_proc_connections_sync(const Arguments &args)
NAN_METHOD(nsutil_proc_connections_sync)
{
    NanScope();
    if (args.Length() != 3) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }
    if (!args[1]->IsArray()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }
    if (!args[2]->IsArray()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    int32_t pid = args[0]->Int32Value();

    Local<Object> af_filter_obj = args[1]->ToObject();
    uint32_t len = af_filter_obj->Get(NanNew("length"))->ToObject()->Uint32Value();
    vector<int> af_filter(len);
    for (uint32_t i = 0; i < len; i++) {
        af_filter.push_back(af_filter_obj->Get(i)->ToObject()->Uint32Value());
    }

    Local<Object> type_filter_obj = args[2]->ToObject();
    len = type_filter_obj->Get(NanNew("length"))->ToObject()->Uint32Value();
    vector<int> type_filter(len);
    for (uint32_t i = 0; i < len; i++) {
        type_filter.push_back(type_filter_obj->Get(i)->ToObject()->Uint32Value());
    }
    
    vector<proc_conn> proc_conn_list;
    sutil_proc_connections(pid, af_filter, type_filter, proc_conn_list);

    if (proc_conn_list.empty()) {
        NanReturnUndefined();
    }

    Local<Array> proc_conn_list_arr = NanNew<Array>(proc_conn_list.size());

    size_t i = 0;
    Local<Object> conn_obj = NanNew<Object>();
    for (auto &conn : proc_conn_list) {
        conn_obj->Set(NanNew("fd"),
                NanNew<Integer>(conn.fd));
        conn_obj->Set(NanNew("family"),
                NanNew<Integer>(conn.family));
        conn_obj->Set(NanNew("type"),
                NanNew<Integer>(conn.type));
        if (conn.family == 1) {
            conn_obj->Set(NanNew("laddr"),
                NanNew(conn.addr.c_str()));
            conn_obj->Set(NanNew("raddr"),
                NanNew(conn.caddr.c_str()));
        } else if (conn.family == 2 || conn.family == 10) {
            Local<Array> laddr = NanNew<Array>();
            laddr->Set(0, NanNew(conn.laddr_ip.c_str()));
            laddr->Set(1, NanNew<Integer>(conn.laddr_port));
            conn_obj->Set(NanNew("laddr"), laddr);
            Local<Array> raddr = NanNew<Array>();
            raddr->Set(0, NanNew(conn.raddr_ip.c_str()));
            raddr->Set(1, NanNew<Integer>(conn.raddr_port));
            conn_obj->Set(NanNew("raddr"), laddr);
        }
        proc_conn_list_arr->Set(i, conn_obj->Clone());
        i++;
    }

    NanReturnValue(proc_conn_list_arr);
}


//Handle<Value>
//nsutil_net_io_counters_sync(const Arguments &args)
NAN_METHOD(nsutil_net_io_counters_sync)
{
    NanScope();
    map<string, vector<uint64_t>> net_io_counters;
    if (sutil_net_io_counters(net_io_counters) == -1) {
        NanReturnUndefined();
    }

    Local<Object> net_io_objs = NanNew<Object>();

    Local<Object> io_obj = NanNew<Object>();
    for (auto &net_io : net_io_counters) {
        
        io_obj->Set(NanNew("obytes"), 
                NanNew<Number>(net_io.second[0]));
        io_obj->Set(NanNew("ibytes"), 
                NanNew<Number>(net_io.second[1]));
        io_obj->Set(NanNew("opkts"), 
                NanNew<Number>(net_io.second[2]));
        io_obj->Set(NanNew("ipkts"), 
                NanNew<Number>(net_io.second[3]));
        io_obj->Set(NanNew("oerrs"), 
                NanNew<Number>(net_io.second[4]));
        io_obj->Set(NanNew("ierrs"), 
                NanNew<Number>(net_io.second[5]));
        io_obj->Set(NanNew("iqdrops"), 
                NanNew<Number>(net_io.second[6]));

        net_io_objs->Set(NanNew(net_io.first.c_str()), io_obj->Clone());

    }

    NanReturnValue(net_io_objs);
}

//Handle<Value>
//nsutil_disk_io_counters_sync(const Arguments &args)
NAN_METHOD(nsutil_disk_io_counters_sync)
{
    NanScope();
    map<string, vector<uint64_t>> disk_io_counters;
    if (sutil_disk_io_counters(disk_io_counters) == -1) {
        NanReturnUndefined(); 
    }

    Local<Object> disks_list = NanNew<Object>();
    Local<Object> disk_obj = NanNew<Object>();
    for (auto &disk : disk_io_counters) {
        disk_obj->Set(NanNew("reads"),
                NanNew<Number>(disk.second[0]));
        disk_obj->Set(NanNew("writes"),
                NanNew<Number>(disk.second[1]));
        disk_obj->Set(NanNew("read_bytes"),
                NanNew<Number>(disk.second[2]));
        disk_obj->Set(NanNew("write_bytes"),
                NanNew<Number>(disk.second[3]));
        disk_obj->Set(NanNew("read_time"),
                NanNew<Number>(disk.second[4]));
        disk_obj->Set(NanNew("write_time"),
                NanNew<Number>(disk.second[5]));
        disks_list->Set(NanNew(disk.first.c_str()), disk_obj->Clone());
    }

    NanReturnValue(disks_list);
}

//Handle<Value>
//nsutil_users_sync(const Arguments &args)
NAN_METHOD(nsutil_users_sync)
{
    NanScope();
    vector<sutil_user_info> user_list;
    if (sutil_users(user_list) == -1) {
        NanReturnUndefined();
    }

    Local<Array> user_arr = NanNew<Array>(user_list.size());

    size_t i = 0;
    Local<Object> user_obj = NanNew<Object>();
    for (auto &user : user_list) {
        user_obj->Set(NanNew("username"), 
                NanNew(user.username.c_str()));
        user_obj->Set(NanNew("tty"), 
                NanNew(user.tty.c_str()));
        user_obj->Set(NanNew("host"), 
                NanNew(user.host.c_str()));
        user_obj->Set(NanNew("startTime"), 
                NanNew<Number>(user.start_time));
        user_arr->Set(i, user_obj->Clone());
        i++;
    }
    NanReturnValue(user_arr);

}













