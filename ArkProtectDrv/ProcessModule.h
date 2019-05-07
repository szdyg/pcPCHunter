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
// ������:   APIsProcessModuleInList
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    IN UINT_PTR BaseAddress                  ģ�����ַ
// ������    IN UINT32 ModuleSize                     ģ���С
// ������    IN PPROCESS_MODULE_INFORMATION pmi
// ������    IN UINT32 ModuleCount
// ˵����    �鿴����ģ���Ƿ����б���
//************************************
BOOLEAN APIsProcessModuleInList(IN UINT_PTR BaseAddress, IN UINT32 ModuleSize, IN PPROCESS_MODULE_INFORMATION pmi, IN UINT32 ModuleCount);

//************************************
// ������:   APEnumProcessModuleByZwQueryVirtualMemory
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN PEPROCESS EProcess                           ���̽ṹ��
// ������    OUT PPROCESS_MODULE_INFORMATION pmi             r3�ڴ�
// ������    IN UINT32 ModuleCount
// ˵����    ͨ��ZwQueryVirtualMemory��������ģ�飨����Wow64��
//************************************
NTSTATUS APEnumProcessModuleByZwQueryVirtualMemory(IN PEPROCESS EProcess, OUT PPROCESS_MODULE_INFORMATION pmi, IN UINT32 ModuleCount);

//************************************
// ������:   APEnumProcessModuleByPeb
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN PEPROCESS EProcess                         ���̽ṹ��
// ������    OUT PPROCESS_MODULE_INFORMATION pmi           r3�ڴ�
// ������    IN UINT32 ModuleCount
// ˵����    ͨ������peb��Ldr���������е�һ��������Wow64��
//************************************
NTSTATUS APEnumProcessModuleByPeb(IN PEPROCESS EProcess, OUT PPROCESS_MODULE_INFORMATION pmi, IN UINT32 ModuleCount);

//************************************
// ������:   APEnumProcessModule
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN UINT32 ProcessId                   ����Id
// ������    OUT PVOID OutputBuffer                r3�ڴ�
// ������    IN UINT32 OutputLength                �ڴ泤��
// ˵����    ö�ٽ���ģ��
//************************************
NTSTATUS APEnumProcessModule(IN UINT32 ProcessId, OUT PVOID OutputBuffer, IN UINT32 OutputLength);

