#include <iostream>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int sutil_set_err(std::string desc);
int NoSuchProcess(void);
int AccessDenied(void);
void sutil_clean_err();
std::string sutil_get_err_desc();

