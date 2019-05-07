#include "GetSSDTFuncAddress.h"
#include "ZwQueryVirtualMemory.h"
#include "Common.h"

extern COMMON_INFO g_CommonInfo;
ULONG_PTR   IndexOffset = 0;

ULONG_PTR  GetFuncAddress(PCHAR szFuncName)
{
    ULONG_PTR SSDTDescriptor = 0;
    ULONG_PTR ulIndex = 0;
    ULONG_PTR SSDTFuncAddress = 0;
    g_CommonInfo.WinVersion = GetWindowsVersion();

    switch(g_CommonInfo.WinVersion)
    {
        case WINDOWS_7_7601:
        {
            SSDTDescriptor = GetKeServiceDescriptorTable64();
            IndexOffset = 4;
            break;
        }

        default:
            return 0;
    }

    ulIndex = GetSSDTApiFunIndex(szFuncName);
    SSDTFuncAddress = GetSSDTApiFunAddress(ulIndex, SSDTDescriptor);
    return SSDTFuncAddress;
}



ULONG_PTR GetSSDTApiFunAddress(ULONG_PTR ulIndex,ULONG_PTR SSDTDescriptor)
{
    ULONG_PTR  SSDTFuncAddress = 0;
    switch(g_CommonInfo.WinVersion)
    {
        case WINDOWS_7_7601:
        {
            SSDTFuncAddress = GetSSDTFunctionAddress_7601(ulIndex, SSDTDescriptor);
        }
        break;
        default: 
            SSDTFuncAddress = 0;
            break;
    }

    return SSDTFuncAddress;
}





ULONG_PTR GetSSDTFunctionAddress_7601(ULONG_PTR ulIndex,ULONG_PTR SSDTDescriptor)
{
    LONG dwTemp = 0;
    ULONG_PTR qwTemp = 0;
    ULONG_PTR ServiceTableBase = 0;
    ULONG_PTR FuncAddress = 0;
    PSYSTEM_SERVICE_TABLE64 SSDT = (PSYSTEM_SERVICE_TABLE64)SSDTDescriptor;
    ServiceTableBase = (ULONG_PTR)(SSDT->ServiceTableBase);
    qwTemp = ServiceTableBase + 4 * ulIndex;
    dwTemp = *(PLONG)qwTemp;
    dwTemp = dwTemp >> 4;
    FuncAddress = ServiceTableBase + (ULONG_PTR)dwTemp;
    return FuncAddress;
}


LONG GetSSDTApiFunIndex(IN LPSTR lpszFunName)
{
    LONG Index = -1;
    NTSTATUS Status = STATUS_UNSUCCESSFUL;
    PVOID    pMapBase = NULL;
    PIMAGE_NT_HEADERS  NtHeader;
    PIMAGE_EXPORT_DIRECTORY ExportTable;
    PULONG  FunctionAddresses;
    PULONG  FunctionNames;
    PUSHORT FunIndexs;
    ULONG   ulFunIndex;
    PCHAR   FunName;
    SIZE_T  ViewSize=0;
    ULONG_PTR FunAddress;
    WCHAR wzNtdll[] = L"\\SystemRoot\\System32\\ntdll.dll";

    Status = MapFileInUserSpace(wzNtdll, NtCurrentProcess(), &pMapBase, &ViewSize);
    if (!NT_SUCCESS(Status))
    {
        return STATUS_UNSUCCESSFUL;
    }
    else
    {

        __try
        {
            NtHeader = RtlImageNtHeader(pMapBase);
            if (NtHeader && NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress) 
            {
                ExportTable = (IMAGE_EXPORT_DIRECTORY *)((ULONG_PTR)pMapBase + NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
                FunctionAddresses = (ULONG*)((ULONG_PTR)pMapBase + ExportTable->AddressOfFunctions);
                FunctionNames = (ULONG*)((ULONG_PTR)pMapBase + ExportTable->AddressOfNames);
                FunIndexs = (USHORT*)((ULONG_PTR)pMapBase + ExportTable->AddressOfNameOrdinals);
                for (ULONG uIndex = 0; uIndex < ExportTable->NumberOfNames; uIndex++)
                {
                    FunName = (LPSTR)((ULONG_PTR)pMapBase + FunctionNames[uIndex]);
                    if (_stricmp(FunName, lpszFunName) == 0)
                    {
                        ulFunIndex = FunIndexs[uIndex];
                        FunAddress = (ULONG_PTR)((ULONG_PTR)pMapBase + FunctionAddresses[ulFunIndex]);
                        Index = *(ULONG*)(FunAddress + IndexOffset);
                        break;
                    }
                }
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
        }
    }

    if (Index == -1)
    {
        DbgPrint("%s Get Index Error\n", lpszFunName);
    }

    ZwUnmapViewOfSection(NtCurrentProcess(), pMapBase);
    return Index;
}




ULONG_PTR GetKeServiceDescriptorTable64()
{
    PUCHAR StartSearchAddress = (PUCHAR)__readmsr(0xC0000082);
    PUCHAR EndSearchAddress = StartSearchAddress + 0x500;
    PUCHAR i = NULL;
    ULONG_PTR Temp = 0;
    ULONG_PTR Address = 0;
    for (i = StartSearchAddress; i < EndSearchAddress; i++)
    {
        if (MmIsAddressValid(i) && MmIsAddressValid(i + 1) && MmIsAddressValid(i + 2))
        {
            if (*i == 0x4c && *(i + 1) == 0x8d && *(i + 2) == 0x15) //4c8d15
            {
                memcpy(&Temp, i + 3, 4);
                Address = (ULONG_PTR)Temp + (ULONG_PTR)i + 7;
                return Address;
            }
        }
    }
    return 0;
}




NTSTATUS MapFileInUserSpace(IN LPWSTR lpszFileName, IN HANDLE ProcessHandle OPTIONAL, OUT PVOID *ppBaseAddress, OUT PSIZE_T ViewSize OPTIONAL)
{
    NTSTATUS Status = STATUS_SUCCESS;
    HANDLE   hFile = NULL;
    HANDLE   hSection = NULL;
    OBJECT_ATTRIBUTES oa;
    SIZE_T MapViewSize = 0;
    IO_STATUS_BLOCK Iosb;
    UNICODE_STRING uniFileName;

    do 
    {
        if (NULL==lpszFileName||NULL==ppBaseAddress)
        {
            Status = STATUS_INVALID_PARAMETER;
            break;
        }
        RtlInitUnicodeString(&uniFileName, lpszFileName);
        InitializeObjectAttributes(&oa, &uniFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
        Status = IoCreateFile(
            &hFile,
            GENERIC_READ | SYNCHRONIZE,
            &oa,
            &Iosb,
            NULL,
            FILE_ATTRIBUTE_NORMAL,
            FILE_SHARE_READ,
            FILE_OPEN,
            FILE_SYNCHRONOUS_IO_NONALERT,
            NULL,
            0,
            CreateFileTypeNone,
            NULL,
            IO_NO_PARAMETER_CHECKING);

        if (!NT_SUCCESS(Status))
        {
            break;
        }

        oa.ObjectName = NULL;
        Status = ZwCreateSection(
            &hSection,
            SECTION_QUERY | SECTION_MAP_READ,
            &oa,
            NULL,
            PAGE_WRITECOPY,
            SEC_IMAGE,
            hFile);
        if (!NT_SUCCESS(Status))
        {
            break;
        }

        if (NULL != ProcessHandle)
        {
            ProcessHandle = NtCurrentProcess();
        }
        Status = ZwMapViewOfSection(
            hSection,
            ProcessHandle,
            ppBaseAddress,
            0,
            0,
            0,
            ViewSize != NULL ? ViewSize : &MapViewSize,
            ViewUnmap,
            0,
            PAGE_WRITECOPY);

    } while (FALSE);

    if (hFile != NULL)
    {
        ZwClose(hFile);
        hFile = NULL;
    }
    if (hSection != NULL)
    {
        ZwClose(hSection);
        hSection = NULL;
    }

    return Status;
}
