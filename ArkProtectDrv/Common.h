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
// ������:   GetWindowsVersion
// Ȩ�ޣ�    public 
// ����ֵ:   WIN_VERSION
// ˵����    ��ȡWindows�汾
//************************************
WIN_VERSION GetWindowsVersion();

//************************************
// ������:   GetFunctionAddressByName
// Ȩ�ޣ�    public 
// ����ֵ:   PVOID
// ������    WCHAR * wzFunction
// ˵����    ��ȡ����������ַ
//************************************
PVOID GetFunctionAddressByName(WCHAR *wzFunction);

//************************************
// ������:   KeGetObjectType
// Ȩ�ޣ�    public 
// ����ֵ:   ULONG_PTR
// ������    PVOID Object
// ˵����    ��ȡObGetObjectType����ָ��
//************************************
ULONG_PTR KeGetObjectType(PVOID Object);

//************************************
// ������:   IsProcessDie
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    PEPROCESS EProcess
// ˵����    �жϽ����Ƿ�����
//************************************
BOOLEAN IsProcessDie(PEPROCESS EProcess);

BOOLEAN IsRealProcess(PEPROCESS EProcess);


CHAR ChangePreMode(PETHREAD EThread);
VOID RecoverPreMode(PETHREAD EThread, CHAR PreMode);
//************************************
// ������:   InitGlobalVariable
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ˵����    ��ʼ��һЩƫ��
//************************************
VOID InitGlobalVariable();
//************************************
// ������:   NtPathToDosPathW
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    WCHAR * wzFullNtPath
// ������    WCHAR * wzFullDosPath
// ˵����    ·��ת��
//************************************
BOOLEAN NtPathToDosPathW(PWCHAR wzFullNtPath,PWCHAR wzFullDosPath);
ULONG NtQueryDosDevice(PWCHAR wzDosDevice, PWCHAR wzNtDevice, ULONG ucchMax);


