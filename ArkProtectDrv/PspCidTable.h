
#pragma once
#include <ntifs.h>
#include "Private.h"

//************************************
// 函数名:   APGetPspCidTableAddress
// 权限：    public 
// 返回值:   UINT_PTR
// 说明：    通过PsLookupProcessByProcessId的硬编码获得PspCidTable地址
//************************************
UINT_PTR APGetPspCidTableAddress();
