#pragma once
#include <ntifs.h>
#include "Private.h"
#include "ProcessCore.h"


typedef struct _PROCESS_MEMORY_ENTRY_INFORMATION
{
    UINT_PTR    BaseAddress;
    UINT_PTR    RegionSize;
    UINT32        Protect;
    UINT32        State;
    UINT32        Type;
} PROCESS_MEMORY_ENTRY_INFORMATION, *PPROCESS_MEMORY_ENTRY_INFORMATION;

typedef struct _PROCESS_MEMORY_INFORMATION
{
    UINT32                                NumberOfMemories;
    PROCESS_MEMORY_ENTRY_INFORMATION    MemoryEntry[1];
}PROCESS_MEMORY_INFORMATION, *PPROCESS_MEMORY_INFORMATION;


//************************************
// 函数名:   APEnumProcessMemoryByZwQueryVirtualMemory
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN PEPROCESS EProcess                  进程结构体
// 参数：    OUT PPROCESS_MEMORY_INFORMATION pmi    ring3内存
// 参数：    IN UINT32 MemoryCount                  个数
// 说明：    
//************************************
NTSTATUS APEnumProcessMemoryByZwQueryVirtualMemory(IN PEPROCESS EProcess, OUT PPROCESS_MEMORY_INFORMATION pmi, IN UINT32 MemoryCount);

//************************************
// 函数名:   APEnumProcessMemory
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN UINT32 ProcessId            进程Id
// 参数：    OUT PVOID OutputBuffer         R3内存
// 参数：    IN UINT32 OutputLength         内存长度
// 说明：    枚举进程模块
//************************************
NTSTATUS APEnumProcessMemory(IN UINT32 ProcessId, OUT PVOID OutputBuffer, IN UINT32 OutputLength);


