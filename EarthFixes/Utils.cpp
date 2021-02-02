#include "Utils.h"
#include "pch.h"

void ToByteArray(DWORD ulong, BYTE result[])
{
	result[0] = (BYTE)((ulong >> 24) & 0xFF);
	result[1] = (BYTE)((ulong >> 16) & 0xFF);
	result[2] = (BYTE)((ulong >> 8) & 0XFF);
	result[3] = (BYTE)(ulong & 0XFF);
}