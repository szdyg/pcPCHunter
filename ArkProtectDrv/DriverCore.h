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
// 函数名:   APGetDriverModuleLdr
// 权限：    public 
// 返回值:   PLDR_DATA_TABLE_ENTRY
// 参数：    IN const PWCHAR  wzDriverName
// 参数：    IN PLDR_DATA_TABLE_ENTRY PsLoadedModuleList
// 说明：    通过遍历Ldr枚举内核模块 按加载顺序来
//************************************
PLDR_DATA_TABLE_ENTRY APGetDriverModuleLdr(IN const PWCHAR  wzDriverName, IN PLDR_DATA_TABLE_ENTRY PsLoadedModuleList);

//************************************
// 函数名:   APEnumDriverModuleByLdrDataTableEntry
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN PLDR_DATA_TABLE_ENTRY PsLoadedModuleList       内核模块加载List
// 参数：    OUT PDRIVER_INFORMATION di                        Ring3Buffer
// 参数：    IN UINT32 DriverCount
// 说明：    通过遍历Ldr枚举内核模块 按加载顺序来
//************************************
NTSTATUS APEnumDriverModuleByLdrDataTableEntry(IN PLDR_DATA_TABLE_ENTRY PsLoadedModuleList, OUT PDRIVER_INFORMATION di, IN UINT32 DriverCount);

//************************************
// 函数名:   APIsDriverInList
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    IN PDRIVER_INFORMATION di
// 参数：    IN PDRIVER_OBJECT DriverObject        驱动对象
// 参数：    IN UINT32 DriverCount
// 说明：    查看传入的对象是否已经存在结构体中，如果在 则继续完善信息，如果不在，则返回false，留给母程序处理
//************************************
BOOLEAN APIsDriverInList(IN PDRIVER_INFORMATION di, IN PDRIVER_OBJECT DriverObject, IN UINT32 DriverCount);

//************************************
// 函数名:   APGetDriverInfo
// 权限：    public 
// 返回值:   VOID
// 参数：    OUT PDRIVER_INFORMATION di
// 参数：    IN PDRIVER_OBJECT DriverObject        驱动对象
// 参数：    IN UINT32 DriverCount
// 说明：    插入驱动对象信息
//************************************
VOID APGetDriverInfo(OUT PDRIVER_INFORMATION di, IN PDRIVER_OBJECT DriverObject, IN UINT32 DriverCount);

//************************************
// 函数名:   APEnumDriverModuleByIterateDirectoryObject
// 权限：    public 
// 返回值:   VOID
// 参数：    OUT PDRIVER_INFORMATION di
// 参数：    IN UINT32 DriverCount
// 说明：    通过遍历目录对象来遍历系统内的驱动对象
//************************************
VOID APEnumDriverModuleByIterateDirectoryObject(OUT PDRIVER_INFORMATION di, IN UINT32 DriverCount);

//************************************
// 函数名:   APEnumDriverInfo
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    OUT PVOID OutputBuffer            Ring3Buffer
// 参数：    IN UINT32 OutputLength            Ring3BufferLength
// 说明：    枚举驱动信息
//************************************
NTSTATUS APEnumDriverInfo(OUT PVOID OutputBuffer, IN UINT32 OutputLength);

//************************************
// 函数名:   APIsValidDriverObject
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    IN PDRIVER_OBJECT DriverObject
// 说明：    判断一个驱动对象是否为真
//************************************
BOOLEAN APIsValidDriverObject(IN PDRIVER_OBJECT DriverObject);

VOID APDriverUnloadThreadCallback(IN PVOID lParam);

NTSTATUS APUnloadDriverByCreateSystemThread(IN PDRIVER_OBJECT DriverObject);

NTSTATUS APUnloadDriverObject(IN UINT_PTR InputBuffer);

VOID APGetDeviceObjectNameInfo(IN PDEVICE_OBJECT DeviceObject, OUT PWCHAR DeviceName);



