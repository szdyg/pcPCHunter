#include "Sssdt.h"
#include "main.h"

extern GOLBAL_INFO g_DriverInfo;

extern PWCHAR g_SssdtFunctionName[0x400];

PVOID    g_ReloadWin32kImage = NULL;       // ����Win32k�Ļ���ַ
PKSERVICE_TABLE_DESCRIPTOR g_CurrentWin32pServiceTableAddress = NULL;   // ��ǰϵͳ�����ŵ�Win32k��ServiceTable����ַ
KSERVICE_TABLE_DESCRIPTOR  g_ReloadWin32pServiceTableAddress = { 0 };   // ShadowSsdtҲ��Ntkrnl�ShawdowSsdt->base��Win32k��
UINT_PTR g_OriginalSssdtFunctionAddress[0x400] = { 0 };    // SssdtFunctionԭ���ĵ�ַ
//UINT32   g_SssdtItem[0x400] = { 0 };                       // Sssdt������ԭʼ��ŵ�����



UINT_PTR APGetCurrentSssdtAddress()
{
    UINT_PTR CurrentSssdtAddress = NULL;
    /*
    kd> rdmsr c0000082
    msr[c0000082] = fffff800`03e81640
    */
    PUINT8    StartSearchAddress = (PUINT8)__readmsr(0xC0000082);   // fffff800`03ecf640
    PUINT8    EndSearchAddress = StartSearchAddress + 0x500;
    PUINT8    i = NULL;
    UINT8     v1 = 0, v2 = 0, v3 = 0;
    INT32     iOffset = 0;    // 002320c7 ƫ�Ʋ��ᳬ��4�ֽ�

    if (7 == g_DriverInfo.OsVerSion.dwMajorVersion)
    {
        //Win7
        for (i = StartSearchAddress; i < EndSearchAddress; i++)
        {
            /*
            kd> u fffff800`03e81640 l 500
            nt!KiSystemCall64:
            fffff800`03e81640 0f01f8          swapgs
            ......

            nt!KiSystemServiceRepeat:
            fffff800`03e9c772 4c8d15c7202300  lea     r10,[nt!KeServiceDescriptorTable (fffff800`040ce840)]
            fffff800`03e9c779 4c8d1d00212300  lea     r11,[nt!KeServiceDescriptorTableShadow (fffff800`040ce880)]
            fffff800`03e9c780 f7830001000080000000 test dword ptr [rbx+100h],80h

            TargetAddress = CurrentAddress + Offset + 7
            fffff800`040ce840 = fffff800`03e9c772 + 0x002320c7 + 7
            */

            if (MmIsAddressValid(i) && MmIsAddressValid(i + 1) && MmIsAddressValid(i + 2))
            {
                v1 = *i;
                v2 = *(i + 1);
                v3 = *(i + 2);
                if (v1 == 0x4c && v2 == 0x8d && v3 == 0x1d)        // Ӳ����  lea r11
                {
                    RtlCopyMemory(&iOffset, i + 3, 4);
                    CurrentSssdtAddress = (UINT_PTR)(iOffset + (UINT64)i + 7);
                    break;
                }
            }
        }
    }

    return CurrentSssdtAddress;
}


UINT_PTR APGetCurrentWin32pServiceTable()
{
    /*
    kd> dq fffff800`040be980
    fffff800`040be980  fffff800`03e87800 00000000`00000000
    fffff800`040be990  00000000`00000191 fffff800`03e8848c
    fffff800`040be9a0  fffff960`000e1f00 00000000`00000000
    fffff800`040be9b0  00000000`0000033b fffff960`000e3c1c

    kd> dq win32k!W32pServiceTable
    fffff960`000e1f00  fff0b501`fff3a740 001021c0`000206c0
    fffff960`000e1f10  00022640`00096000 ffde0b03`fff9a900

    */

    if (g_CurrentWin32pServiceTableAddress == NULL)
    {
        (UINT_PTR)g_CurrentWin32pServiceTableAddress = APGetCurrentSssdtAddress() + sizeof(KSERVICE_TABLE_DESCRIPTOR);    // ��Ssdt 
    }

    return (UINT_PTR)g_CurrentWin32pServiceTableAddress;
}



VOID APFixWin32pServiceTable(IN PVOID ImageBase, IN PVOID OriginalBase)
{
    UINT_PTR KrnlOffset = (INT64)((UINT_PTR)ImageBase - (UINT_PTR)OriginalBase);

    DbgPrint("Krnl Offset :%x\r\n", KrnlOffset);

    // ��SSDT��ֵ

    g_ReloadWin32pServiceTableAddress.Base = (PUINT_PTR)((UINT_PTR)(g_CurrentWin32pServiceTableAddress->Base) + KrnlOffset);
    g_ReloadWin32pServiceTableAddress.Limit = g_CurrentWin32pServiceTableAddress->Limit;
    g_ReloadWin32pServiceTableAddress.Number = g_CurrentWin32pServiceTableAddress->Number;

    DbgPrint("New Win32pServiceTable:%p\r\n", g_ReloadWin32pServiceTableAddress);
    DbgPrint("New Win32pServiceTable Base:%p\r\n", g_ReloadWin32pServiceTableAddress.Base);

    // ��Base���ÿ����Ա��ֵ��������ַ��
    if (MmIsAddressValid(g_ReloadWin32pServiceTableAddress.Base))
    {

#ifdef _WIN64
        UINT32 j;
        UINT32 i;
        // �տ�ʼ������Ǻ�������ʵ��ַ�����Ǳ������Լ���ȫ��������
        for (j = 0; j < g_ReloadWin32pServiceTableAddress.Limit; j++)
        {
            g_OriginalSssdtFunctionAddress[j] = *(UINT64*)((UINT_PTR)g_ReloadWin32pServiceTableAddress.Base + j * 8);
        }

        for ( i = 0; i < g_ReloadWin32pServiceTableAddress.Limit; i++)
        {
            UINT32 Temp = 0;
            Temp = (UINT32)(g_OriginalSssdtFunctionAddress[i] - (UINT64)g_CurrentWin32pServiceTableAddress->Base);
            Temp += ((UINT64)g_CurrentWin32pServiceTableAddress->Base & 0xffffffff);
            // ����Ssdt->base�еĳ�ԱΪ�����Base��ƫ��
            *(UINT32*)((UINT64)g_ReloadWin32pServiceTableAddress.Base + i * 4) = (Temp - ((UINT64)g_CurrentWin32pServiceTableAddress->Base & 0xffffffff)) << 4;
        }

        DbgPrint("Current%p\n", g_CurrentWin32pServiceTableAddress->Base);
        DbgPrint("Reload%p\n", g_ReloadWin32pServiceTableAddress.Base);

    /*    for (UINT32 i = 0; i < g_ReloadWin32pServiceTableAddress.Limit; i++)
        {
            g_SssdtItem[i] = *(UINT32*)((UINT64)g_ReloadWin32pServiceTableAddress.Base + i * 4);
        }*/
#else
        UINT32 i;
        for ( i = 0; i < g_ReloadWin32pServiceTableAddress.Limit; i++)
        {
            g_OriginalSssdtFunctionAddress[i] = *(UINT32*)((UINT_PTR)g_ReloadWin32pServiceTableAddress.Base + i * 4);
            //g_SssdtItem[i] = g_OriginalSssdtFunctionAddress[i];
            *(UINT32*)((UINT_PTR)g_ReloadWin32pServiceTableAddress.Base + i * 4) += KrnlOffset;      // ������Ssdt������ַת�������¼��ص��ڴ��еĵ�ַ
        }
#endif // _WIN64

    }
    else
    {
        DbgPrint("New Win32pServiceTable Base is not valid\r\n");
    }
}


NTSTATUS APReloadWin32k()
{
    NTSTATUS    Status = STATUS_SUCCESS;
    if (g_ReloadWin32kImage == NULL)
    {
        PVOID          FileBuffer = NULL;
        PLDR_DATA_TABLE_ENTRY Win32kLdr = NULL;
        Status = STATUS_UNSUCCESSFUL;

        Win32kLdr = APGetDriverModuleLdr(L"win32k.sys", g_DriverInfo.PsLoadedModuleList);

        // 2.��ȡ��һģ���ļ����ڴ棬���ڴ�����ʽ���PE��IAT��BaseReloc�޸�
        FileBuffer = APGetFileBuffer(&Win32kLdr->FullDllName);
        if (FileBuffer)
        {
            PIMAGE_DOS_HEADER DosHeader = NULL;
            PIMAGE_NT_HEADERS NtHeader = NULL;
            PIMAGE_SECTION_HEADER SectionHeader = NULL;

            DosHeader = (PIMAGE_DOS_HEADER)FileBuffer;
            if (DosHeader->e_magic == IMAGE_DOS_SIGNATURE)
            {
                NtHeader = (PIMAGE_NT_HEADERS)((PUINT8)FileBuffer + DosHeader->e_lfanew);
                if (NtHeader->Signature == IMAGE_NT_SIGNATURE)
                {
                    g_ReloadWin32kImage = ExAllocatePool(NonPagedPool, NtHeader->OptionalHeader.SizeOfImage);
                    if (g_ReloadWin32kImage)
                    {
                        UINT16 i = 0;
                        DbgPrint("New Base::%p\r\n", g_ReloadWin32kImage);

                        // 2.1.��ʼ��������
                        RtlZeroMemory(g_ReloadWin32kImage, NtHeader->OptionalHeader.SizeOfImage);
                        // 2.1.1.����ͷ
                        RtlCopyMemory(g_ReloadWin32kImage, FileBuffer, NtHeader->OptionalHeader.SizeOfHeaders);
                        // 2.1.2.��������
                        SectionHeader = IMAGE_FIRST_SECTION(NtHeader);
                        for (  i = 0; i < NtHeader->FileHeader.NumberOfSections; i++)
                        {
                            RtlCopyMemory(
                                (PUINT8)g_ReloadWin32kImage + SectionHeader[i].VirtualAddress,
                                (PUINT8)FileBuffer + SectionHeader[i].PointerToRawData,
                                SectionHeader[i].SizeOfRawData);
                        }

                        // 2.2.�޸������ַ��
                        APFixImportAddressTable(g_ReloadWin32kImage);

                        // 2.3.�޸��ض����
                        APFixRelocBaseTable(g_ReloadWin32kImage, Win32kLdr->DllBase);

                        // 2.4.�޸�SSDT
                        APFixWin32pServiceTable(g_ReloadWin32kImage, Win32kLdr->DllBase);

                        Status = STATUS_SUCCESS;
                    }
                    else
                    {
                        DbgPrint("ReloadNtkrnl:: Not Valid PE\r\n");
                    }
                }
                else
                {
                    DbgPrint("ReloadNtkrnl:: Not Valid PE\r\n");
                }
            }
            else
            {
                DbgPrint("ReloadNtkrnl:: Not Valid PE\r\n");
            }
            ExFreePool(FileBuffer);
            FileBuffer = NULL;
        }

    }

    return Status;
}


NTSTATUS APEnumSssdtHookByReloadWin32k( PSSSDT_HOOK_INFORMATION shi,  UINT32 SssdtFunctionCount)
{
    NTSTATUS  Status = STATUS_UNSUCCESSFUL;
    // 1.��õ�ǰ��SSSDT
    g_CurrentWin32pServiceTableAddress = (PKSERVICE_TABLE_DESCRIPTOR)APGetCurrentWin32pServiceTable();
    if (g_CurrentWin32pServiceTableAddress && MmIsAddressValid(g_CurrentWin32pServiceTableAddress))
    {
        // 2.Attach��gui����
        PEPROCESS GuiEProcess = APGetCsrssProcess();
        if (GuiEProcess &&MmIsAddressValid(GuiEProcess))
        {
            KAPC_STATE    ApcState = { 0 };

            // ת��Ŀ����̿ռ����±�������
            KeStackAttachProcess(GuiEProcess, &ApcState);

            // 3.�����ں�SSDT(�õ�ԭ�ȵ�SSDT������ַ����)
            Status = APReloadWin32k();
            if (NT_SUCCESS(Status))
            {
                // 3.�Ա�Original&Current
                UINT32 i = 0;
                for ( i = 0; i < g_CurrentWin32pServiceTableAddress->Limit; i++)
                {
                    if (SssdtFunctionCount >= shi->NumberOfSssdtFunctions)
                    {
#ifdef _WIN64
                        // 64λ�洢���� ƫ�ƣ���28λ��
                        //INT32 OriginalOffset = g_SssdtItem[i] >> 4;
                        INT32 CurrentOffset = (*(PINT32)((UINT64)g_CurrentWin32pServiceTableAddress->Base + i * 4)) >> 4;    // ������λ����λ

                        UINT64 CurrentSssdtFunctionAddress = (UINT_PTR)((UINT_PTR)g_CurrentWin32pServiceTableAddress->Base + CurrentOffset);
                        UINT64 OriginalSssdtFunctionAddress = g_OriginalSssdtFunctionAddress[i];

#else
                        // 32λ�洢���� ���Ե�ַ
                        UINT32 CurrentSssdtFunctionAddress = *(UINT32*)((UINT32)g_CurrentWin32pServiceTableAddress->Base + i * 4);
                        UINT32 OriginalSssdtFunctionAddress = g_OriginalSssdtFunctionAddress[i];

#endif // _WIN64

                        if (OriginalSssdtFunctionAddress != CurrentSssdtFunctionAddress)   // ������Hook��
                        {
                            shi->SssdtHookEntry[shi->NumberOfSssdtFunctions].bHooked = TRUE;
                        }
                        else
                        {
                            shi->SssdtHookEntry[shi->NumberOfSssdtFunctions].bHooked = FALSE;
                        }
                        shi->SssdtHookEntry[shi->NumberOfSssdtFunctions].Ordinal = i;
                        shi->SssdtHookEntry[shi->NumberOfSssdtFunctions].CurrentAddress = CurrentSssdtFunctionAddress;
                        shi->SssdtHookEntry[shi->NumberOfSssdtFunctions].OriginalAddress = OriginalSssdtFunctionAddress;

                        RtlStringCchCopyW(shi->SssdtHookEntry[shi->NumberOfSssdtFunctions].wzFunctionName, wcslen(g_SssdtFunctionName[i]) + 1, g_SssdtFunctionName[i]);

                        Status = STATUS_SUCCESS;
                    }
                    else
                    {
                        Status = STATUS_BUFFER_TOO_SMALL;
                    }
                    shi->NumberOfSssdtFunctions++;
                }
            }
            else
            {
                DbgPrint("Reload Win32k & Sssdt Failed\r\n");
            }

            KeUnstackDetachProcess(&ApcState);
        }
    }
    else
    {
        DbgPrint("Get Current Sssdt Failed\r\n");
    }

    return Status;
}

NTSTATUS APEnumSssdtHook( PVOID OutputBuffer,  UINT32 OutputLength)
{
    NTSTATUS  Status = STATUS_UNSUCCESSFUL;

    UINT32    SssdtFunctionCount = (OutputLength - sizeof(SSSDT_HOOK_INFORMATION)) / sizeof(SSSDT_HOOK_ENTRY_INFORMATION);

    PSSSDT_HOOK_INFORMATION shi = (PSSSDT_HOOK_INFORMATION)ExAllocatePool(PagedPool, OutputLength);
    if (shi)
    {
        RtlZeroMemory(shi, OutputLength);

        Status = APEnumSssdtHookByReloadWin32k(shi, SssdtFunctionCount);
        if (NT_SUCCESS(Status))
        {
            if (SssdtFunctionCount >= shi->NumberOfSssdtFunctions)
            {
                RtlCopyMemory(OutputBuffer, shi, OutputLength);
                Status = STATUS_SUCCESS;
            }
            else
            {
                ((PSSSDT_HOOK_INFORMATION)OutputBuffer)->NumberOfSssdtFunctions = shi->NumberOfSssdtFunctions;    // ��Ring3֪����Ҫ���ٸ�
                Status = STATUS_BUFFER_TOO_SMALL;    // ��ring3�����ڴ治������Ϣ
            }
        }

        ExFreePool(shi);
        shi = NULL;
    }

    return Status;
}

NTSTATUS APResumeSssdtHook(IN UINT32 Ordinal)
{
    NTSTATUS       Status = STATUS_UNSUCCESSFUL;

    if (Ordinal == RESUME_ALL_HOOKS)
    {
        // �ָ�����SsdtHook

        // �Ա�Original&Current
        UINT32 i = 0;
        for ( i = 0; i < g_CurrentWin32pServiceTableAddress->Limit; i++)
        {

#ifdef _WIN64
            // 64λ�洢���� ƫ�ƣ���28λ��
            INT32 CurrentOffset = (*(PINT32)((UINT64)g_CurrentWin32pServiceTableAddress->Base + i * 4)) >> 4;    // ������λ����λ

            UINT64 CurrentSsdtFunctionAddress = (UINT_PTR)((UINT_PTR)g_CurrentWin32pServiceTableAddress->Base + CurrentOffset);
            UINT64 OriginalSsdtFunctionAddress = g_OriginalSssdtFunctionAddress[i];

#else
            // 32λ�洢���� ���Ե�ַ
            UINT32 CurrentSsdtFunctionAddress = *(UINT32*)((UINT32)g_CurrentWin32pServiceTableAddress->Base + i * 4);
            UINT32 OriginalSsdtFunctionAddress = g_OriginalSssdtFunctionAddress[i];

#endif // _WIN64

            if (OriginalSsdtFunctionAddress != CurrentSsdtFunctionAddress)   // ������Hook��
            {
                APPageProtectOff();

                *(UINT32*)((UINT_PTR)g_CurrentWin32pServiceTableAddress->Base + i * 4) = *(UINT32*)((UINT_PTR)g_ReloadWin32pServiceTableAddress.Base + i * 4);

                APPageProtectOn();
            }
        }

        Status = STATUS_SUCCESS;
    }
    else if (Ordinal > g_CurrentWin32pServiceTableAddress->Limit)
    {
        Status = STATUS_INVALID_PARAMETER;
    }
    else
    {
        // �ָ�ָ�����SssdtHook
        // ��Ҫ�����ǽ���ǰSssdt�б����ֵ��Ϊg_ReloadWin32pServiceTableAddress.Base�еı����ֵ

        APPageProtectOff();

        *(UINT32*)((UINT_PTR)g_CurrentWin32pServiceTableAddress->Base + Ordinal * 4) = *(UINT32*)((UINT_PTR)g_ReloadWin32pServiceTableAddress.Base + Ordinal * 4);

        APPageProtectOn();

        Status = STATUS_SUCCESS;
    }

    return Status;
}


UINT_PTR APGetSssdtFunctionAddress(IN PCWCHAR wzFunctionName)
{
    UINT32   Ordinal = 0;
    BOOL     bOk = FALSE;
    UINT_PTR FunctionAddress = 0;

    g_CurrentWin32pServiceTableAddress = (PKSERVICE_TABLE_DESCRIPTOR)APGetCurrentWin32pServiceTable();
    if (g_CurrentWin32pServiceTableAddress && MmIsAddressValid(g_CurrentWin32pServiceTableAddress))
    {
        for (Ordinal = 0; Ordinal < g_CurrentWin32pServiceTableAddress->Limit; Ordinal++)
        {
            if (_wcsicmp(g_SssdtFunctionName[Ordinal], wzFunctionName) == 0)
            {
                // �õ��� Ordinal !
                bOk = TRUE;
                break;
            }
        }

        if (bOk)
        {
#ifdef _WIN64
            INT32 CurrentOffset = (*(PINT32)((UINT64)g_CurrentWin32pServiceTableAddress->Base + Ordinal * 4)) >> 4;    // ������λ����λ

            FunctionAddress = (UINT_PTR)((UINT_PTR)g_CurrentWin32pServiceTableAddress->Base + CurrentOffset);
#else
            FunctionAddress = *(UINT32*)((UINT32)g_CurrentWin32pServiceTableAddress->Base + Ordinal * 4);
#endif // !_WIN64
        }
        else
        {
            DbgPrint("Get Sssdt Function Ordinal Failed\r\n");
        }
    }
    else
    {
        DbgPrint("Get Sssdt Address Failed\r\n");
    }
    return FunctionAddress;
}