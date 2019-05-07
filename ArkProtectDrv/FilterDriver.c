#include "FilterDriver.h"
#include "main.h"

extern GOLBAL_INFO g_DriverInfo;

FILTER_DRIVER_INFO g_Filter_Info = { 0 };


NTSTATUS APGetFilterDriverInfo(IN PDEVICE_OBJECT HighDeviceObject, IN PDRIVER_OBJECT LowDriverObject, OUT PFILTER_DRIVER_INFORMATION fdi, IN UINT32 FilterDriverCount, IN ULONG FilterType)
{

    NTSTATUS status = STATUS_SUCCESS;
    PDRIVER_OBJECT HighDriverObject = NULL;
    PLDR_DATA_TABLE_ENTRY LdrDataTableEntry = NULL;
    UINT32 uIndex = 0;
    do
    {
        if (NULL == HighDeviceObject ||
            NULL == LowDriverObject ||
            NULL == fdi ||
            !MmIsAddressValid((PVOID)HighDeviceObject) ||
            !MmIsAddressValid((PVOID)LowDriverObject) ||
            !MmIsAddressValid((PVOID)fdi))
        {
            status = STATUS_INVALID_PARAMETER;
            break;
        }
        if (FilterDriverCount <= fdi->NumberOfFilterDrivers)
        {
            status = STATUS_BUFFER_TOO_SMALL;
            break;
        }

        HighDriverObject = HighDeviceObject->DriverObject;        // 去挂钩设备的驱动对象（上层）       
        if (FilterType == ft_File || FilterType == ft_Raw)
        {

            for (uIndex = 0; uIndex < fdi->NumberOfFilterDrivers; uIndex++)
            {
                if (_wcsnicmp(fdi->FilterDriverEntry[uIndex].wzFilterDriverName, HighDriverObject->DriverName.Buffer, wcslen(fdi->FilterDriverEntry[uIndex].wzFilterDriverName)) == 0 &&
                    _wcsnicmp(fdi->FilterDriverEntry[uIndex].wzAttachedDriverName, HighDriverObject->DriverName.Buffer, wcslen(fdi->FilterDriverEntry[uIndex].wzAttachedDriverName)) == 0)
                {
                    return STATUS_SUCCESS;
                }
            }
        }
        if (FilterType == ft_Volume)
        {
            UINT32 i = 0;
            for (i = 0; i < fdi->NumberOfFilterDrivers; i++)
            {
                if (_wcsnicmp(fdi->FilterDriverEntry[i].wzFilterDriverName, HighDriverObject->DriverName.Buffer, wcslen(fdi->FilterDriverEntry[i].wzFilterDriverName)) == 0 &&
                    _wcsnicmp(fdi->FilterDriverEntry[i].wzAttachedDriverName, HighDriverObject->DriverName.Buffer, wcslen(fdi->FilterDriverEntry[i].wzAttachedDriverName)) == 0)
                {
                    return STATUS_SUCCESS;
                }
            }
        }

        fdi->FilterDriverEntry[fdi->NumberOfFilterDrivers].FilterType = FilterType;
        fdi->FilterDriverEntry[fdi->NumberOfFilterDrivers].FilterDeviceObject = (UINT_PTR)HighDeviceObject;

        // 挂钩驱动（上层）（过滤驱动对象）
        if (APIsUnicodeStringValid(&(HighDriverObject->DriverName)))
        {
            RtlCopyMemory(fdi->FilterDriverEntry[fdi->NumberOfFilterDrivers].wzFilterDriverName, HighDriverObject->DriverName.Buffer, HighDriverObject->DriverName.Length);
        }

        // 挂钩驱动（上层）（过滤驱动的设备对象名称）
        APGetDeviceObjectNameInfo(HighDeviceObject, fdi->FilterDriverEntry[fdi->NumberOfFilterDrivers].wzFilterDeviceName);

        // 被挂钩驱动（下层）(宿主驱动对象)
        if (APIsUnicodeStringValid(&(LowDriverObject->DriverName)))
        {
            RtlCopyMemory(fdi->FilterDriverEntry[fdi->NumberOfFilterDrivers].wzAttachedDriverName, LowDriverObject->DriverName.Buffer, LowDriverObject->DriverName.Length);
        }

        // 过滤驱动对象路径
        LdrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)HighDriverObject->DriverSection;

        if ((UINT_PTR)LdrDataTableEntry > g_DriverInfo.DynamicData.MinKernelSpaceAddress)
        {
            if (APIsUnicodeStringValid(&(LdrDataTableEntry->FullDllName)))
            {
                RtlCopyMemory(fdi->FilterDriverEntry[fdi->NumberOfFilterDrivers].wzFilePath, LdrDataTableEntry->FullDllName.Buffer, LdrDataTableEntry->FullDllName.Length);
            }
            else if (APIsUnicodeStringValid(&(LdrDataTableEntry->BaseDllName)))
            {
                RtlCopyMemory(fdi->FilterDriverEntry[fdi->NumberOfFilterDrivers].wzFilePath, LdrDataTableEntry->BaseDllName.Buffer, LdrDataTableEntry->BaseDllName.Length);
            }
        }
        status = STATUS_SUCCESS;
    } while (FALSE);

    fdi->NumberOfFilterDrivers++;
    return status;
}


NTSTATUS APGetFilterDriverByDriverName(IN PWCHAR wzDriverName, OUT  PFILTER_DRIVER_INFORMATION fdi, IN UINT32 FilterDriverCount, IN ULONG FilterType)
{
    NTSTATUS Status = STATUS_UNSUCCESSFUL;
    UNICODE_STRING uniDriverName;
    PDRIVER_OBJECT DriverObject = NULL;
    PDEVICE_OBJECT DeviceObject = NULL;
    PDRIVER_OBJECT LowDriverObject = NULL;
    PDEVICE_OBJECT HighDeviceObject = NULL;
    do 
    {
        RtlInitUnicodeString(&uniDriverName, wzDriverName);

        Status = ObReferenceObjectByName(
            &uniDriverName,
            OBJ_CASE_INSENSITIVE,
            NULL,
            0,
            *IoDriverObjectType,
            KernelMode,
            NULL,
            (PVOID*)&DriverObject);
        if (!NT_SUCCESS(Status))
        {
            break;
        }
        for (DeviceObject = DriverObject->DeviceObject;
            DeviceObject && MmIsAddressValid((PVOID)DeviceObject);
            DeviceObject = DeviceObject->NextDevice)
        {
            LowDriverObject = DeviceObject->DriverObject;

            // 遍历垂直层次结构 AttachedDevice  设备栈
            for (HighDeviceObject = DeviceObject->AttachedDevice;
                NULL != HighDeviceObject;
                HighDeviceObject = HighDeviceObject->AttachedDevice)
            {
                // HighDeviceObject --> 去挂载的驱动（上层）
                // LowDriverObject --> 被挂载的驱动（下层）
                Status = APGetFilterDriverInfo(HighDeviceObject, LowDriverObject, fdi, FilterDriverCount, FilterType);
                LowDriverObject = HighDeviceObject->DriverObject;
            }

        }
        ObDereferenceObject(DriverObject);
    } while (FALSE);

    return Status;
}




NTSTATUS APEnumFilterDriver(OUT PVOID OutputBuffer, IN UINT32 OutputLength)
{
    NTSTATUS Status = STATUS_UNSUCCESSFUL;

    PFILTER_DRIVER_INFORMATION fdi = (PFILTER_DRIVER_INFORMATION)OutputBuffer;

    UINT32 FilterDriverCount = (OutputLength - sizeof(FILTER_DRIVER_INFORMATION)) / sizeof(FILTER_DRIVER_ENTRY_INFORMATION);

    // 写上所有的驱动名。

    g_Filter_Info.VolumeStartCount = 0;
    g_Filter_Info.FileSystemStartCount = 0;

    APGetFilterDriverByDriverName(L"\\Driver\\Disk", fdi, FilterDriverCount, ft_Disk);
    APGetFilterDriverByDriverName(L"\\Driver\\volmgr", fdi, FilterDriverCount, ft_Volume);
    APGetFilterDriverByDriverName(L"\\FileSystem\\ntfs", fdi, FilterDriverCount, ft_File);
    APGetFilterDriverByDriverName(L"\\FileSystem\\fastfat", fdi, FilterDriverCount, ft_File);
    APGetFilterDriverByDriverName(L"\\Driver\\kbdclass", fdi, FilterDriverCount, ft_Keyboard);
    APGetFilterDriverByDriverName(L"\\Driver\\mouclass", fdi, FilterDriverCount, ft_Mouse);
    APGetFilterDriverByDriverName(L"\\Driver\\i8042prt", fdi, FilterDriverCount, ft_I8042prt);
    APGetFilterDriverByDriverName(L"\\Driver\\tdx", fdi, FilterDriverCount, ft_Tdx);
    APGetFilterDriverByDriverName(L"\\Driver\\NDIS", fdi, FilterDriverCount, ft_Ndis);
    APGetFilterDriverByDriverName(L"\\Driver\\PnpManager", fdi, FilterDriverCount, ft_PnpManager);
    APGetFilterDriverByDriverName(L"\\FileSystem\\Raw", fdi, FilterDriverCount, ft_Raw);


    if (FilterDriverCount >= fdi->NumberOfFilterDrivers)
    {
        Status = STATUS_SUCCESS;
    }
    else
    {
        Status = STATUS_BUFFER_TOO_SMALL;
    }

    return Status;
}