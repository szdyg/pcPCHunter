#pragma once
#include <ntifs.h>
#include <ntstrsafe.h>
#include "Private.h"
#include "ProcessCore.h"

typedef enum _eLdrType
{
    lt_InLoadOrderModuleList = 0,
    lt_InMemoryOrderModuleList,
    lt_InInitializationOrderModuleList
} eLdrType;

typedef struct _PROCESS_MODULE_ENTRY_INFORMATION
{
    UINT_PTR    BaseAddress;
    UINT_PTR    SizeOfImage;
    WCHAR        wzFilePath[MAX_PATH];
    WCHAR       wzCompanyName[MAX_PATH];
} PROCESS_MODULE_ENTRY_INFORMATION, *PPROCESS_MODULE_ENTRY_INFORMATION;

typedef struct _PROCESS_MODULE_INFORMATION
{
    UINT32                           NumberOfModules;
    PROCESS_MODULE_ENTRY_INFORMATION ModuleEntry[1];
} PROCESS_MODULE_INFORMATION, *PPROCESS_MODULE_INFORMATION;



//************************************
// 函数名:   APIsProcessModuleInList
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    IN UINT_PTR BaseAddress                  模块基地址
// 参数：    IN UINT32 ModuleSize                     模块大小
// 参数：    IN PPROCESS_MODULE_INFORMATION pmi
// 参数：    IN UINT32 ModuleCount
// 说明：    查看进程模块是否在列表中
//************************************
BOOLEAN APIsProcessModuleInList(IN UINT_PTR BaseAddress, IN UINT32 ModuleSize, IN PPROCESS_MODULE_INFORMATION pmi, IN UINT32 ModuleCount);

//************************************
// 函数名:   APEnumProcessModuleByZwQueryVirtualMemory
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN PEPROCESS EProcess                           进程结构体
// 参数：    OUT PPROCESS_MODULE_INFORMATION pmi             r3内存
// 参数：    IN UINT32 ModuleCount
// 说明：    通过ZwQueryVirtualMemory便利进程模块（处理Wow64）
//************************************
NTSTATUS APEnumProcessModuleByZwQueryVirtualMemory(IN PEPROCESS EProcess, OUT PPROCESS_MODULE_INFORMATION pmi, IN UINT32 ModuleCount);

//************************************
// 函数名:   APEnumProcessModuleByPeb
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN PEPROCESS EProcess                         进程结构体
// 参数：    OUT PPROCESS_MODULE_INFORMATION pmi           r3内存
// 参数：    IN UINT32 ModuleCount
// 说明：    通过遍历peb的Ldr三根链表中的一个表（处理Wow64）
//************************************
NTSTATUS APEnumProcessModuleByPeb(IN PEPROCESS EProcess, OUT PPROCESS_MODULE_INFORMATION pmi, IN UINT32 ModuleCount);

//************************************
// 函数名:   APEnumProcessModule
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN UINT32 ProcessId                   进程Id
// 参数：    OUT PVOID OutputBuffer                r3内存
// 参数：    IN UINT32 OutputLength                内存长度
// 说明：    枚举进程模块
//************************************
NTSTATUS APEnumProcessModule(IN UINT32 ProcessId, OUT PVOID OutputBuffer, IN UINT32 OutputLength);

