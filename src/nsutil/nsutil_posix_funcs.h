#include <node.h>
#include <v8.h>
#include "../sutil/sutil_posix.h"

v8::Handle<v8::Value>
nsutil_posix_getpriority_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_posix_setpriority_sync(const v8::Arguments &args);

v8::Handle<v8::Value>
nsutil_posix_statvfs(const v8::Arguments &args);
