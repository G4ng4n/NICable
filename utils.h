#pragma once

#include <Windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <devguid.h>
#include <cfgmgr32.h>
#include <setupapi.h>

#pragma comment(lib, "cfgmgr32.lib")
#pragma comment(lib, "setupapi.lib")

#define NIC_CONTROL_ENABLE 0
#define NIC_CONTROL_DISABLE 1

int check_admin();
int enum_NIC();
int set_NIC(int, int);
void test();
