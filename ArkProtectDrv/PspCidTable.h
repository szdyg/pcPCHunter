
#pragma once
#include <ntifs.h>
#include "Private.h"

//************************************
// ������:   APGetPspCidTableAddress
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ˵����    ͨ��PsLookupProcessByProcessId��Ӳ������PspCidTable��ַ
//************************************
UINT_PTR APGetPspCidTableAddress();
