#include <node.h>
#include <nan.h>
#include <v8.h>
#include "../sutil/osx/process_info.h"
#include "../sutil/sutil_osx.h"

NAN_METHOD(nsutil_cpu_times_async);

NAN_METHOD(nsutil_per_cpu_times_async);

NAN_METHOD(nsutil_swap_mem_async);

