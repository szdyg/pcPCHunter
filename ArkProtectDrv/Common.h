#pragma once
#include "ZwQueryVirtualMemory.h"
#include <ntddk.h>

typedef enum WIN_VERSION 
{
    WINDOWS_UNKNOW = 0,
    WINDOWS_XP,
    WINDOWS_7_7600,
    WINDOWS_7_7601,
    WINDOWS_8_9200,
    WINDOWS_8_9600,
    WINDOWS_10_10240,
    WINDOWS_10_10586,
    WINDOWS_10_14393,
    WINDOWS_10_15063,
    WINDOWS_10_16299,
    WINDOWS_10_17134,
} WIN_VERSION;

typedef struct _COMMON_INFO
{
    ULONG_PTR   ObjectTableOffsetOf_EPROCESS;
    ULONG_PTR   PreviousModeOffsetOf_KTHREAD;
    ULONG_PTR   IndexOffsetOfFunction;
    ULONG_PTR   SSDTDescriptor;
    ULONG_PTR   HighUserAddress;
    WIN_VERSION WinVersion;
    ULONG_PTR   LdrInPebOffset;
    ULONG_PTR   ModListInLdrOffset;
    ULONG_PTR   ObjectHeaderSize;
    ULONG_PTR   ObjectTypeOffsetOf_Object_Header;
}COMMON_INFO,*PCOMMON_INFO;

typedef NTSTATUS (*pfnRtlGetVersion)(OUT PRTL_OSVERSIONINFOW lpVersionInformation);
typedef ULONG_PTR (*pfnObGetObjectType)(PVOID pObject);

//************************************
// 函数名:   GetWindowsVersion
// 权限：    public 
// 返回值:   WIN_VERSION
// 说明：    获取Windows版本
//************************************
WIN_VERSION GetWindowsVersion();

//************************************
// 函数名:   GetFunctionAddressByName
// 权限：    public 
// 返回值:   PVOID
// 参数：    WCHAR * wzFunction
// 说明：    获取导出函数地址
//************************************
PVOID GetFunctionAddressByName(WCHAR *wzFunction);

//************************************
// 函数名:   KeGetObjectType
// 权限：    public 
// 返回值:   ULONG_PTR
// 参数：    PVOID Object
// 说明：    获取ObGetObjectType函数指针
//************************************
ULONG_PTR KeGetObjectType(PVOID Object);

//************************************
// 函数名:   IsProcessDie
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    PEPROCESS EProcess
// 说明：    判断进程是否死亡
//************************************
BOOLEAN IsProcessDie(PEPROCESS EProcess);

BOOLEAN IsRealProcess(PEPROCESS EProcess);


CHAR ChangePreMode(PETHREAD EThread);
VOID RecoverPreMode(PETHREAD EThread, CHAR PreMode);
//************************************
// 函数名:   InitGlobalVariable
// 权限：    public 
// 返回值:   VOID
// 说明：    初始化一些偏移
//************************************
VOID InitGlobalVariable();
//************************************
// 函数名:   NtPathToDosPathW
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    WCHAR * wzFullNtPath
// 参数：    WCHAR * wzFullDosPath
// 说明：    路径转化
//************************************
BOOLEAN NtPathToDosPathW(PWCHAR wzFullNtPath,PWCHAR wzFullDosPath);
ULONG NtQueryDosDevice(PWCHAR wzDosDevice, PWCHAR wzNtDevice, ULONG ucchMax);


