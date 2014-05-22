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
