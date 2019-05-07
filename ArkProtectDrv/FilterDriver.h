#pragma once
#include <ntifs.h>
#include "Private.h"
#include "DriverCore.h"

typedef enum FILTER_TYPE
{
    ft_Unkonw = 0,
    ft_File,                // �ļ�------------
    ft_Disk,                // ����    �ļ����
    ft_Volume,              // ��  ------------- 
    ft_Keyboard,            // ����
    ft_Mouse,               // ���            Ӳ���ӿ�
    ft_I8042prt,            // ��������
    ft_Tcpip,               // tcpip-------------------
    ft_Ndis,                // ���������ӿ�
    ft_PnpManager,          // ���弴�ù�����       �������
    ft_Tdx,                 // �������
    ft_Raw,
};


//FilterDriverģ��ȫ�ֱ����ṹ��
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
// ������:   APGetFilterDriverInfo
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN PDEVICE_OBJECT AttachDeviceObject            �ϲ��豸����
// ������    IN PDRIVER_OBJECT AttachedDriverObject          �²���������
// ������    OUT PFILTER_DRIVER_INFORMATION fdi              Ring3Buffer
// ������    IN UINT32 FilterDriverCount                     Count
// ������    IN eFilterType FilterType                       ����
// ˵����    ��������ͨ�ţ�ö�ٽ���ģ����Ϣ
//************************************
NTSTATUS APGetFilterDriverInfo(IN PDEVICE_OBJECT AttachDeviceObject, IN PDRIVER_OBJECT AttachedDriverObject, OUT PFILTER_DRIVER_INFORMATION fdi, IN UINT32 FilterDriverCount, IN ULONG FilterType);

//************************************
// ������:   APGetFilterDriverByDriverName
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN PWCHAR wzDriverName                      ������
// ������    OUT PFILTER_DRIVER_INFORMATION fdi
// ������    IN UINT32 FilterDriverCount                 �������ܷż�����������Ϣ
// ������    IN ULONG FilterType                         ������������
// ˵����    ö��ָ�������Ĺ�������
//************************************
NTSTATUS APGetFilterDriverByDriverName(IN PWCHAR  wzDriverName, OUT PFILTER_DRIVER_INFORMATION fdi, IN UINT32 FilterDriverCount, IN ULONG FilterType);

//************************************
// ������:   APEnumFilterDriver
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    OUT PVOID OutputBuffer
// ������    IN UINT32 OutputLength
// ˵����    ö�ٹ�������
//************************************
NTSTATUS APEnumFilterDriver(OUT PVOID OutputBuffer, IN UINT32 OutputLength);


