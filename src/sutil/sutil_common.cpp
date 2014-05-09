
#include <iostream>

using namespace std;

void NoSuchProcess(void) {
    cout << strerror(ESRCH) << endl;
}

void AccessDenied(void) {
    cout << strerror(EACCES) << endl;
}
