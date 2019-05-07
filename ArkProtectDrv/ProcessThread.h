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
// ������:   APChangeThreadMode
// Ȩ�ޣ�    public 
// ����ֵ:   UINT8                         ��ǰģʽ
// ������    IN PETHREAD EThread           �߳̽ṹ��
// ������    IN UINT8 WantedMode           ��Ҫ��ģʽ
// ˵����    �޸��߳�ģʽΪĿ��ģʽ
//************************************
UINT8 APChangeThreadMode(IN PETHREAD EThread, IN UINT8 WantedMode);

//************************************
// ������:   APIsThreadInList
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    IN PETHREAD EThread
// ������    IN PPROCESS_THREAD_INFORMATION pti
// ������    IN UINT32 ThreadCount
// ˵����    ����߳��Ƿ���������
//************************************
BOOLEAN APIsThreadInList(IN PETHREAD EThread, IN PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// ������:   APGetThreadStartAddress
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ������    IN PETHREAD EThread         �߳̽ṹ��
// ˵����    ��ȡ�̺߳�����ַ
//************************************
UINT_PTR APGetThreadStartAddress(IN PETHREAD EThread);

//************************************
// ������:   APGetProcessThreadInfo
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN PETHREAD EThread
// ������    IN PEPROCESS EProcess
// ������    OUT PPROCESS_THREAD_INFORMATION pti
// ������    IN UINT32 ThreadCount
// ˵����    ��ȡ�����߳���Ϣ
//************************************
VOID APGetProcessThreadInfo(IN PETHREAD EThread, IN PEPROCESS EProcess, OUT PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// ������:   APEnumProcessThreadByIterateFirstLevelHandleTable
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN UINT_PTR TableCode
// ������    IN PEPROCESS EProcess
// ������    OUT PPROCESS_THREAD_INFORMATION pti
// ������    IN UINT32 ThreadCount
// ˵����    ����һ����
//************************************
VOID APEnumProcessThreadByIterateFirstLevelHandleTable(IN UINT_PTR TableCode, IN PEPROCESS EProcess, OUT PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// ������:   APEnumProcessThreadByIterateSecondLevelHandleTable
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN UINT_PTR TableCode
// ������    IN PEPROCESS EProcess
// ������    OUT PPROCESS_THREAD_INFORMATION pti
// ������    IN UINT32 ThreadCount
// ˵����    ����������
//************************************
VOID APEnumProcessThreadByIterateSecondLevelHandleTable(IN UINT_PTR TableCode, IN PEPROCESS EProcess, OUT PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// ������:   APEnumProcessThreadByIterateThirdLevelHandleTable
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN UINT_PTR TableCode
// ������    IN PEPROCESS EProcess
// ������    OUT PPROCESS_THREAD_INFORMATION pti
// ������    IN UINT32 ThreadCount
// ˵����    ����������
//************************************
VOID APEnumProcessThreadByIterateThirdLevelHandleTable(IN UINT_PTR TableCode, IN PEPROCESS EProcess, OUT PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// ������:   APEnumProcessThreadByIteratePspCidTable
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN PEPROCESS EProcess
// ������    OUT PPROCESS_THREAD_INFORMATION pti
// ������    IN UINT32 ThreadCount
// ˵����    ͨ������EProcess��KProcess��ThreadListHead����
//************************************
NTSTATUS APEnumProcessThreadByIteratePspCidTable(IN PEPROCESS EProcess, OUT PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// ������:   APEnumProcessThreadByIterateThreadListHead
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN PEPROCESS EProcess
// ������    OUT PPROCESS_THREAD_INFORMATION pti
// ������    IN UINT32 ThreadCount
// ˵����    ͨ������EProcess��KProcess��ThreadListHead����
//************************************
NTSTATUS APEnumProcessThreadByIterateThreadListHead(IN PEPROCESS EProcess, OUT PPROCESS_THREAD_INFORMATION pti, IN UINT32 ThreadCount);

//************************************
// ������:   APEnumProcessThread
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN UINT32 ProcessId                 PID
// ������    OUT PVOID OutputBuffer
// ������    IN UINT32 OutputLength
// ˵����    �������̵��߳�
//************************************
NTSTATUS APEnumProcessThread(IN UINT32 ProcessId, OUT PVOID OutputBuffer, IN UINT32 OutputLength);

//************************************
// ������:   APGetPspTerminateThreadByPointerAddress
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ˵����    ͨ��PsTerminateSystemThread���PspTerminateThreadByPointer������ַ
//************************************
UINT_PTR APGetPspTerminateThreadByPointerAddress();

//************************************
// ������:   APTerminateProcessByIterateThreadListHead
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN PEPROCESS EProcess
// ˵����    ����������������̣߳�ͨ��PspTerminateThreadByPointer�����̣߳��Ӷ���������
//************************************
NTSTATUS APTerminateProcessByIterateThreadListHead(IN PEPROCESS EProcess);




