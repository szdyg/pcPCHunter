#pragma once

#include <ntifs.h>
#include "Dispatch.h"
#include "Private.h"


#define DEVICE_NAME  L"\\Device\\ArkProtectDeviceName"
#define LINK_NAME    L"\\??\\ArkProtectLinkName"


typedef struct _GOLBAL_INFO
{
    DYNAMIC_DATA            DynamicData;
    RTL_OSVERSIONINFOW      OsVerSion;
    PDRIVER_OBJECT          DriverObject;           // ����ȫ����������
    PEPROCESS               SystemEProcess;         // ����ȫ��ϵͳ����
    PLDR_DATA_TABLE_ENTRY   PsLoadedModuleList;     // ����ģ��List
}GOLBAL_INFO,*PGOLBAL_INFO;


//************************************
// ������:   APInitializeDynamicData
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN OUT PDYNAMIC_DATA DynamicData
// ˵����    ��ʼ����Ϣ
//************************************
NTSTATUS APInitializeDynamicData(IN OUT PDYNAMIC_DATA DynamicData);

//************************************
// ������:   APDefaultPassThrough
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN PDEVICE_OBJECT DeviceObject
// ������    IN PIRP Irp
// ˵����    �����ĵ�Irpֱ�����
//************************************
NTSTATUS APDefaultPassThrough(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

//************************************
// ������:   APUnloadDriver
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN PDRIVER_OBJECT DriverObject
// ˵����    ж�غ���
//************************************
VOID APUnloadDriver(IN PDRIVER_OBJECT DriverObject);
