#pragma once
#include <ntifs.h>
#include "Private.h"
#include "Imports.h"
#include "NtStructs.h"
#include "PspCidTable.h"
#include "ProcessThread.h"

#define MAX_PROCESS_COUNT  200000

typedef struct _PROCESS_ENTRY_INFORMATION
{
    WCHAR     wzImageName[100];
    UINT32      ProcessId;
    UINT32      ParentProcessId;
    WCHAR     wzFilePath[MAX_PATH];
    UINT_PTR  EProcess;
    BOOL      bUserAccess;
    WCHAR     wzCompanyName[MAX_PATH];
} PROCESS_ENTRY_INFORMATION, *PPROCESS_ENTRY_INFORMATION;

typedef struct _PROCESS_INFORMATION
{
    UINT32                    NumberOfProcesses;
    PROCESS_ENTRY_INFORMATION ProcessEntry[1];
} PROCESS_INFORMATION, *PPROCESS_INFORMATION;

//************************************
// 函数名:   APGetPsIdleProcess
// 权限：    public 
// 返回值:   UINT_PTR
// 说明：    获得System Idle Process 的EProcess地址
//************************************
UINT_PTR APGetPsIdleProcess();

//************************************
// 函数名:   APGetObjectType
// 权限：    public 
// 返回值:   UINT_PTR                  对象类型
// 参数：    IN PVOID Object           对象体首地址
// 说明：    通过ObGetObjectType获得对象类型
//************************************
UINT_PTR APGetObjectType(IN PVOID Object);

//************************************
// 函数名:   APIsActiveProcess
// 权限：    public 
// 返回值:   BOOLEAN                   TRUE存活/ FALSE死进程
// 参数：    IN PEPROCESS EProcess
// 说明：    通过是否存在句柄表判断进程是否存活
//************************************
BOOLEAN APIsActiveProcess(IN PEPROCESS EProcess);

//************************************
// 函数名:   APIsValidProcess
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    IN PEPROCESS EProcess
// 说明：    判断是否为合法进程         TRUE合法/ FALSE非法
//************************************
BOOLEAN APIsValidProcess(IN PEPROCESS EProcess);

//************************************
// 函数名:   APGetProcessNum
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    OUT PVOID OutputBuffer      地址指针
// 说明：    获得当前进程个数
//************************************
NTSTATUS APGetProcessNum(OUT PULONG OutputBuffer);

//************************************
// 函数名:   APGetParentProcessId
// 权限：    public 
// 返回值:   UINT_PTR
// 参数：    IN PEPROCESS EProcess
// 说明：    获得父进程Id
//************************************
UINT_PTR APGetParentProcessId(IN PEPROCESS EProcess);

//************************************
// 函数名:   APGetProcessFullPath
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN PEPROCESS EProcess
// 参数：    OUT PWCHAR ProcessFullPath
// 说明：    获得进程完整路径
//************************************
NTSTATUS APGetProcessFullPath(IN PEPROCESS EProcess, OUT PWCHAR ProcessFullPath);

//************************************
// 函数名:   APGetProcessInfo
// 权限：    public 
// 返回值:   VOID
// 参数：    IN PEPROCESS EProcess
// 参数：    OUT PPROCESS_INFORMATION pi
// 参数：    IN UINT32 ProcessCount
// 说明：    获得进程信息
//************************************
VOID APGetProcessInfo(IN PEPROCESS EProcess, OUT PPROCESS_INFORMATION pi, IN UINT32 ProcessCount);

//************************************
// 函数名:   APEnumProcessInfoByIterateFirstLevelHandleTable
// 权限：    public 
// 返回值:   VOID
// 参数：    IN UINT_PTR TableCode
// 参数：    OUT PPROCESS_INFORMATION pi
// 参数：    IN UINT32 ProcessCount
// 说明：    遍历一级表
//************************************
VOID APEnumProcessInfoByIterateFirstLevelHandleTable(IN UINT_PTR TableCode, OUT PPROCESS_INFORMATION pi, IN UINT32 ProcessCount);

//************************************
// 函数名:   APEnumProcessInfoByIterateSecondLevelHandleTable
// 权限：    public 
// 返回值:   VOID
// 参数：    IN UINT_PTR TableCode
// 参数：    OUT PPROCESS_INFORMATION pi
// 参数：    IN UINT32 ProcessCount
// 说明：    遍历二级表
//************************************
VOID APEnumProcessInfoByIterateSecondLevelHandleTable(IN UINT_PTR TableCode, OUT PPROCESS_INFORMATION pi, IN UINT32 ProcessCount);

//************************************
// 函数名:   APEnumProcessInfoByIterateThirdLevelHandleTable
// 权限：    public 
// 返回值:   VOID
// 参数：    IN UINT_PTR TableCode
// 参数：    OUT PPROCESS_INFORMATION pi
// 参数：    IN UINT32 ProcessCount
// 说明：    遍历三级表
//************************************
VOID APEnumProcessInfoByIterateThirdLevelHandleTable(IN UINT_PTR TableCode, OUT PPROCESS_INFORMATION pi, IN UINT32 ProcessCount);

//************************************
// 函数名:   APEnumProcessInfoByIteratePspCidTable
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    OUT PPROCESS_INFORMATION pi
// 参数：    IN UINT32 ProcessCount
// 说明：    
//************************************
NTSTATUS  APEnumProcessInfoByIteratePspCidTable(OUT PPROCESS_INFORMATION pi, IN UINT32 ProcessCount);

//************************************
// 函数名:   APEnumProcessInfo
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    OUT PVOID OutputBuffer
// 参数：    IN UINT32 OutputLength
// 说明：    枚举进程信息
//************************************
NTSTATUS APEnumProcessInfo(OUT PVOID OutputBuffer, IN UINT32 OutputLength);

//************************************
// 函数名:   APTerminateProcess
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN UINT32 ProcessId
// 说明：    结束进程
//************************************
NTSTATUS APTerminateProcess(IN UINT32 ProcessId);

//************************************
// 函数名:   APGetGuiProcess
// 权限：    public 
// 返回值:   PEPROCESS
// 说明：    获得csrss Eprocess
//************************************
PEPROCESS APGetCsrssProcess();




