#include "nsutil_posix_funcs.h"

using namespace std;
using namespace v8;

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

Handle<Value>
nsutil_posix_statvfs(const Arguments &args)
{
    HandleScope scope;
    if (args.Length() != 1) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsString()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    String::Utf8Value str(args[0]->ToString());
    char const *path = *str;
    struct statvfs buffer;
    int status = sutil_posix_statvfs(path, buffer);
    if (status) {
        return scope.Close(Undefined());
    }
    Local<Object> obj = Object::New();
    obj->Set(String::NewSymbol("bsize"), 
            Number::New(buffer.f_bsize));
    obj->Set(String::NewSymbol("frsize"), 
            Number::New(buffer.f_frsize));
    obj->Set(String::NewSymbol("blocks"), 
            Number::New(buffer.f_blocks));
    obj->Set(String::NewSymbol("bfree"), 
            Number::New(buffer.f_bfree));
    obj->Set(String::NewSymbol("bavail"), 
            Number::New(buffer.f_bavail));
    obj->Set(String::NewSymbol("files"), 
            Number::New(buffer.f_files));
    obj->Set(String::NewSymbol("ffree"), 
            Number::New(buffer.f_ffree));
    obj->Set(String::NewSymbol("favail"), 
            Number::New(buffer.f_favail));
    obj->Set(String::NewSymbol("flag"), 
            Number::New(buffer.f_flag));
    obj->Set(String::NewSymbol("namemax"), 
            Number::New(buffer.f_namemax));
    return scope.Close(obj);
}




