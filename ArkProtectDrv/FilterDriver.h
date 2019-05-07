#pragma once
#include <ntifs.h>
#include "Private.h"
#include "DriverCore.h"

typedef enum FILTER_TYPE
{
    ft_Unkonw = 0,
    ft_File,                // 文件------------
    ft_Disk,                // 磁盘    文件相关
    ft_Volume,              // 卷  ------------- 
    ft_Keyboard,            // 键盘
    ft_Mouse,               // 鼠标            硬件接口
    ft_I8042prt,            // 键盘驱动
    ft_Tcpip,               // tcpip-------------------
    ft_Ndis,                // 网络驱动接口
    ft_PnpManager,          // 即插即用管理器       网络相关
    ft_Tdx,                 // 网络相关
    ft_Raw,
};


//FilterDriver模块全局变量结构体
typedef struct _FILTER_DRIVER_INFO
{
    ULONG VolumeStartCount;
    ULONG FileSystemStartCount;
}FILTER_DRIVER_INFO,*PFILTER_DRIVER_INFO;

typedef struct _FILTER_DRIVER_ENTRY_INFORMATION
{
    ULONG       FilterType;
    UINT_PTR    FilterDeviceObject;
    WCHAR       wzFilterDriverName[MAX_PATH];
    WCHAR       wzFilterDeviceName[MAX_PATH];
    WCHAR       wzAttachedDriverName[MAX_PATH];
    WCHAR       wzFilePath[MAX_PATH];
} FILTER_DRIVER_ENTRY_INFORMATION, *PFILTER_DRIVER_ENTRY_INFORMATION;

typedef struct _FILTER_DRIVER_INFORMATION
{
    UINT32                          NumberOfFilterDrivers;
    FILTER_DRIVER_ENTRY_INFORMATION FilterDriverEntry[1];
} FILTER_DRIVER_INFORMATION, *PFILTER_DRIVER_INFORMATION;



//************************************
// 函数名:   APGetFilterDriverInfo
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN PDEVICE_OBJECT AttachDeviceObject            上层设备对象
// 参数：    IN PDRIVER_OBJECT AttachedDriverObject          下层驱动对象
// 参数：    OUT PFILTER_DRIVER_INFORMATION fdi              Ring3Buffer
// 参数：    IN UINT32 FilterDriverCount                     Count
// 参数：    IN eFilterType FilterType                       类型
// 说明：    与驱动层通信，枚举进程模块信息
//************************************
NTSTATUS APGetFilterDriverInfo(IN PDEVICE_OBJECT AttachDeviceObject, IN PDRIVER_OBJECT AttachedDriverObject, OUT PFILTER_DRIVER_INFORMATION fdi, IN UINT32 FilterDriverCount, IN ULONG FilterType);

//************************************
// 函数名:   APGetFilterDriverByDriverName
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN PWCHAR wzDriverName                      驱动名
// 参数：    OUT PFILTER_DRIVER_INFORMATION fdi
// 参数：    IN UINT32 FilterDriverCount                 缓存中能放几个驱动的信息
// 参数：    IN ULONG FilterType                         过滤驱动类型
// 说明：    枚举指定驱动的过滤驱动
//************************************
NTSTATUS APGetFilterDriverByDriverName(IN PWCHAR  wzDriverName, OUT PFILTER_DRIVER_INFORMATION fdi, IN UINT32 FilterDriverCount, IN ULONG FilterType);

//************************************
// 函数名:   APEnumFilterDriver
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    OUT PVOID OutputBuffer
// 参数：    IN UINT32 OutputLength
// 说明：    枚举过滤驱动
//************************************
NTSTATUS APEnumFilterDriver(OUT PVOID OutputBuffer, IN UINT32 OutputLength);


