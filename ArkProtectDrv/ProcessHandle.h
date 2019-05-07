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
    UINT32        ReferenceCount;            // ���ü���
    WCHAR        wzHandleType[MAX_PATH];
    WCHAR        wzHandleName[MAX_PATH];
} PROCESS_HANDLE_ENTRY_INFORMATION, *PPROCESS_HANDLE_ENTRY_INFORMATION;

typedef struct _PROCESS_HANDLE_INFORMATION
{
    UINT32                                NumberOfHandles;
    PROCESS_HANDLE_ENTRY_INFORMATION    HandleEntry[1];
} PROCESS_HANDLE_INFORMATION, *PPROCESS_HANDLE_INFORMATION;



//************************************
// ������:   APGetHandleType
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN HANDLE Handle                     ���
// ������    OUT PWCHAR wzHandleType              �������
// ˵����    ZwQueryObject+ObjectTypeInformation��ѯ�������
//************************************
VOID APGetHandleType(IN HANDLE Handle, OUT PWCHAR wzHandleType);

//************************************
// ������:   APGetHandleName
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN HANDLE Handle                    ���
// ������    OUT PWCHAR wzHandleName             �������
// ˵����    ZwQueryObject+ObjectNameInformation��ѯ�������
//************************************
VOID APGetHandleName(IN HANDLE Handle, OUT PWCHAR wzHandleName);

//************************************
// ������:   APGetProcessHandleInfo
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN PEPROCESS EProcess                 ���̽ṹ��
// ������    IN HANDLE Handle                      ���̾��
// ������    IN PVOID Object                       ���̶���
// ������    OUT PPROCESS_HANDLE_INFORMATION phi   ������Ϣ
// ˵����    ö��Ŀ����̵ľ����Ϣ������Ring3�ṩ�ṹ��
//************************************
VOID APGetProcessHandleInfo(IN PEPROCESS EProcess, IN HANDLE Handle, IN PVOID Object, OUT PPROCESS_HANDLE_INFORMATION phi);

//************************************
// ������:   APEnumProcessHandleByZwQuerySystemInformation
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN UINT32 ProcessId                     ����id
// ������    IN PEPROCESS EProcess                   ���̽ṹ��
// ������    OUT PPROCESS_HANDLE_INFORMATION phi
// ������    IN UINT32 HandleCount
// ˵����    
//************************************
NTSTATUS APEnumProcessHandleByZwQuerySystemInformation(IN UINT32 ProcessId, IN PEPROCESS EProcess, OUT PPROCESS_HANDLE_INFORMATION phi, IN UINT32 HandleCount);

//************************************
// ������:   APEnumProcessHandle
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN UINT32 ProcessId                ����Id
// ������    OUT PVOID OutputBuffer             R3�ڴ�
// ������    IN UINT32 OutputLength             R3�ڴ��С
// ˵����    ö�ٽ���ģ��
//************************************
NTSTATUS APEnumProcessHandle(IN UINT32 ProcessId, OUT PVOID OutputBuffer, IN UINT32 OutputLength);



