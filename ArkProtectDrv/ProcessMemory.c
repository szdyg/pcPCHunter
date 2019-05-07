#include "ProcessMemory.h"
#include "GetSSDTFuncAddress.h"
#include "main.h"
#include "ZwQueryVirtualMemory.h"
#include <ntifs.h>

extern GOLBAL_INFO g_DriverInfo;
extern pfnNtQueryVirtualMemory   g_NtQueryVirtualMemoryAddress;


NTSTATUS APEnumProcessMemoryByZwQueryVirtualMemory(IN PEPROCESS EProcess, OUT PPROCESS_MEMORY_INFORMATION pmi, IN UINT32 MemoryCount)
{
    NTSTATUS Status = STATUS_UNSUCCESSFUL;
    HANDLE   ProcessHandle = NULL;
    UINT_PTR BaseAddress = 0;

    do
    {
        Status = ObOpenObjectByPointer(
            EProcess,
            OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
            NULL,
            GENERIC_ALL,
            *PsProcessType,
            KernelMode,
            &ProcessHandle);

        if (!NT_SUCCESS(Status))
        {
            break;
        }
        // 处理Wow64程序
        if (PsGetProcessWow64Process(EProcess))
        {
            g_DriverInfo.DynamicData.MaxUserSpaceAddress = 0x7FFFFFFF;
        }

        while (BaseAddress < g_DriverInfo.DynamicData.MaxUserSpaceAddress)
        {
            MEMORY_BASIC_INFORMATION  mbi = { 0 };
            SIZE_T  ReturnLength = 0;
            g_NtQueryVirtualMemoryAddress = (pfnNtQueryVirtualMemory)GetFuncAddress("NtQueryVirtualMemory");
            if (g_NtQueryVirtualMemoryAddress == NULL)
            {
                return Status;
            }
            Status = g_NtQueryVirtualMemoryAddress(ProcessHandle, (PVOID)BaseAddress, MemoryBasicInformation,
                &mbi, sizeof(MEMORY_BASIC_INFORMATION), &ReturnLength);
            if (NT_SUCCESS(Status))
            {
                if (MemoryCount > pmi->NumberOfMemories)
                {
                    pmi->MemoryEntry[pmi->NumberOfMemories].BaseAddress = BaseAddress;
                    pmi->MemoryEntry[pmi->NumberOfMemories].RegionSize = mbi.RegionSize;
                    pmi->MemoryEntry[pmi->NumberOfMemories].Protect = mbi.Protect;
                    pmi->MemoryEntry[pmi->NumberOfMemories].State = mbi.State;
                    pmi->MemoryEntry[pmi->NumberOfMemories].Type = mbi.Type;
                }

                pmi->NumberOfMemories++;
                BaseAddress += mbi.RegionSize;
            }
            else
            {
                BaseAddress += PAGE_SIZE;
            }
            Status = STATUS_SUCCESS;
        }

        if (PsGetProcessWow64Process(EProcess))
        {
            g_DriverInfo.DynamicData.MaxUserSpaceAddress = 0x000007FFFFFFFFFF;
        }

    } while (FALSE);


    if (ProcessHandle != NULL)
    {
        ZwClose(ProcessHandle);
        ProcessHandle = NULL;
    }
    return Status;
}



NTSTATUS APEnumProcessMemory(IN UINT32 ProcessId, OUT PVOID OutputBuffer, IN UINT32 OutputLength)
{
    NTSTATUS  Status = STATUS_UNSUCCESSFUL;
    PPROCESS_MEMORY_INFORMATION pmi = (PPROCESS_MEMORY_INFORMATION)OutputBuffer;
    UINT32    ModuleCount = (OutputLength - sizeof(PROCESS_MEMORY_INFORMATION)) / sizeof(PROCESS_MEMORY_ENTRY_INFORMATION);
    PEPROCESS EProcess = NULL;

    do
    {
        if (0 == ProcessId)
        {
            Status = STATUS_INVALID_PARAMETER;
            break;
        }
        Status = PsLookupProcessByProcessId((HANDLE)ProcessId, &EProcess);
        if (!NT_SUCCESS(Status) || APIsValidProcess(EProcess))
        {
            break;
        }
        Status = APEnumProcessMemoryByZwQueryVirtualMemory(EProcess, pmi, ModuleCount);
        if (!NT_SUCCESS(Status))
        {
            break;
        }
        if (ModuleCount >= pmi->NumberOfMemories)
        {
            Status = STATUS_SUCCESS;
        }
        else
        {
            Status = STATUS_BUFFER_TOO_SMALL;    // 给ring3返回内存不够的信息
        }
    } while (FALSE);

    if (EProcess != NULL)
    {
        ObDereferenceObject(EProcess);
        EProcess = NULL;
    }

    return Status;
}