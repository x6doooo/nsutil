#include "sutil_posix.h"

/*
 * Given a PID return process priority as a integer.
 */
using namespace std;
int
sutil_posix_getpriority(const int32_t &pid, int &priority)
{
    errno = 0;
    priority = getpriority(PRIO_PROCESS, pid);
    if (errno != 0) {
        cout << "os error" << endl;
        return -1;
    }
    return 0;
}

/*
 * Given a PID and a value change process priority.
 */
int
sutil_posix_setpriority(const int32_t &pid, const int &priority)
{
    int retval;
    retval = setpriority(PRIO_PROCESS, pid, priority);
    if (retval == -1) {
        cout << "os error" << endl;
        return -1;
    }
    return 0;
}
