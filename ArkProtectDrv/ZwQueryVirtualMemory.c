#pragma once
#include "ZwQueryVirtualMemory.h"
#include "Common.h"
#include "NtStructs.h"
#include "GetSSDTFuncAddress.h"

extern COMMON_INFO g_CommonInfo;




// NTSTATUS EnumMoudleByNtQueryVirtualMemory(ULONG ProcessId)
// {
//     NTSTATUS Status;
//     PEPROCESS  Process = NULL;
//     HANDLE    hProcess = NULL;
//     SIZE_T ulRet = 0;
//     WCHAR DosPath[260] = { 0 };
// 
//     if (ProcessId)
//     {
//         Status = PsLookupProcessByProcessId((HANDLE)ProcessId, &Process);
//         if (!NT_SUCCESS(Status))
//         {
//             return Status;
//         }
//     }
//     if (IsRealProcess(Process))   //判断是否为僵尸进程，我只是判断了对象类型和句柄表是否存在
//     {
//         ObfDereferenceObject(Process);
//         Status = ObOpenObjectByPointer(
//             Process,
//             OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
//             NULL,
//             GENERIC_ALL,
//             *PsProcessType,
//             KernelMode,
//             &hProcess);
//         if (NT_SUCCESS(Status))
//         {
//             ULONG_PTR ulBase = 0;
//             //改变PreviousMode
//             PETHREAD EThread = PsGetCurrentThread();
//             CHAR PreMode = ChangePreMode(EThread);   //KernelMode
//             do
//             {
//                 MEMORY_BASIC_INFORMATION mbi = { 0 };
//                 Status = my_NtQueryVirtualMemoryAddress(hProcess,
//                     (PVOID)ulBase,
//                     MemoryBasicInformation,
//                     &mbi,
//                     sizeof(MEMORY_BASIC_INFORMATION),
//                     &ulRet);
//                 if (NT_SUCCESS(Status))
//                 {
//                     //如果是Image 再查询SectionName,即FileObject Name
//                     if (mbi.Type == MEM_IMAGE)
//                     {
//                         MEMORY_SECTION_NAME msn = { 0 };
//                         Status = NtQueryVirtualMemoryAddress(hProcess,
//                             (PVOID)ulBase,
//                             MemorySectionName,
//                             &msn,
//                             sizeof(MEMORY_SECTION_NAME),
//                             &ulRet);
//                         if (NT_SUCCESS(Status))
//                         {
//                             DbgPrint("SectionName:%wZ\r\n", &(msn.NameBuffer));
//                             NtPathToDosPathW(msn.NameBuffer, DosPath);
//                             DbgPrint("DosName:%S\r\n", DosPath);
//                         }
//                     }
//                     ulBase += mbi.RegionSize;
//                 }
//                 else ulBase += PAGE_SIZE;
//             } while (ulBase < (ULONG_PTR)HighUserAddress);
//             NtClose(hProcess);
//             RecoverPreMode(EThread, PreMode);
//         }
//     }
//     return Status;
// }


