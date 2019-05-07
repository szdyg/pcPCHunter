#pragma once
#include <ntifs.h>
#include "Private.h"
#include "Imports.h"
#include "NtStructs.h"
#include "ProcessCore.h"
#include "ProcessThread.h"

typedef struct _DRIVER_ENTRY_INFORMATION
{
    UINT_PTR  BaseAddress;
    UINT_PTR  Size;
    UINT_PTR  DriverObject;
    UINT_PTR  DirverStartAddress;
    UINT_PTR  LoadOrder;
    WCHAR     wzDriverName[100];
    WCHAR     wzDriverPath[MAX_PATH];
    WCHAR     wzServiceName[MAX_PATH];
    WCHAR     wzCompanyName[MAX_PATH];
} DRIVER_ENTRY_INFORMATION, *PDRIVER_ENTRY_INFORMATION;

typedef struct _DRIVER_INFORMATION
{
    UINT32                          NumberOfDrivers;
    DRIVER_ENTRY_INFORMATION        DriverEntry[1];
} DRIVER_INFORMATION, *PDRIVER_INFORMATION;


//************************************
// ������:   APGetDriverModuleLdr
// Ȩ�ޣ�    public 
// ����ֵ:   PLDR_DATA_TABLE_ENTRY
// ������    IN const PWCHAR  wzDriverName
// ������    IN PLDR_DATA_TABLE_ENTRY PsLoadedModuleList
// ˵����    ͨ������Ldrö���ں�ģ�� ������˳����
//************************************
PLDR_DATA_TABLE_ENTRY APGetDriverModuleLdr(IN const PWCHAR  wzDriverName, IN PLDR_DATA_TABLE_ENTRY PsLoadedModuleList);

//************************************
// ������:   APEnumDriverModuleByLdrDataTableEntry
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN PLDR_DATA_TABLE_ENTRY PsLoadedModuleList       �ں�ģ�����List
// ������    OUT PDRIVER_INFORMATION di                        Ring3Buffer
// ������    IN UINT32 DriverCount
// ˵����    ͨ������Ldrö���ں�ģ�� ������˳����
//************************************
NTSTATUS APEnumDriverModuleByLdrDataTableEntry(IN PLDR_DATA_TABLE_ENTRY PsLoadedModuleList, OUT PDRIVER_INFORMATION di, IN UINT32 DriverCount);

//************************************
// ������:   APIsDriverInList
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    IN PDRIVER_INFORMATION di
// ������    IN PDRIVER_OBJECT DriverObject        ��������
// ������    IN UINT32 DriverCount
// ˵����    �鿴����Ķ����Ƿ��Ѿ����ڽṹ���У������ �����������Ϣ��������ڣ��򷵻�false������ĸ������
//************************************
BOOLEAN APIsDriverInList(IN PDRIVER_INFORMATION di, IN PDRIVER_OBJECT DriverObject, IN UINT32 DriverCount);

//************************************
// ������:   APGetDriverInfo
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    OUT PDRIVER_INFORMATION di
// ������    IN PDRIVER_OBJECT DriverObject        ��������
// ������    IN UINT32 DriverCount
// ˵����    ��������������Ϣ
//************************************
VOID APGetDriverInfo(OUT PDRIVER_INFORMATION di, IN PDRIVER_OBJECT DriverObject, IN UINT32 DriverCount);

//************************************
// ������:   APEnumDriverModuleByIterateDirectoryObject
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    OUT PDRIVER_INFORMATION di
// ������    IN UINT32 DriverCount
// ˵����    ͨ������Ŀ¼����������ϵͳ�ڵ���������
//************************************
VOID APEnumDriverModuleByIterateDirectoryObject(OUT PDRIVER_INFORMATION di, IN UINT32 DriverCount);

//************************************
// ������:   APEnumDriverInfo
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    OUT PVOID OutputBuffer            Ring3Buffer
// ������    IN UINT32 OutputLength            Ring3BufferLength
// ˵����    ö��������Ϣ
//************************************
NTSTATUS APEnumDriverInfo(OUT PVOID OutputBuffer, IN UINT32 OutputLength);

//************************************
// ������:   APIsValidDriverObject
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    IN PDRIVER_OBJECT DriverObject
// ˵����    �ж�һ�����������Ƿ�Ϊ��
//************************************
BOOLEAN APIsValidDriverObject(IN PDRIVER_OBJECT DriverObject);

VOID APDriverUnloadThreadCallback(IN PVOID lParam);

NTSTATUS APUnloadDriverByCreateSystemThread(IN PDRIVER_OBJECT DriverObject);

NTSTATUS APUnloadDriverObject(IN UINT_PTR InputBuffer);

VOID APGetDeviceObjectNameInfo(IN PDEVICE_OBJECT DeviceObject, OUT PWCHAR DeviceName);



