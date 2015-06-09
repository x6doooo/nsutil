#include "nsutil_linux_funcs.h"

using namespace v8;
using namespace std;

#if SUTIL_HAVE_IOPRIO

NAN_METHOD(nsutil_pid_exists)
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnValue(NanUndefined());
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnValue(NanUndefined());
    }
    int32_t pid = args[0]->Int32Value();
    NanReturnValue(NanNew<Boolean>(sutil_pid_exists(pid)));
}

NAN_METHOD(nsutil_proc_ioprio_get) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnValue(NanUndefined());
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnValue(NanUndefined());
    }
    int32_t pid = args[0]->Int32Value();
    int *proc_ioprio = new int[2];
    if (sutil_proc_ioprio_get(pid, proc_ioprio) == -1) {
        NanReturnValue(NanUndefined());
    }
    Local<Object> obj = NanNew<Object>();
    obj->Set(NanNew("class"), NanNew<Integer>(proc_ioprio[0]));
    obj->Set(NanNew("data"), NanNew<Integer>(proc_ioprio[1]));

    delete[] proc_ioprio;
    NanReturnValue(obj);
}

NAN_METHOD(nsutil_proc_ioprio_set) 
{
    NanScope();
    if (args.Length() != 3) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnValue(NanUndefined());
    }
    if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnValue(NanUndefined());
    }

    int32_t pid = args[0]->Int32Value();
    int io_class = args[1]->IntegerValue();
    int io_data = args[2]->IntegerValue();

    int re = sutil_proc_ioprio_set(pid, io_class, io_data);
    NanReturnValue(NanNew<Integer>(re));
}

#endif


#if SUTIL_HAVE_PRLIMIT

//XXX: TODO: get set
NAN_METHOD(nsutil_linux_prlimit) 
{
    NanScope();
    NanReturnValue(NanUndefined());
}

#endif


NAN_METHOD(nsutil_disk_partitions)
{
    NanScope();

    vector<vector<string>> devlist;

    if (sutil_disk_partitions(devlist) == -1) {
        NanReturnValue(NanUndefined());
    }
    
    Local<Array> arr = NanNew<Array>(devlist.size());

    int i = 0;
    Local<Object> obj = NanNew<Object>();
    for (auto &dev : devlist) {
        obj->Set(NanNew("device"),
                NanNew(dev[0].c_str()));
        obj->Set(NanNew("mountpoint"),
                NanNew(dev[1].c_str()));
        obj->Set(NanNew("fstype"),
                NanNew(dev[2].c_str()));
        obj->Set(NanNew("opts"),
                NanNew(dev[3].c_str()));
        arr->Set(i, obj->Clone());
        i++;
    }


    NanReturnValue(arr);
}

//int
//sutil_linux_sysinfo(uint64_t* &info)
NAN_METHOD(nsutil_sysinfo)
{
    NanScope();
    
    uint64_t* info = new uint64_t[6]; 

    if (sutil_linux_sysinfo(info) == -1) {
        NanReturnValue(NanUndefined());
    }

    Local<Object> obj = NanNew<Object>();

    obj->Set(NanNew("total"), 
            NanNew<Number>(info[0]));
    obj->Set(NanNew("free"), 
            NanNew<Number>(info[1]));
    obj->Set(NanNew("buffer"), 
            NanNew<Number>(info[2]));
    obj->Set(NanNew("shared"), 
            NanNew<Number>(info[3]));
    obj->Set(NanNew("swap_total"), 
            NanNew<Number>(info[4]));
    obj->Set(NanNew("swap_free"), 
            NanNew<Number>(info[5]));

    delete[] info;
    NanReturnValue(obj);
}

//int
//sutil_proc_cpu_affinity_get(const int32_t pid, uint32_t &mask)
NAN_METHOD(nsutil_proc_cpu_affinity_get)
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnValue(NanUndefined());
    }
    if (!args[0]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnValue(NanUndefined());
    }
    int32_t pid = args[0]->Int32Value();
    uint32_t mask;
    
    if (sutil_proc_cpu_affinity_get(pid, mask) == -1) {
        NanReturnValue(NanUndefined());
    }

    NanReturnValue(NanNew<Number>(mask));
}

//XXX
//int
//sutil_proc_cpu_affinity_set(const int32_t &pid, vector<int32_t> &cpu_set_list)
/*
NAN_METHOD(nsutil_proc_cpu_affinity_set) 
{
    NanScope(;

    if (args.Length() != 2) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnValue(NanUndefined());
    }
    if (!args[0]->IsNumber() || !args[1]->IsArray()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnValue(NanUndefined());
    }

    int32_t pid = args[0]->Int32Value();


    //TODO

    NanReturnValue(NanUndefined());

}
*/




//int
//sutil_users(vector<sutil_user_info> &user_list)
NAN_METHOD(nsutil_users)
{
    NanScope();

    vector<sutil_user_info> user_list;

    if (sutil_users(user_list) == -1) {
        NanReturnValue(NanUndefined());
    }
    
    Local<Array> arr = NanNew<Array>(user_list.size());
    Local<Object> obj = NanNew<Object>();

    int i = 0;
    for (auto &u : user_list) {
        obj->Set(NanNew("username"), NanNew(u.username.c_str()));
        obj->Set(NanNew("tty"), NanNew(u.tty.c_str()));
        obj->Set(NanNew("host"), NanNew(u.host.c_str()));
        obj->Set(NanNew("start_time"), NanNew<Number>(u.start_time));
        obj->Set(NanNew("user_proc"), NanNew<Boolean>(u.user_proc));
        arr->Set(i, obj->Clone());
        i++;
    }

    NanReturnValue(arr);
}



// sysconf
NAN_METHOD(nsutil_sysconf) 
{
    NanScope();
    if (args.Length() == 0) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnValue(NanUndefined());
    }
    if (!args[0]->IsString()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnValue(NanUndefined());
    }
    String::Utf8Value str(args[0]->ToString());
    string which = *str;
    
    int ret = sutil_sysconf(which);
    NanReturnValue(NanNew<Number>(ret));
}









