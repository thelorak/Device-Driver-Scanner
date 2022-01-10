#pragma once
#pragma comment (lib, "Setupapi.lib")
#include <setupapi.h>
static HDEVINFO hDevInfo; // device information set
static SP_DEVINFO_DATA* pspDevInfoData; // singular device
static SP_PROPCHANGE_PARAMS spPropChangeParams; // property of device