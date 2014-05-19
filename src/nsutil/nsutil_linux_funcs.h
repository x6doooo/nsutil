#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <v8.h>
#include "../sutil/sutil_linux.h"

v8::Handle<v8::Value>
nsutil_proc_ioprio_get(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_proc_ioprio_set(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_disk_partitions(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_sysinfo(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_proc_cpu_affinity_get(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_users(const v8::Arguments &args);




