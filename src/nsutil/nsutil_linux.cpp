#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#include <node.h>
#include <nan.h>
#include <v8.h>
#include "nsutil_linux_funcs.h"

using v8::FunctionTemplate;
using v8::String;

void Init(v8::Handle<v8::Object> exports) {
#if SUTIL_HAVE_IOPRIO
    exports->Set(NanNew<String>("nsutil_proc_ioprio_get"), 
            NanNew<FunctionTemplate>(nsutil_proc_ioprio_get)->GetFunction());
    exports->Set(NanNew<String>("nsutil_proc_ioprio_set"), 
            NanNew<FunctionTemplate>(nsutil_proc_ioprio_set)->GetFunction());
#endif

    exports->Set(NanNew<String>("nsutil_disk_partitions"), 
            NanNew<FunctionTemplate>(nsutil_disk_partitions)->GetFunction());

    exports->Set(NanNew<String>("nsutil_sysinfo"), 
            NanNew<FunctionTemplate>(nsutil_sysinfo)->GetFunction());

    exports->Set(NanNew<String>("nsutil_proc_cpu_affinity_get"), 
            NanNew<FunctionTemplate>(nsutil_proc_cpu_affinity_get)->GetFunction());

    exports->Set(NanNew<String>("nsutil_users"), 
            NanNew<FunctionTemplate>(nsutil_users)->GetFunction());

    exports->Set(NanNew<String>("nsutil_pid_exists"), 
            NanNew<FunctionTemplate>(nsutil_pid_exists)->GetFunction());

    exports->Set(NanNew<String>("nsutil_sysconf"), 
            NanNew<FunctionTemplate>(nsutil_sysconf)->GetFunction());

    /*
    exports->Set(NanNew<String>("nsutil_"), 
            NanNew<FunctionTemplate>(nsutil_)->GetFunction());

            */

}

NODE_MODULE(nsutil_linux, Init);
