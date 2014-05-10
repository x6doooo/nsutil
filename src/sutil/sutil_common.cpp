#include <iostream>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "./sutil_common.h"

using namespace std;

void NoSuchProcess(void) {
    cout << strerror(ESRCH) << endl;
}

void AccessDenied(void) {
    cout << strerror(EACCES) << endl;
}
