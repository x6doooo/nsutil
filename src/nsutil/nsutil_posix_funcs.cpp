#include "nsutil_posix_funcs.h"

using namespace std;
using namespace v8;

// handle sutil_posix
NAN_METHOD(nsutil_posix_getpriority_sync)
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
    int priority;
    if(sutil_posix_getpriority(pid, priority) == -1) {
        NanReturnValue(NanUndefined());
    }

    NanReturnValue(NanNew<Number>(priority));
}

NAN_METHOD(nsutil_posix_setpriority_sync)
{
    NanScope();
    if (args.Length() != 2) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnValue(NanUndefined());
    }
    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnValue(NanUndefined());
    }
    int32_t pid = args[0]->Int32Value();
    int priority = args[0]->IntegerValue();
    if(sutil_posix_setpriority(pid, priority) == -1) {
        NanThrowTypeError("SetPriority Error");
    }

    NanReturnValue(NanUndefined());
}

NAN_METHOD(nsutil_posix_statvfs)
{
    NanScope();
    if (args.Length() != 1) {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnValue(NanUndefined());
    }
    if (!args[0]->IsString()) {
        NanThrowTypeError("Wrong arguments");
        NanReturnValue(NanUndefined());
    }
    String::Utf8Value str(args[0]->ToString());
    char const *path = *str;
    struct statvfs buffer;
    int status = sutil_posix_statvfs(path, buffer);
    if (status) {
        NanReturnValue(NanUndefined());
    }
    Local<Object> obj = NanNew<Object>();
    obj->Set(NanNew("bsize"), 
            NanNew<Number>(buffer.f_bsize));
    obj->Set(NanNew("frsize"), 
            NanNew<Number>(buffer.f_frsize));
    obj->Set(NanNew("blocks"), 
            NanNew<Number>(buffer.f_blocks));
    obj->Set(NanNew("bfree"), 
            NanNew<Number>(buffer.f_bfree));
    obj->Set(NanNew("bavail"), 
            NanNew<Number>(buffer.f_bavail));
    obj->Set(NanNew("files"), 
            NanNew<Number>(buffer.f_files));
    obj->Set(NanNew("ffree"), 
            NanNew<Number>(buffer.f_ffree));
    obj->Set(NanNew("favail"), 
            NanNew<Number>(buffer.f_favail));
    obj->Set(NanNew("flag"), 
            NanNew<Number>(buffer.f_flag));
    obj->Set(NanNew("namemax"), 
            NanNew<Number>(buffer.f_namemax));
    NanReturnValue(obj);
}




