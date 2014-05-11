#include "./sutil_common.h"

using namespace std;

static string sutil_error_desc;

int NoSuchProcess(void) {
    sutil_error_desc = strerror(ESRCH);
    return -1;
}

int AccessDenied(void) {
    sutil_error_desc = strerror(EACCES);
    return -1;
}

int sutil_set_err(string desc) {
    sutil_error_desc = desc;
    return -1;
}

void sutil_clean_err() {
    sutil_error_desc.clear();
}

string sutil_get_err_desc() {
    return sutil_error_desc;
}

