#include "./nsutil_osx_funcs_async.h"

using namespace v8;
using namespace std;

//定义一个结构体，存储异步请求信息
template<typename T>
struct Baton {
    Persistent<Function> callback;

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
    HandleScope scope;
    Baton<double*>* baton = static_cast<Baton<double*>*>(req->data);
    if (baton->error) {
        Local<Value> err = Exception::Error(String::New(baton->error_message.c_str()));

        // 准备回调函数的参数
        const unsigned argc = 1;
        Local<Value> argv[argc] = { err };

        //捕捉回调中的错误，在Node环境中可利用process.on('uncaughtException')捕捉错误 
        TryCatch try_catch;

        baton->callback->Call(Context::GetCurrent()->Global(), argc, argv);
        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
    } else {
         
        const unsigned argc = 2;
        Local<Object> cpu_obj = Object::New();
        cpu_obj->Set(String::NewSymbol("user"),
                Number::New(baton->result[0]));
        cpu_obj->Set(String::NewSymbol("nice"),
                Number::New(baton->result[1]));
        cpu_obj->Set(String::NewSymbol("sys"),
                Number::New(baton->result[2]));
        cpu_obj->Set(String::NewSymbol("idle"),
                Number::New(baton->result[3]));

        Local<Value> argv[argc] = {
            Local<Value>::New(Null()),
            cpu_obj
        };
        TryCatch try_catch;
        baton->callback->Call(Context::GetCurrent()->Global(), argc, argv);
        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
    }
    //释放
    baton->callback.Dispose();
    delete[] baton->result;
    delete baton;
    delete req;
}


Handle<Value>
nsutil_cpu_times_async(const Arguments &args)
{
    HandleScope scope;

    if (args.Length() != 1) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return Undefined();
    }

    if (!args[0]->IsFunction()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return Undefined();
    }

    Local<Function> callback = Local<Function>::Cast(args[0]);
   
    Baton<double*>* baton = new Baton<double*>();
    baton->error = false;
    baton->callback = Persistent<Function>::New(callback);
        
    uv_work_t *req = new uv_work_t();
    req->data = baton;
    
    int status = uv_queue_work(uv_default_loop(), req, cpu_times,
                               (uv_after_work_cb)cpu_times_after);
    assert(status == 0);
    return Undefined();
}


void per_cpu_times(uv_work_t* req) {
    Baton<vector<vector<double>>>* baton = static_cast<Baton<vector<vector<double>>>*>(req->data);
    vector<vector<double>> per_cpu_times;

    if (sutil_per_cpu_times(per_cpu_times) == -1) {
    }

    baton->result = per_cpu_times;

}

void per_cpu_times_after(uv_work_t* req) {
    HandleScope scope;
    Baton<vector<vector<double>>>* baton = static_cast<Baton<vector<vector<double>>>*>(req->data);
    if (baton->error) {
        Local<Value> err = Exception::Error(String::New(baton->error_message.c_str()));

        // 准备回调函数的参数
        const unsigned argc = 1;
        Local<Value> argv[argc] = { err };

        //捕捉回调中的错误，在Node环境中可利用process.on('uncaughtException')捕捉错误 
        TryCatch try_catch;

        baton->callback->Call(Context::GetCurrent()->Global(), argc, argv);
        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
    } else {
         
        const unsigned argc = 2;

        Local<Object> cpu_obj = Object::New();

        size_t i = 0;

        Local<Array> per_cpu_arr = Array::New(baton->result.size());

        for (auto &cpu_times : baton->result) {
        
            cpu_obj->Set(String::NewSymbol("user"),
                    Number::New(cpu_times[0]));
            cpu_obj->Set(String::NewSymbol("nice"),
                    Number::New(cpu_times[1]));
            cpu_obj->Set(String::NewSymbol("sys"),
                    Number::New(cpu_times[2]));
            cpu_obj->Set(String::NewSymbol("idle"),
                    Number::New(cpu_times[3]));

            per_cpu_arr->Set(i, cpu_obj->Clone());
            i++;
        
        }

        Local<Value> argv[argc] = {
            Local<Value>::New(Null()),
            per_cpu_arr
        };
        TryCatch try_catch;
        baton->callback->Call(Context::GetCurrent()->Global(), argc, argv);
        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
    }
    //释放
    baton->callback.Dispose();
    delete baton;
    delete req;
}

Handle<Value>
nsutil_per_cpu_times_async(const Arguments &args)
{
    HandleScope scope;
    if (args.Length() != 1) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return Undefined();
    }

    if (!args[0]->IsFunction()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return Undefined();
    }

    Local<Function> callback = Local<Function>::Cast(args[0]);
   
    Baton<vector<vector<double>>>* baton = new Baton<vector<vector<double>>>();
    baton->error = false;
    baton->callback = Persistent<Function>::New(callback);
        
    uv_work_t *req = new uv_work_t();
    req->data = baton;
    
    int status = uv_queue_work(uv_default_loop(), req, per_cpu_times,
                               (uv_after_work_cb)per_cpu_times_after);
    assert(status == 0);
    return Undefined();


}


