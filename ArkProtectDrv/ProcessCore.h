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
// ������:   APGetPsIdleProcess
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ˵����    ���System Idle Process ��EProcess��ַ
//************************************
UINT_PTR APGetPsIdleProcess();

//************************************
// ������:   APGetObjectType
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR                  ��������
// ������    IN PVOID Object           �������׵�ַ
// ˵����    ͨ��ObGetObjectType��ö�������
//************************************
UINT_PTR APGetObjectType(IN PVOID Object);

//************************************
// ������:   APIsActiveProcess
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN                   TRUE���/ FALSE������
// ������    IN PEPROCESS EProcess
// ˵����    ͨ���Ƿ���ھ�����жϽ����Ƿ���
//************************************
BOOLEAN APIsActiveProcess(IN PEPROCESS EProcess);

//************************************
// ������:   APIsValidProcess
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    IN PEPROCESS EProcess
// ˵����    �ж��Ƿ�Ϊ�Ϸ�����         TRUE�Ϸ�/ FALSE�Ƿ�
//************************************
BOOLEAN APIsValidProcess(IN PEPROCESS EProcess);

//************************************
// ������:   APGetProcessNum
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    OUT PVOID OutputBuffer      ��ַָ��
// ˵����    ��õ�ǰ���̸���
//************************************
NTSTATUS APGetProcessNum(OUT PULONG OutputBuffer);

//************************************
// ������:   APGetParentProcessId
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ������    IN PEPROCESS EProcess
// ˵����    ��ø�����Id
//************************************
UINT_PTR APGetParentProcessId(IN PEPROCESS EProcess);

//************************************
// ������:   APGetProcessFullPath
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN PEPROCESS EProcess
// ������    OUT PWCHAR ProcessFullPath
// ˵����    ��ý�������·��
//************************************
NTSTATUS APGetProcessFullPath(IN PEPROCESS EProcess, OUT PWCHAR ProcessFullPath);

//************************************
// ������:   APGetProcessInfo
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN PEPROCESS EProcess
// ������    OUT PPROCESS_INFORMATION pi
// ������    IN UINT32 ProcessCount
// ˵����    ��ý�����Ϣ
//************************************
VOID APGetProcessInfo(IN PEPROCESS EProcess, OUT PPROCESS_INFORMATION pi, IN UINT32 ProcessCount);

//************************************
// ������:   APEnumProcessInfoByIterateFirstLevelHandleTable
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN UINT_PTR TableCode
// ������    OUT PPROCESS_INFORMATION pi
// ������    IN UINT32 ProcessCount
// ˵����    ����һ����
//************************************
VOID APEnumProcessInfoByIterateFirstLevelHandleTable(IN UINT_PTR TableCode, OUT PPROCESS_INFORMATION pi, IN UINT32 ProcessCount);

//************************************
// ������:   APEnumProcessInfoByIterateSecondLevelHandleTable
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN UINT_PTR TableCode
// ������    OUT PPROCESS_INFORMATION pi
// ������    IN UINT32 ProcessCount
// ˵����    ����������
//************************************
VOID APEnumProcessInfoByIterateSecondLevelHandleTable(IN UINT_PTR TableCode, OUT PPROCESS_INFORMATION pi, IN UINT32 ProcessCount);

//************************************
// ������:   APEnumProcessInfoByIterateThirdLevelHandleTable
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN UINT_PTR TableCode
// ������    OUT PPROCESS_INFORMATION pi
// ������    IN UINT32 ProcessCount
// ˵����    ����������
//************************************
VOID APEnumProcessInfoByIterateThirdLevelHandleTable(IN UINT_PTR TableCode, OUT PPROCESS_INFORMATION pi, IN UINT32 ProcessCount);

//************************************
// ������:   APEnumProcessInfoByIteratePspCidTable
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    OUT PPROCESS_INFORMATION pi
// ������    IN UINT32 ProcessCount
// ˵����    
//************************************
NTSTATUS  APEnumProcessInfoByIteratePspCidTable(OUT PPROCESS_INFORMATION pi, IN UINT32 ProcessCount);

//************************************
// ������:   APEnumProcessInfo
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    OUT PVOID OutputBuffer
// ������    IN UINT32 OutputLength
// ˵����    ö�ٽ�����Ϣ
//************************************
NTSTATUS APEnumProcessInfo(OUT PVOID OutputBuffer, IN UINT32 OutputLength);

//************************************
// ������:   APTerminateProcess
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN UINT32 ProcessId
// ˵����    ��������
//************************************
NTSTATUS APTerminateProcess(IN UINT32 ProcessId);

//************************************
// ������:   APGetGuiProcess
// Ȩ�ޣ�    public 
// ����ֵ:   PEPROCESS
// ˵����    ���csrss Eprocess
//************************************
PEPROCESS APGetCsrssProcess();




