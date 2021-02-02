#pragma once
#include "pch.h"

static void* GiveOrdersAddress;
void HookGiveOrdersCall();
static void __stdcall GiveOrders(DWORD orderAddress, DWORD unitPointer, DWORD param1, DWORD param2, DWORD param3);