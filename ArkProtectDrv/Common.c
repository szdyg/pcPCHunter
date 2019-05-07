#include "Common.h"
#include "Imports.h"
#include "NtStructs.h"

COMMON_INFO g_CommonInfo = { 0 };




WIN_VERSION GetWindowsVersion()
{
    RTL_OSVERSIONINFOEXW osverInfo = { 0 };
    pfnRtlGetVersion RtlGetVersion = NULL;
    WIN_VERSION WinVersion;
    WCHAR szRtlGetVersion[] = L"RtlGetVersion";

    RtlGetVersion = (pfnRtlGetVersion)GetFunctionAddressByName(szRtlGetVersion); 

    if (RtlGetVersion)
    {
        RtlGetVersion((PRTL_OSVERSIONINFOW)&osverInfo); 
    } 
    else 
    {
        PsGetVersion(&osverInfo.dwMajorVersion, &osverInfo.dwMinorVersion, &osverInfo.dwBuildNumber, NULL);
    }

    if(osverInfo.dwMajorVersion == 6 && osverInfo.dwMinorVersion == 1 && osverInfo.dwBuildNumber == 7600)
    {
        DbgPrint("WINDOWS 7\r\n");
        WinVersion = WINDOWS_7_7600;
    }
    else if(osverInfo.dwMajorVersion == 6 && osverInfo.dwMinorVersion == 1 && osverInfo.dwBuildNumber == 7601)
    {
        DbgPrint("WINDOWS 7\r\n");
        WinVersion = WINDOWS_7_7601;
    }
    else if(osverInfo.dwMajorVersion == 6 && osverInfo.dwMinorVersion == 2 && osverInfo.dwBuildNumber == 9200)
    {
        DbgPrint("WINDOWS 8\r\n");
        WinVersion = WINDOWS_8_9200;
    }
    else if(osverInfo.dwMajorVersion == 6 && osverInfo.dwMinorVersion == 3 && osverInfo.dwBuildNumber == 9600)
    {
        DbgPrint("WINDOWS 8.1\r\n");
        WinVersion = WINDOWS_8_9600;
    }
    else if(osverInfo.dwMajorVersion == 10 && osverInfo.dwMinorVersion == 0 && osverInfo.dwBuildNumber == 10240)
    {
        DbgPrint("WINDOWS 10 10240\r\n");
        WinVersion = WINDOWS_10_10240;
    }
    else if(osverInfo.dwMajorVersion == 10 && osverInfo.dwMinorVersion == 0 && osverInfo.dwBuildNumber == 10586)
    {
        DbgPrint("WINDOWS 10 10586\r\n");
        WinVersion = WINDOWS_10_10586;
    }
    else if(osverInfo.dwMajorVersion == 10 && osverInfo.dwMinorVersion == 0 && osverInfo.dwBuildNumber == 14393)
    {
        DbgPrint("WINDOWS 10 14393\r\n");
        WinVersion = WINDOWS_10_14393;
    }
    else if(osverInfo.dwMajorVersion == 10 && osverInfo.dwMinorVersion == 0 && osverInfo.dwBuildNumber == 15063)
    {
        DbgPrint("WINDOWS 10 15063\r\n");
        WinVersion = WINDOWS_10_15063;
    }
    else if(osverInfo.dwMajorVersion == 10 && osverInfo.dwMinorVersion == 0 && osverInfo.dwBuildNumber == 16299)
    {
        DbgPrint("WINDOWS 10 16299\r\n");
        WinVersion = WINDOWS_10_16299;
    }
    else if(osverInfo.dwMajorVersion == 10 && osverInfo.dwMinorVersion == 0 && osverInfo.dwBuildNumber == 17134)
    {
        DbgPrint("WINDOWS 10 17134\r\n");
        WinVersion = WINDOWS_10_17134;
    }
    else
    {
        DbgPrint("This is a new os\r\n");
        WinVersion = WINDOWS_UNKNOW;
    }

    return WinVersion;
}



PVOID GetFunctionAddressByName(WCHAR *wzFunction)
{
    UNICODE_STRING uniFunction;  
    PVOID AddrBase = NULL;
    if (wzFunction && wcslen(wzFunction) > 0)
    {
        RtlInitUnicodeString(&uniFunction, wzFunction);      //常量指针
        AddrBase = MmGetSystemRoutineAddress(&uniFunction);  //在System 进程  第一个模块  Ntosknrl.exe  ExportTable
    }
    return AddrBase;
}


VOID InitGlobalVariable()
{
    g_CommonInfo.WinVersion = GetWindowsVersion();
    switch(g_CommonInfo.WinVersion)
    {
        case WINDOWS_7_7600:
        case WINDOWS_7_7601:
        {
            g_CommonInfo.LdrInPebOffset = 0x018;
            g_CommonInfo.ModListInLdrOffset = 0x010;
            g_CommonInfo.ObjectTableOffsetOf_EPROCESS = 0x200;
            g_CommonInfo.PreviousModeOffsetOf_KTHREAD = 0x1f6;
            g_CommonInfo.HighUserAddress = 0x80000000000;
        }
        break;

        default:
            break;;
    }
}



BOOLEAN IsRealProcess(PEPROCESS EProcess) 
{ 
    ULONG_PTR    ObjectType; 
    ULONG_PTR    ObjectTypeAddress; 
    BOOLEAN bRet = FALSE;

    ULONG_PTR ProcessType = ((ULONG_PTR)*PsProcessType);

    if (ProcessType && EProcess && MmIsAddressValid((PVOID)(EProcess)))
    { 
        ObjectType = KeGetObjectType((PVOID)EProcess);   //*PsProcessType  
        if (ObjectType && ProcessType == ObjectType && !IsProcessDie(EProcess))
        {
            bRet = TRUE; 
        }
    } 

    return bRet; 
} 



ULONG_PTR KeGetObjectType(PVOID Object)
{
    ULONG_PTR ObjectType = 0;
    pfnObGetObjectType        ObGetObjectType = NULL;    

    if (NULL == MmIsAddressValid ||!Object||!MmIsAddressValid(Object))
    {
        return 0;
    }

    if (g_CommonInfo.WinVersion == WINDOWS_XP)
    {
        ULONG SizeOfObjectHeader = 0, ObjectTypeOffset = 0;
        ULONG_PTR ObjectTypeAddress = 0;
        ObjectTypeAddress = (ULONG_PTR)Object - g_CommonInfo.ObjectHeaderSize + g_CommonInfo.ObjectTypeOffsetOf_Object_Header;
        if (MmIsAddressValid((PVOID)ObjectTypeAddress))
        {
            ObjectType = *(ULONG_PTR*)ObjectTypeAddress;
        }
    }
    else
    {
        //高版本使用函数
        ObGetObjectType = (pfnObGetObjectType)GetFunctionAddressByName(L"ObGetObjectType");
        if (ObGetObjectType)
        {
            ObjectType = ObGetObjectType(Object);
        }
    }

    return ObjectType;
}

BOOLEAN IsProcessDie(PEPROCESS EProcess)
{
    BOOLEAN bDie = FALSE;
    do
    {
        if (NULL == EProcess)
            break;

        if (MmIsAddressValid(EProcess) && MmIsAddressValid((PVOID)((ULONG_PTR)EProcess + g_CommonInfo.ObjectTableOffsetOf_EPROCESS)))
        {
            PVOID ObjectTable = *(PVOID*)((ULONG_PTR)EProcess + g_CommonInfo.ObjectTableOffsetOf_EPROCESS);
            if (!ObjectTable || !MmIsAddressValid(ObjectTable))
            {
                DbgPrint("Process is Die\r\n");
                bDie = TRUE;
            }
        }
        else
        {
            DbgPrint("Process is Die2\r\n");
            bDie = TRUE;
        }
    } while (FALSE);
    return bDie;
}




CHAR ChangePreMode(PETHREAD EThread)
{

    CHAR PreMode = *(PCHAR)((ULONG_PTR)EThread + g_CommonInfo.PreviousModeOffsetOf_KTHREAD);
    *(PCHAR)((ULONG_PTR)EThread + g_CommonInfo.PreviousModeOffsetOf_KTHREAD) = KernelMode;
    return PreMode;
}

VOID RecoverPreMode(PETHREAD EThread, CHAR PreMode)
{
    *(PCHAR)((ULONG_PTR)EThread + g_CommonInfo.PreviousModeOffsetOf_KTHREAD) = PreMode;
}



BOOLEAN NtPathToDosPathW(PWCHAR wzFullNtPath,PWCHAR wzFullDosPath)
{
    WCHAR wzDosDevice[4] = {0};
    WCHAR wzNtDevice[64] = {0};
    WCHAR *RetStr = NULL;
    size_t NtDeviceLen = 0;
    WCHAR wLetter = L"A";
    BOOLEAN bRet = FALSE;
    do 
    {
        if (NULL == wzFullNtPath || NULL == wzFullDosPath)
            break;
        for (; wLetter < L"Z"; wLetter++)
        {
            wzDosDevice[0] = wLetter;
            wzDosDevice[1] = L':';
            if (NtQueryDosDevice(wzDosDevice, wzNtDevice, 64))
            {
                if (wzNtDevice)
                {
                    NtDeviceLen = wcslen(wzNtDevice);
                    if (!_wcsnicmp(wzNtDevice, wzFullNtPath, NtDeviceLen))
                    {
                        wcscpy(wzFullDosPath, wzDosDevice);
                        wcscat(wzFullDosPath, wzFullNtPath + NtDeviceLen);
                        bRet = TRUE;
                    }
                }
            }
        }
    } while (FALSE);

    return FALSE;
}

ULONG NtQueryDosDevice(PWCHAR wzDosDevice, PWCHAR wzNtDevice, ULONG ucchMax)
{
    NTSTATUS status;
    POBJECT_DIRECTORY_INFORMATION ObjectDirectoryInfor;
    OBJECT_ATTRIBUTES oa;
    UNICODE_STRING uniString;
    HANDLE hDirectory = NULL;
    HANDLE hDevice = NULL;
    ULONG  ulReturnLength;
    ULONG  ulNameLength;
    ULONG  ulLength = 0;
    ULONG  Context;
    BOOLEAN     bRestartScan;
    PWCHAR      Ptr = NULL;
    PUCHAR      pData = NULL;
    do
    {
        pData = ExAllocatePool(NonPagedPool, 1024);
        if (NULL == pData)
        {
            break;
        }
        if (wzDosDevice != NULL)
        {
            RtlInitUnicodeString(&uniString, L"\\??");
            InitializeObjectAttributes(&oa, &uniString, OBJ_CASE_INSENSITIVE, NULL, NULL);
            status = ZwOpenDirectoryObject(&hDirectory, DIRECTORY_QUERY, &oa);
            if (!NT_SUCCESS(status))
            {
                break;
            }
            RtlInitUnicodeString(&uniString, wzDosDevice);
            InitializeObjectAttributes(&oa, &uniString, OBJ_CASE_INSENSITIVE, hDirectory, NULL);
            status = ZwOpenSymbolicLinkObject(&hDevice, GENERIC_READ, &oa);
            if (!NT_SUCCESS(status))
            {
                break;
            }
            uniString.Length = 0;
            uniString.MaximumLength = (USHORT)ucchMax * sizeof(WCHAR);
            uniString.Buffer = wzNtDevice;
            ulReturnLength = 0;
            status = ZwQuerySymbolicLinkObject(hDevice, &uniString, &ulReturnLength);
            if (!NT_SUCCESS(status))
            {
                break;
            }
            ulLength = uniString.Length / sizeof(WCHAR);
            if (ulLength < ucchMax)
            {
                wzNtDevice[ulLength] = UNICODE_NULL;
                ulLength++;
            }
            else
            {
                break;
            }
        }
        else
        {
            bRestartScan = TRUE;
            Context = 0;
            Ptr = wzNtDevice;
            ObjectDirectoryInfor = (POBJECT_DIRECTORY_INFORMATION)pData;
            while (TRUE)
            {
                status = ZwQueryDirectoryObject(hDirectory, pData, 1024, TRUE, bRestartScan, &Context, &ulReturnLength);
                if (!NT_SUCCESS(status))
                {
                    if (status == STATUS_NO_MORE_ENTRIES)
                    {
                        *Ptr = UNICODE_NULL;
                        ulLength++;
                        status = STATUS_SUCCESS;
                    }
                    else
                    {
                        ulLength = 0;
                    }
                    break;
                }
                if (!wcscmp(ObjectDirectoryInfor->TypeName.Buffer, L"SymbolicLink"))
                {
                    ulNameLength = ObjectDirectoryInfor->Name.Length / sizeof(WCHAR);
                    if (ulLength + ulNameLength + 1 >= ucchMax)
                    {
                        ulLength = 0;
                        break;
                    }
                    memcpy(Ptr, ObjectDirectoryInfor->Name.Buffer, ObjectDirectoryInfor->Name.Length);
                    Ptr += ulNameLength;
                    ulLength += ulNameLength;
                    *Ptr = UNICODE_NULL;
                    Ptr++;
                    ulLength++;
                }
                bRestartScan = FALSE;
            }
        }

    } while (FALSE);

    if (hDirectory != NULL)
    {
        ZwClose(hDirectory);
        hDirectory = NULL;
    }
    if (hDevice != NULL)
    {
        ZwClose(hDevice);
        hDevice = NULL;
    }
    if (pData != NULL)
    {
        ExFreePool(pData);
        pData = NULL;
    }

    return ulLength;
}