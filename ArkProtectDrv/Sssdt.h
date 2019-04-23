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
// ������:   APGetCurrentSssdtAddress
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ˵����    ��ȡSSSDT��ַ
//************************************
UINT_PTR APGetCurrentSssdtAddress();

UINT_PTR APGetCurrentWin32pServiceTable();

//************************************
// ������:   APFixWin32pServiceTable
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN PVOID ImageBase                  ��ģ����ػ���ַ ��PVOID��
// ������    IN PVOID OriginalBase               ԭģ����ػ���ַ ��PVOID��
// ˵����    ����Win32pServiceTable �Լ�base����ĺ���
//************************************
VOID APFixWin32pServiceTable(IN PVOID ImageBase, IN PVOID OriginalBase);

//************************************
// ������:   APReloadWin32k
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ˵����    �����ں˵�һģ��,win32k.sys
//************************************
NTSTATUS APReloadWin32k();

//************************************
// ������:   APEnumSssdtHookByReloadWin32k
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    OUT PSSSDT_HOOK_INFORMATION shi
// ������    IN UINT32 SssdtFunctionCount
// ˵����    ����Win32k ���Sssdt Hook
//************************************
NTSTATUS APEnumSssdtHookByReloadWin32k(OUT PSSSDT_HOOK_INFORMATION shi, IN UINT32 SssdtFunctionCount);

//************************************
// ������:   APEnumSssdtHook
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    OUT PVOID OutputBuffer
// ������    IN UINT32 OutputLength
// ˵����    ö�ٽ���ģ��
//************************************
NTSTATUS APEnumSssdtHook(OUT PVOID OutputBuffer, IN UINT32 OutputLength);

//************************************
// ������:   APResumeSssdtHook
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN UINT32 Ordinal           �������
// ˵����    �ָ�ָ����SsdtHook����ģ��
//************************************
NTSTATUS APResumeSssdtHook(IN UINT32 Ordinal);

//************************************
// ������:   APGetSssdtFunctionAddress
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ������    IN PCWCHAR wzFunctionName
// ˵����    ��ȡSSSDT������ַ
//************************************
UINT_PTR APGetSssdtFunctionAddress(IN PCWCHAR wzFunctionName);

#endif // !CXX_Sssdt_H


