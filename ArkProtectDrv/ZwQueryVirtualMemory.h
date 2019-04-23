#pragma once
#include <ntifs.h>
#include <devioctl.h>

typedef unsigned long DWORD;




// typedef struct _MEMORY_BASIC_INFORMATION
// {
//     PVOID       BaseAddress;           //查询内存块所占的第一个页面基地址
//     PVOID       AllocationBase;        //内存块所占的第一块区域基地址，小于等于BaseAddress，
//     DWORD       AllocationProtect;     //区域被初次保留时赋予的保护属性
//     ULONG_PTR   RegionSize;            //从BaseAddress开始，具有相同属性的页面的大小，
//     DWORD       State;                 //页面的状态，有三种可能值MEM_COMMIT、MEM_FREE和MEM_RESERVE
//     DWORD       Protect;               //页面的属性，其可能的取值与AllocationProtect相同
//     DWORD       Type;                  //该内存块的类型，有三种可能值：MEM_IMAGE、MEM_MAPPED和MEM_PRIVATE
// } MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

typedef NTSTATUS(*pfnNtQueryVirtualMemory)(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    MEMORY_INFORMATION_CLASS MemoryInformationClass,
    PVOID MemoryInformation,
    ULONG_PTR MemoryInformationLength,
    PSIZE_T ReturnLength);

NTSTATUS EnumMoudleByNtQueryVirtualMemory(ULONG ProcessId);


