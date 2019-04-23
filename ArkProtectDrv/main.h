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
    PDRIVER_OBJECT          DriverObject;           // 保存全局驱动对象
    PEPROCESS               SystemEProcess;         // 保存全局系统进程
    PLDR_DATA_TABLE_ENTRY   PsLoadedModuleList;     // 加载模块List
}GOLBAL_INFO,*PGOLBAL_INFO;
NTSTATUS APInitializeDynamicData(IN OUT PDYNAMIC_DATA DynamicData);

NTSTATUS APDefaultPassThrough(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

//************************************
// 函数名:   APUnloadDriver
// 权限：    public 
// 返回值:   VOID
// 参数：    IN PDRIVER_OBJECT DriverObject
// 说明：    卸载函数
//************************************
VOID APUnloadDriver(IN PDRIVER_OBJECT DriverObject);
