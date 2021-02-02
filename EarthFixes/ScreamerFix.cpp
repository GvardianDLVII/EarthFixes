#include "pch.h"
#include "ScreamerFix.h"

static BYTE originalOrderCall[] = {
    0x55,									//push	ebp
    0x89, 0xE5,								//mov	ebp, esp
    0x56,									//push	esi
    0x57,									//push	edi
    0x68, 0x00, 0x00, 0x00, 0x00,			//push [param3]
    0x68, 0x00, 0x00, 0x00, 0x00,			//push [param2]
    0x68, 0x00, 0x00, 0x00, 0x00,			//push [param1]
    0xBA, 0x00, 0x00, 0x00, 0x00,			//mov edx, [unitPointer]
    0xBB, 0x00, 0x00, 0x00, 0x00,			//mov ebx, [orderAddress]
    0x89, 0xD1,			                    //mov ecx, edx
    0xFF, 0x93, 0x9C, 0x01, 0x00, 0x00,      //call dword ptr[ebx + 0000019C]
    0x5F,									//pop	edi
    0x5E,									//pop	esi
    0x5D,									//pop	ebp
    0xC3									//ret
};

void TmpOrderCall(DWORD orderAddress, DWORD unitPointer, DWORD param1, DWORD param2, DWORD param3)
{
    BYTE orderBytes[4];
    ToByteArray(orderAddress, orderBytes);
    BYTE unitBytes[4];
    ToByteArray(unitPointer, unitBytes);
    BYTE paramBytes1[4];
    ToByteArray(param1, paramBytes1);
    BYTE paramBytes2[4];
    ToByteArray(param2, paramBytes2);
    BYTE paramBytes3[4];
    ToByteArray(param3, paramBytes3);
    originalOrderCall[6] = paramBytes3[3];
    originalOrderCall[7] = paramBytes3[2];
    originalOrderCall[8] = paramBytes3[1];
    originalOrderCall[9] = paramBytes3[0];
    originalOrderCall[11] = paramBytes2[3];
    originalOrderCall[12] = paramBytes2[2];
    originalOrderCall[13] = paramBytes2[1];
    originalOrderCall[14] = paramBytes2[0];
    originalOrderCall[16] = paramBytes1[3];
    originalOrderCall[17] = paramBytes1[2];
    originalOrderCall[18] = paramBytes1[1];
    originalOrderCall[19] = paramBytes1[0];
    originalOrderCall[21] = unitBytes[3];
    originalOrderCall[22] = unitBytes[2];
    originalOrderCall[23] = unitBytes[1];
    originalOrderCall[24] = unitBytes[0];
    originalOrderCall[26] = orderBytes[3];
    originalOrderCall[27] = orderBytes[2];
    originalOrderCall[28] = orderBytes[1];
    originalOrderCall[29] = orderBytes[0];
    typedef void(_stdcall* originalCall)(void);
    void* originalFunctionPointer = (void*)originalOrderCall;
    originalCall call = (originalCall)(originalFunctionPointer);
    call();
}

void __stdcall GiveOrders(DWORD orderAddress, DWORD unitPointer, DWORD param1, DWORD param2, DWORD param3)
{
    BYTE unitFlags = *((BYTE*)(unitPointer + 0xC3));
    if ((unitFlags & 0x11) == 0) {
        TmpOrderCall(orderAddress, unitPointer, param1, param2, param3);
    }
}

void HookGiveOrdersCall()
{
    GiveOrdersAddress = &GiveOrders;
    const ULONG_PTR injectAddress = 0x004C4663;
    void** proxyFunctionAddress = &GiveOrdersAddress;
    BYTE bytes[4];
    ToByteArray((ULONG)proxyFunctionAddress, bytes);
    BYTE screamerProxy[] = {
        0x52,                                                   //push edx
        0x53,                                                   //push ebx
        0xFF, 0x15, bytes[3], bytes[2], bytes[1], bytes[0]      //call DWRD PTR ds:${proxyAddress}
    };

    WriteProcessMemory(GetCurrentProcess(), (PVOID)injectAddress, screamerProxy, sizeof(screamerProxy), NULL);

    const ULONG_PTR injectAddress2 = 0x004C4E9E;
    BYTE screamerProxy2[] = {
        0x50,                                                   //push eax
        0x52,                                                   //push edx
        0xFF, 0x15, bytes[3], bytes[2], bytes[1], bytes[0]      //call DWRD PTR ds:${proxyAddress}
    };

    WriteProcessMemory(GetCurrentProcess(), (PVOID)injectAddress2, screamerProxy2, sizeof(screamerProxy2), NULL);
}