#include "nsutil_linux_funcs.h"

using namespace v8;
using namespace std;

#if SUTIL_HAVE_IOPRIO

Handle<Value>
nsutil_proc_ioprio_get(const Arguments &args) 
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
    int *proc_ioprio = new int[2];
    if (sutil_proc_ioprio_get(pid, proc_ioprio) == -1) {
        return scope.Close(Undefined());
    }
    Local<Object> obj = Object::New();
    obj->Set(String::NewSymbol("class"), Integer::New(proc_ioprio[0]));
    obj->Set(String::NewSymbol("data"), Integer::New(proc_ioprio[1]));

    delete[] proc_ioprio;
    return scope.Close(obj);
}

Handle<Value>
nsutil_proc_ioprio_set(const Arguments &args) 
{
    HandleScope scope;
    if (args.Length() != 3) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int32_t pid = args[0]->Int32Value();
    int io_class = args[1]->IntegerValue();
    int io_data = args[2]->IntegerValue();

    int re = sutil_proc_ioprio_set(pid, io_class, io_data);
    return scope.Close(Integer::New(re));
}

#endif


#if SUTIL_HAVE_PRLIMIT

//XXX: TODO: get set
Handle<Value>
nsutil_linux_prlimit(const Arguments &args) 
{
    HandleScope scope;
    return scope.Close(Undefined());
}

#endif


Handle<Value>
nsutil_disk_partitions(const Arguments &args)
{
    HandleScope scope;

    vector<vector<string>> devlist;

    if (sutil_disk_partitions(devlist) == -1) {
        return scope.Close(Undefined());
    }
    
    Local<Array> arr = Array::New(devlist.size());

    int i = 0;
    for (auto &dev : devlist) {
        Local<Object> obj = Object::New();
        obj->Set(String::NewSymbol("device"),
                String::New(dev[0].c_str()));
        obj->Set(String::NewSymbol("mountpoint"),
                String::New(dev[1].c_str()));
        obj->Set(String::NewSymbol("fstype"),
                String::New(dev[2].c_str()));
        obj->Set(String::NewSymbol("opts"),
                String::New(dev[3].c_str()));
        arr->Set(i, obj);
        i++;
    }


    return scope.Close(arr);
}

//int
//sutil_linux_sysinfo(uint64_t* &info)
Handle<Value>
nsutil_sysinfo(const Arguments &args)
{
    HandleScope scope;
    
    uint64_t* info = new uint64_t[6]; 

    if (sutil_linux_sysinfo(info) == -1) {
        return scope.Close(Undefined());
    }

    Local<Object> obj = Object::New();

    obj->Set(String::NewSymbol("total"), 
            Number::New(info[0]));
    obj->Set(String::NewSymbol("free"), 
            Number::New(info[1]));
    obj->Set(String::NewSymbol("buffer"), 
            Number::New(info[2]));
    obj->Set(String::NewSymbol("shared"), 
            Number::New(info[3]));
    obj->Set(String::NewSymbol("swap_total"), 
            Number::New(info[4]));
    obj->Set(String::NewSymbol("swap_free"), 
            Number::New(info[5]));

    delete[] info;
    return scope.Close(obj);
}

//int
//sutil_proc_cpu_affinity_get(const int32_t pid, uint32_t &mask)
Handle<Value>
nsutil_proc_cpu_affinity_get(const Arguments &args)
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
    uint32_t mask;
    
    if (sutil_proc_cpu_affinity_get(pid, mask) == -1) {
        return scope.Close(Undefined());
    }

    return scope.Close(Number::New(mask));
}

//XXX TODO
//int
//sutil_proc_cpu_affinity_set(const int32_t &pid, vector<int32_t> &cpu_set_list)



//int
//sutil_users(vector<sutil_user_info> &user_list)
Handle<Value>
nsutil_users(const Arguments &args)
{
    HandleScope scope;

    vector<sutil_user_info> user_list;

    if (sutil_users(user_list) == -1) {
        return scope.Close(Undefined());
    }
    
    Local<Array> arr = Array::New(user_list.size());

    int i = 0;
    for (auto &u : user_list) {
        Local<Object> obj = Object::New();
        obj->Set(String::NewSymbol("username"), String::New(u.username));
        obj->Set(String::NewSymbol("tty"), String::New(u.tty));
        obj->Set(String::NewSymbol("host"), String::New(u.host));
        obj->Set(String::NewSymbol("start_time"), Number::New(u.start_time));
        obj->Set(String::NewSymbol("user_proc"), Boolean::New(u.user_proc););
        arr->Set(i, obj);
        i++;
    }

    return scope.Close(arr);
}













