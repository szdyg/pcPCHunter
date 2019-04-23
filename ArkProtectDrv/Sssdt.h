#ifndef CXX_Sssdt_H
#define CXX_Sssdt_H

#include <ntifs.h>
#include "Private.h"
#include "NtStructs.h"
#include "PeLoader.h"
#include "DriverCore.h"
#include "ProcessCore.h"

typedef struct _SSSDT_HOOK_ENTRY_INFORMATION
{
    UINT32      Ordinal;
    BOOL        bHooked;
    UINT_PTR    CurrentAddress;
    UINT_PTR    OriginalAddress;
    WCHAR       wzFunctionName[100];
} SSSDT_HOOK_ENTRY_INFORMATION, *PSSSDT_HOOK_ENTRY_INFORMATION;

typedef struct _SSSDT_HOOK_INFORMATION
{
    UINT32                         NumberOfSssdtFunctions;
    SSSDT_HOOK_ENTRY_INFORMATION   SssdtHookEntry[1];
} SSSDT_HOOK_INFORMATION, *PSSSDT_HOOK_INFORMATION;



//************************************
// 函数名:   APGetCurrentSssdtAddress
// 权限：    public 
// 返回值:   UINT_PTR
// 说明：    获取SSSDT地址
//************************************
UINT_PTR APGetCurrentSssdtAddress();

UINT_PTR APGetCurrentWin32pServiceTable();

//************************************
// 函数名:   APFixWin32pServiceTable
// 权限：    public 
// 返回值:   VOID
// 参数：    IN PVOID ImageBase                  新模块加载基地址 （PVOID）
// 参数：    IN PVOID OriginalBase               原模块加载基地址 （PVOID）
// 说明：    修正Win32pServiceTable 以及base里面的函数
//************************************
VOID APFixWin32pServiceTable(IN PVOID ImageBase, IN PVOID OriginalBase);

//************************************
// 函数名:   APReloadWin32k
// 权限：    public 
// 返回值:   NTSTATUS
// 说明：    重载内核第一模块,win32k.sys
//************************************
NTSTATUS APReloadWin32k();

//************************************
// 函数名:   APEnumSssdtHookByReloadWin32k
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    OUT PSSSDT_HOOK_INFORMATION shi
// 参数：    IN UINT32 SssdtFunctionCount
// 说明：    重载Win32k 检查Sssdt Hook
//************************************
NTSTATUS APEnumSssdtHookByReloadWin32k(OUT PSSSDT_HOOK_INFORMATION shi, IN UINT32 SssdtFunctionCount);

//************************************
// 函数名:   APEnumSssdtHook
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    OUT PVOID OutputBuffer
// 参数：    IN UINT32 OutputLength
// 说明：    枚举进程模块
//************************************
NTSTATUS APEnumSssdtHook(OUT PVOID OutputBuffer, IN UINT32 OutputLength);

//************************************
// 函数名:   APResumeSssdtHook
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN UINT32 Ordinal           函数序号
// 说明：    恢复指定的SsdtHook进程模块
//************************************
NTSTATUS APResumeSssdtHook(IN UINT32 Ordinal);

//************************************
// 函数名:   APGetSssdtFunctionAddress
// 权限：    public 
// 返回值:   UINT_PTR
// 参数：    IN PCWCHAR wzFunctionName
// 说明：    获取SSSDT函数地址
//************************************
UINT_PTR APGetSssdtFunctionAddress(IN PCWCHAR wzFunctionName);

#endif // !CXX_Sssdt_H


