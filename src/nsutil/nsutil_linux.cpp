#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <v8.h>
#include "nsutil_linux_funcs.h"

void Init(v8::Handle<v8::Object> exports) {
#if SUTIL_HAVE_IOPRIO
    exports->Set(v8::String::NewSymbol("nsutil_proc_ioprio_get"), 
            v8::FunctionTemplate::New(nsutil_proc_ioprio_get)->GetFunction());
    exports->Set(v8::String::NewSymbol("nsutil_proc_ioprio_set"), 
            v8::FunctionTemplate::New(nsutil_proc_ioprio_set)->GetFunction());
#endif

    exports->Set(v8::String::NewSymbol("nsutil_disk_partitions"), 
            v8::FunctionTemplate::New(nsutil_disk_partitions)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_sysinfo"), 
            v8::FunctionTemplate::New(nsutil_sysinfo)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_proc_cpu_affinity_get"), 
            v8::FunctionTemplate::New(nsutil_proc_cpu_affinity_get)->GetFunction());

    exports->Set(v8::String::NewSymbol("nsutil_users"), 
            v8::FunctionTemplate::New(nsutil_users)->GetFunction());

    /*
    exports->Set(v8::String::NewSymbol("nsutil_"), 
            v8::FunctionTemplate::New(nsutil_)->GetFunction());

            */

}

NODE_MODULE(nsutil_linux, Init);
