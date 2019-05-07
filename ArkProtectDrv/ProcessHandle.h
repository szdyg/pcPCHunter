#pragma once
#include <ntifs.h>
#include <ntstrsafe.h>
#include "Private.h"
#include "ProcessCore.h"
#include "ProcessThread.h"


typedef struct _PROCESS_HANDLE_ENTRY_INFORMATION
{
    HANDLE        Handle;
    PVOID        Object;
    UINT32        ReferenceCount;            // 引用计数
    WCHAR        wzHandleType[MAX_PATH];
    WCHAR        wzHandleName[MAX_PATH];
} PROCESS_HANDLE_ENTRY_INFORMATION, *PPROCESS_HANDLE_ENTRY_INFORMATION;

typedef struct _PROCESS_HANDLE_INFORMATION
{
    UINT32                                NumberOfHandles;
    PROCESS_HANDLE_ENTRY_INFORMATION    HandleEntry[1];
} PROCESS_HANDLE_INFORMATION, *PPROCESS_HANDLE_INFORMATION;



//************************************
// 函数名:   APGetHandleType
// 权限：    public 
// 返回值:   VOID
// 参数：    IN HANDLE Handle                     句柄
// 参数：    OUT PWCHAR wzHandleType              句柄类型
// 说明：    ZwQueryObject+ObjectTypeInformation查询句柄类型
//************************************
VOID APGetHandleType(IN HANDLE Handle, OUT PWCHAR wzHandleType);

//************************************
// 函数名:   APGetHandleName
// 权限：    public 
// 返回值:   VOID
// 参数：    IN HANDLE Handle                    句柄
// 参数：    OUT PWCHAR wzHandleName             句柄名称
// 说明：    ZwQueryObject+ObjectNameInformation查询句柄名称
//************************************
VOID APGetHandleName(IN HANDLE Handle, OUT PWCHAR wzHandleName);

//************************************
// 函数名:   APGetProcessHandleInfo
// 权限：    public 
// 返回值:   VOID
// 参数：    IN PEPROCESS EProcess                 进程结构体
// 参数：    IN HANDLE Handle                      进程句柄
// 参数：    IN PVOID Object                       进程对象
// 参数：    OUT PPROCESS_HANDLE_INFORMATION phi   返回信息
// 说明：    枚举目标进程的句柄信息，存入Ring3提供结构体
//************************************
VOID APGetProcessHandleInfo(IN PEPROCESS EProcess, IN HANDLE Handle, IN PVOID Object, OUT PPROCESS_HANDLE_INFORMATION phi);

//************************************
// 函数名:   APEnumProcessHandleByZwQuerySystemInformation
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN UINT32 ProcessId                     进程id
// 参数：    IN PEPROCESS EProcess                   进程结构体
// 参数：    OUT PPROCESS_HANDLE_INFORMATION phi
// 参数：    IN UINT32 HandleCount
// 说明：    
//************************************
NTSTATUS APEnumProcessHandleByZwQuerySystemInformation(IN UINT32 ProcessId, IN PEPROCESS EProcess, OUT PPROCESS_HANDLE_INFORMATION phi, IN UINT32 HandleCount);

//************************************
// 函数名:   APEnumProcessHandle
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN UINT32 ProcessId                进程Id
// 参数：    OUT PVOID OutputBuffer             R3内存
// 参数：    IN UINT32 OutputLength             R3内存大小
// 说明：    枚举进程模块
//************************************
NTSTATUS APEnumProcessHandle(IN UINT32 ProcessId, OUT PVOID OutputBuffer, IN UINT32 OutputLength);



