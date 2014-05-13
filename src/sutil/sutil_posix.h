#include <errno.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <iostream>

int sutil_posix_getpriority(const int32_t &pid, int &priority);
int sutil_posix_setpriority(const int32_t &pid, const int &priority);
