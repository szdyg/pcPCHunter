#pragma once
#include <ntifs.h>
#include <ntstrsafe.h>
#include "Private.h"
#include "ProcessCore.h"
#include "PspCidTable.h"


typedef struct _PROCESS_THREAD_ENTRY_INFORMATION
{
    UINT_PTR EThread;
    UINT32   ThreadId;
    UINT_PTR Teb;
    UINT8    Priority;
    UINT_PTR Win32StartAddress;
    UINT32   ContextSwitches;
    UINT8    State;
} PROCESS_THREAD_ENTRY_INFORMATION, *PPROCESS_THREAD_ENTRY_INFORMATION;


typedef struct _PROCESS_THREAD_INFORMATION
{
    UINT32                           NumberOfThreads;
    PROCESS_THREAD_ENTRY_INFORMATION ThreadEntry[1];
} PROCESS_THREAD_INFORMATION, *PPROCESS_THREAD_INFORMATION;


//************************************
// 函数名:   APChangeThreadMode
// 权限：    public 
// 返回值:   UINT8                         先前模式
// 参数：    IN PETHREAD EThread           线程结构体
// 参数：    IN UINT8 WantedMode           想要的模式
// 说明：    修改线程模式为目标模式
//************************************
UINT8 APChangeThreadMode(IN PETHREAD EThread, IN UINT8 WantedMode);

//************************************
// 函数名:   APIsThreadInList
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    IN PETHREAD EThread
// 参数：    IN PPROCESS_THREAD_INFORMATION pti
// 参数：    IN UINT32 ThreadCount
// 说明：    检查线程是否在链表中
//************************************
BOOLEAN APIsThreadInList(IN PETHREAD EThread, IN PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// 函数名:   APGetThreadStartAddress
// 权限：    public 
// 返回值:   UINT_PTR
// 参数：    IN PETHREAD EThread         线程结构体
// 说明：    获取线程函数地址
//************************************
UINT_PTR APGetThreadStartAddress(IN PETHREAD EThread);

//************************************
// 函数名:   APGetProcessThreadInfo
// 权限：    public 
// 返回值:   VOID
// 参数：    IN PETHREAD EThread
// 参数：    IN PEPROCESS EProcess
// 参数：    OUT PPROCESS_THREAD_INFORMATION pti
// 参数：    IN UINT32 ThreadCount
// 说明：    获取进程线程信息
//************************************
VOID APGetProcessThreadInfo(IN PETHREAD EThread, IN PEPROCESS EProcess, OUT PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// 函数名:   APEnumProcessThreadByIterateFirstLevelHandleTable
// 权限：    public 
// 返回值:   VOID
// 参数：    IN UINT_PTR TableCode
// 参数：    IN PEPROCESS EProcess
// 参数：    OUT PPROCESS_THREAD_INFORMATION pti
// 参数：    IN UINT32 ThreadCount
// 说明：    遍历一级表
//************************************
VOID APEnumProcessThreadByIterateFirstLevelHandleTable(IN UINT_PTR TableCode, IN PEPROCESS EProcess, OUT PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// 函数名:   APEnumProcessThreadByIterateSecondLevelHandleTable
// 权限：    public 
// 返回值:   VOID
// 参数：    IN UINT_PTR TableCode
// 参数：    IN PEPROCESS EProcess
// 参数：    OUT PPROCESS_THREAD_INFORMATION pti
// 参数：    IN UINT32 ThreadCount
// 说明：    遍历二级表
//************************************
VOID APEnumProcessThreadByIterateSecondLevelHandleTable(IN UINT_PTR TableCode, IN PEPROCESS EProcess, OUT PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// 函数名:   APEnumProcessThreadByIterateThirdLevelHandleTable
// 权限：    public 
// 返回值:   VOID
// 参数：    IN UINT_PTR TableCode
// 参数：    IN PEPROCESS EProcess
// 参数：    OUT PPROCESS_THREAD_INFORMATION pti
// 参数：    IN UINT32 ThreadCount
// 说明：    遍历三级表
//************************************
VOID APEnumProcessThreadByIterateThirdLevelHandleTable(IN UINT_PTR TableCode, IN PEPROCESS EProcess, OUT PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// 函数名:   APEnumProcessThreadByIteratePspCidTable
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN PEPROCESS EProcess
// 参数：    OUT PPROCESS_THREAD_INFORMATION pti
// 参数：    IN UINT32 ThreadCount
// 说明：    通过遍历EProcess和KProcess的ThreadListHead链表
//************************************
NTSTATUS APEnumProcessThreadByIteratePspCidTable(IN PEPROCESS EProcess, OUT PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// 函数名:   APEnumProcessThreadByIterateThreadListHead
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN PEPROCESS EProcess
// 参数：    OUT PPROCESS_THREAD_INFORMATION pti
// 参数：    IN UINT32 ThreadCount
// 说明：    通过遍历EProcess和KProcess的ThreadListHead链表
//************************************
NTSTATUS APEnumProcessThreadByIterateThreadListHead(IN PEPROCESS EProcess, OUT PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// 函数名:   APEnumProcessThread
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN UINT32 ProcessId                 PID
// 参数：    OUT PVOID OutputBuffer
// 参数：    IN UINT32 OutputLength
// 说明：    遍历进程的线程
//************************************
NTSTATUS APEnumProcessThread(IN UINT32 ProcessId, OUT PVOID OutputBuffer, IN UINT32 OutputLength);

//************************************
// 函数名:   APGetPspTerminateThreadByPointerAddress
// 权限：    public 
// 返回值:   UINT_PTR
// 说明：    通过PsTerminateSystemThread获得PspTerminateThreadByPointer函数地址
//************************************
UINT_PTR APGetPspTerminateThreadByPointerAddress();

//************************************
// 函数名:   APTerminateProcessByIterateThreadListHead
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN PEPROCESS EProcess
// 说明：    遍历进程体的所有线程，通过PspTerminateThreadByPointer结束线程，从而结束进程
//************************************
NTSTATUS APTerminateProcessByIterateThreadListHead(IN PEPROCESS EProcess);




