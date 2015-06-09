#include "./nsutil_osx_funcs_async.h"

using namespace v8;
using namespace std;

template<typename T>
struct Baton {
    NanCallback *callback;
    bool error;
    std::string error_message;
    T result;
};

void cpu_times(uv_work_t* req) {
    Baton<double*>* baton = static_cast<Baton<double*>*>(req->data);
    double *cpu_times = new double[4];
    if (sutil_cpu_times(cpu_times) == -1) {
        baton->error = true;
        baton->error_message = "sutil_cpu_times error";
    }
    baton->result = cpu_times;
}

void cpu_times_after(uv_work_t* req) {
    NanScope();
    Baton<double*>* baton = static_cast<Baton<double*>*>(req->data);
    if (baton->error) {
        Local<Value> err = Exception::Error(NanNew<String>(baton->error_message.c_str()));

        const unsigned argc = 1;
        Local<Value> argv[argc] = { err };

        TryCatch try_catch;

        baton->callback->Call(argc, argv);
        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
    } else {
         
        const unsigned argc = 2;
        Local<Object> cpu_obj = NanNew<Object>();
        cpu_obj->Set(NanNew("user"),
                NanNew<Number>(baton->result[0]));
        cpu_obj->Set(NanNew("nice"),
                NanNew<Number>(baton->result[1]));
        cpu_obj->Set(NanNew("sys"),
                NanNew<Number>(baton->result[2]));
        cpu_obj->Set(NanNew("idle"),
                NanNew<Number>(baton->result[3]));

        Local<Value> argv[argc] = {
            NanNull(),
            NanNew<Value>(cpu_obj)
        };
        TryCatch try_catch;
        baton->callback->Call(argc, argv);
        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
    }
    //释放
    delete baton->callback;
    delete[] baton->result;
    delete baton;
    delete req;
}


NAN_METHOD(nsutil_cpu_times_async)
{
    NanScope();

    if (args.Length() != 1) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }

    if (!args[0]->IsFunction()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    Local<Function> callback = args[0].As<Function>();
   
    Baton<double*>* baton = new Baton<double*>();
    baton->error = false;
    baton->callback = new NanCallback(callback);
        
    uv_work_t *req = new uv_work_t();
    req->data = baton;
    
    int status = uv_queue_work(uv_default_loop(), req, cpu_times,
                               (uv_after_work_cb)cpu_times_after);
    assert(status == 0);
    NanReturnUndefined();
}


void per_cpu_times(uv_work_t* req) {
    Baton<vector<vector<double>>>* baton = static_cast<Baton<vector<vector<double>>>*>(req->data);
    vector<vector<double>> per_cpu_times;

    if (sutil_per_cpu_times(per_cpu_times) == -1) {
        baton->error = true;
        baton->error_message = "sutil_per_cpu_times error";
    }

    baton->result = per_cpu_times;

}

void per_cpu_times_after(uv_work_t* req) {
    NanScope();
    Baton<vector<vector<double>>>* baton = static_cast<Baton<vector<vector<double>>>*>(req->data);
    if (baton->error) {
        Local<Value> err = Exception::Error(NanNew<String>(baton->error_message.c_str()));

        const unsigned argc = 1;
        Local<Value> argv[argc] = { err };

        TryCatch try_catch;

        baton->callback->Call(argc, argv);
        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
    } else {
         
        const unsigned argc = 2;

        Local<Object> cpu_obj = NanNew<Object>();

        size_t i = 0;

        Local<Array> per_cpu_arr = NanNew<Array>(baton->result.size());

        for (auto &cpu_times : baton->result) {
        
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

        Local<Value> argv[argc] = {
            NanNull(),
            NanNew<Value>(per_cpu_arr)
        };
        TryCatch try_catch;
        baton->callback->Call(argc, argv);
        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
    }
    //释放
    delete baton->callback;
    delete baton;
    delete req;
}

NAN_METHOD(nsutil_per_cpu_times_async)
{
    NanScope();
    if (args.Length() != 1) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }

    if (!args[0]->IsFunction()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    Local<Function> callback = args[0].As<Function>();
   
    Baton<vector<vector<double>>>* baton = new Baton<vector<vector<double>>>();
    baton->error = false;
    baton->callback = new NanCallback(callback);
        
    uv_work_t *req = new uv_work_t();
    req->data = baton;
    
    int status = uv_queue_work(uv_default_loop(), req, per_cpu_times,
                               (uv_after_work_cb)per_cpu_times_after);
    assert(status == 0);
    NanReturnUndefined();


}

void swap_mem(uv_work_t* req) {
    Baton<uint64_t*>* baton = static_cast<Baton<uint64_t*>*>(req->data);

    uint64_t *swap_mem = new uint64_t[5];
    if (sutil_swap_mem(swap_mem) == -1) {
        baton->error = true;
        baton->error_message = "sutil_swap_mem error";
    }

    baton->result = swap_mem;
}

void swap_mem_after(uv_work_t* req) {
    NanScope();
    Baton<uint64_t*>* baton = static_cast<Baton<uint64_t*>*>(req->data);
    if (baton->error) {
        Local<Value> err = Exception::Error(NanNew<String>(baton->error_message.c_str()));

        const unsigned argc = 1;
        Local<Value> argv[argc] = { err };

        TryCatch try_catch;

        baton->callback->Call(argc, argv);
        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
    } else {
         
        const unsigned argc = 2;

        Local<Object> swap_obj = NanNew<Object>();
    
        swap_obj->Set(NanNew("total"),
                NanNew<Number>(baton->result[0]));
        swap_obj->Set(NanNew("used"),
                NanNew<Number>(baton->result[1]));
        swap_obj->Set(NanNew("free"),
                NanNew<Number>(baton->result[2]));
        swap_obj->Set(NanNew("sin"),
                NanNew<Number>(baton->result[3]));
        swap_obj->Set(NanNew("sout"),
                NanNew<Number>(baton->result[4]));


        Local<Value> argv[argc] = {
            NanNull(),
            NanNew<Value>(swap_obj)
        };
        TryCatch try_catch;
        baton->callback->Call(argc, argv);
        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
    }
    //释放
    delete baton->callback;
    delete[] baton->result;
    delete baton;
    delete req;
}


NAN_METHOD(nsutil_swap_mem_async)
{
    NanScope();
    if (args.Length() != 1) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }

    if (!args[0]->IsFunction()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnUndefined();
    }

    Local<Function> callback = args[0].As<Function>();
   
    Baton<uint64_t*>* baton = new Baton<uint64_t*>();
    baton->error = false;
    baton->callback = new NanCallback(callback);
        
    uv_work_t *req = new uv_work_t();
    req->data = baton;
    
    int status = uv_queue_work(uv_default_loop(), req, swap_mem,
                               (uv_after_work_cb)swap_mem_after);
    assert(status == 0);
    NanReturnUndefined();
}







