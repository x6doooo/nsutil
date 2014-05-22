#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <v8.h>
#include "nsutil_posix_funcs.h"

void Init(v8::Handle<v8::Object> exports) {
    exports->Set(v8::String::NewSymbol("nsutil_posix_getpriority_sync"), 
            v8::FunctionTemplate::New(nsutil_posix_getpriority_sync)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_posix_setpriority_sync"), 
            v8::FunctionTemplate::New(nsutil_posix_setpriority_sync)->GetFunction());
}


#ifdef __APPLE__
NODE_MODULE(nsutil_posix_osx, Init);
#else
NODE_MODULE(nsutil_posix_linux, Init);
#endif

