#include <vector>
#include <string>
#include <iostream>
#include <sys/sysctl.h>
#include <libproc.h>

#ifndef _PROCESS_INFO_H_
#define _PROCESS_INFO_H_

int sutil_pid_exists(long pid);
int sutil_get_kinfo_proc(pid_t pid, struct kinfo_proc *kp);
int sutil_get_proc_list(struct kinfo_proc **procList, size_t &procCount);
int sutil_get_argmax(void);
int sutil_proc_pidinfo(long pid, int flavor, void *pti, int size);
std::vector<std::string> sutil_get_arg_list(long pid);

#endif