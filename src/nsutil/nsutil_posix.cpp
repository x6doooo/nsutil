#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#include <node.h>
#include <v8.h>
#include "nsutil_posix_funcs.h"

using v8::FunctionTemplate;
using v8::String;

void Init(v8::Handle<v8::Object> exports) {
    exports->Set(NanNew<String>("nsutil_posix_getpriority_sync"), 
            NanNew<FunctionTemplate>(nsutil_posix_getpriority_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_posix_setpriority_sync"), 
            NanNew<FunctionTemplate>(nsutil_posix_setpriority_sync)->GetFunction());

    exports->Set(NanNew<String>("nsutil_posix_statvfs"),
            NanNew<FunctionTemplate>(nsutil_posix_statvfs)->GetFunction());
}


#ifdef __APPLE__
NODE_MODULE(nsutil_posix_osx, Init);
#else
NODE_MODULE(nsutil_posix_linux, Init);
#endif

