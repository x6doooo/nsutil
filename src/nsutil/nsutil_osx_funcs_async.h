#include <node.h>
#include <v8.h>
#include "../sutil/osx/process_info.h"
#include "../sutil/sutil_osx.h"

v8::Handle<v8::Value>
nsutil_cpu_times_async(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_per_cpu_times_async(const v8::Arguments &args);

