#include "ProcessHandle.h"
#include "main.h"

extern GOLBAL_INFO g_DriverInfo;


VOID APGetHandleType(IN HANDLE Handle, OUT PWCHAR wzHandleType)
{
    PVOID    pBuffer = NULL;
    UINT32   ReturnLength = 0;
    NTSTATUS Status;
    PPUBLIC_OBJECT_TYPE_INFORMATION ObjectTypeInfo;
    do
    {
        pBuffer = ExAllocatePool(NonPagedPool, PAGE_SIZE);
        if (NULL == pBuffer)
        {
            break;
        }
        // ����֮ǰ��ģʽ��ת��KernelMode
        PETHREAD EThread = PsGetCurrentThread();
        UINT8    PreviousMode = APChangeThreadMode(EThread, KernelMode);
        RtlZeroMemory(pBuffer, PAGE_SIZE);
        Status = ZwQueryObject(Handle, ObjectTypeInformation, pBuffer, PAGE_SIZE, &ReturnLength);
        if (!NT_SUCCESS(Status))
        {
            break;
        }

        ObjectTypeInfo = (PPUBLIC_OBJECT_TYPE_INFORMATION)pBuffer;

        if (ObjectTypeInfo->TypeName.Buffer != NULL &&
            ObjectTypeInfo->TypeName.Length > 0 &&
            MmIsAddressValid(ObjectTypeInfo->TypeName.Buffer))
        {
            if (ObjectTypeInfo->TypeName.Length / sizeof(WCHAR) >= MAX_PATH - 1)
            {
                RtlStringCchCopyW(wzHandleType, MAX_PATH, ObjectTypeInfo->TypeName.Buffer);
            }
            else
            {
                RtlStringCchCopyW(wzHandleType, ObjectTypeInfo->TypeName.Length / sizeof(WCHAR) + 1, ObjectTypeInfo->TypeName.Buffer);
            }
        }
        APChangeThreadMode(EThread, PreviousMode);
    } while (FALSE);
    if (pBuffer != NULL)
    {
        ExFreePool(pBuffer);
        pBuffer = NULL;
    }
}

VOID APGetHandleName(IN HANDLE Handle, OUT PWCHAR wzHandleName)
{
    PVOID    pBuffer = NULL;
    UINT32   ReturnLength = 0;
    NTSTATUS Status;
    UINT8    PreviousMode;
    PETHREAD EThread;
    POBJECT_NAME_INFORMATION ObjectNameInfo = NULL;
    do 
    {
        pBuffer = ExAllocatePool(NonPagedPool, PAGE_SIZE);
        if (NULL==pBuffer)
        {
            break;
        }
        RtlZeroMemory(pBuffer, PAGE_SIZE);
        EThread = PsGetCurrentThread();
        PreviousMode = APChangeThreadMode(EThread, KernelMode);
        Status = ZwQueryObject(Handle, ObjectNameInformation, pBuffer, PAGE_SIZE, &ReturnLength);
        if (!NT_SUCCESS(Status))
        {
            break;
        }

        ObjectNameInfo = (POBJECT_NAME_INFORMATION)pBuffer;
        if (ObjectNameInfo->Name.Buffer != NULL &&
            ObjectNameInfo->Name.Length > 0 &&
            MmIsAddressValid(ObjectNameInfo->Name.Buffer))
        {
            if (ObjectNameInfo->Name.Length / sizeof(WCHAR) >= MAX_PATH - 1)
            {
                RtlStringCchCopyW(wzHandleName, MAX_PATH, ObjectNameInfo->Name.Buffer);
            }
            else
            {
                RtlStringCchCopyW(wzHandleName, ObjectNameInfo->Name.Length / sizeof(WCHAR) + 1, ObjectNameInfo->Name.Buffer);
            }
        }
        APChangeThreadMode(EThread, PreviousMode);

    } while (FALSE);

    if (pBuffer != NULL)
    {
        ExFreePool(pBuffer);
        pBuffer = NULL;
    }
}


VOID APGetProcessHandleInfo(IN PEPROCESS EProcess, IN HANDLE Handle, IN PVOID Object, OUT PPROCESS_HANDLE_INFORMATION phi)
{
    KAPC_STATE    ApcState = { 0 };
    do 
    {
        if (NULL==Object||!MmIsAddressValid(Object))
        {
            break;
        }
        phi->HandleEntry[phi->NumberOfHandles].Handle = Handle;
        phi->HandleEntry[phi->NumberOfHandles].Object = Object;
        if (MmIsAddressValid((PUINT8)Object - g_DriverInfo.DynamicData.SizeOfObjectHeader))
        {
            phi->HandleEntry[phi->NumberOfHandles].ReferenceCount = (UINT32)((POBJECT_HEADER)((PUINT8)Object - g_DriverInfo.DynamicData.SizeOfObjectHeader))->PointerCount;
        }
        else
        {
            phi->HandleEntry[phi->NumberOfHandles].ReferenceCount = 0;
        }

        // ת��Ŀ����̿ռ����±�������
        KeStackAttachProcess(EProcess, &ApcState);

        APGetHandleName(Handle, phi->HandleEntry[phi->NumberOfHandles].wzHandleName);
        APGetHandleType(Handle, phi->HandleEntry[phi->NumberOfHandles].wzHandleType);

        KeUnstackDetachProcess(&ApcState);

    } while (FALSE);
}


NTSTATUS APEnumProcessHandleByZwQuerySystemInformation(IN UINT32 ProcessId, IN PEPROCESS EProcess, OUT PPROCESS_HANDLE_INFORMATION phi, IN UINT32 HandleCount)
{
    NTSTATUS Status = STATUS_UNSUCCESSFUL;
    UINT32   ReturnLength = PAGE_SIZE;

    // ����֮ǰ��ģʽ��ת��KernelMode
    PETHREAD EThread = PsGetCurrentThread();
    UINT8    PreviousMode = APChangeThreadMode(EThread, KernelMode);
    PVOID pBuffer = NULL;

    do
    {
        pBuffer = ExAllocatePool(NonPagedPool, ReturnLength);
        if (NULL == pBuffer)
        {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }
        do
        {
            RtlZeroMemory(pBuffer, ReturnLength);
            // ɨ��ϵͳ���н��̵ľ����Ϣ
            Status = ZwQuerySystemInformation(SystemHandleInformation, pBuffer, ReturnLength, &ReturnLength);
            if (NT_SUCCESS(Status))
            {
                PSYSTEM_HANDLE_INFORMATION shi = (PSYSTEM_HANDLE_INFORMATION)pBuffer;
                for (UINT32 i = 0; i < shi->NumberOfHandles; i++)
                {
                    if (ProcessId == (UINT32)shi->Handles[i].UniqueProcessId)
                    {
                        if (HandleCount > phi->NumberOfHandles)
                        {
                            APGetProcessHandleInfo(EProcess, (HANDLE)shi->Handles[i].HandleValue, (PVOID)shi->Handles[i].Object, phi);
                        }
                        // ��¼�������
                        phi->NumberOfHandles++;
                    }
                }
            }
        } while (Status == STATUS_INFO_LENGTH_MISMATCH);

    } while (FALSE);

    APChangeThreadMode(EThread, PreviousMode);

    if (pBuffer!=NULL)
    {
        ExFreePool(pBuffer);
        pBuffer = NULL;
    }
    // ö�ٵ��˶���
    if (phi->NumberOfHandles)
    {
        Status = STATUS_SUCCESS;
    }

    return Status;
}



NTSTATUS APEnumProcessHandle(IN UINT32 ProcessId, OUT PVOID OutputBuffer, IN UINT32 OutputLength)
{
    NTSTATUS  Status = STATUS_UNSUCCESSFUL;
    UINT32    HandleCount = (OutputLength - sizeof(PROCESS_HANDLE_INFORMATION)) / sizeof(PROCESS_HANDLE_ENTRY_INFORMATION);
    PEPROCESS EProcess = NULL;
    PPROCESS_HANDLE_INFORMATION phi = NULL;
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
        // ��Ϊ֮����ҪAttach��Ŀ����̿ռ䣨˽���ڴ棩��������Ҫ�����ں˿ռ���ڴ棨���õ��ڴ棩
        phi = (PPROCESS_HANDLE_INFORMATION)ExAllocatePool(NonPagedPool, OutputLength);
        if (NULL == phi)
        {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }
        RtlZeroMemory(phi, OutputLength);
        Status = APEnumProcessHandleByZwQuerySystemInformation(ProcessId, EProcess, phi, HandleCount);
        if (!NT_SUCCESS(Status))
        {
            break;
        }
        if (HandleCount >= phi->NumberOfHandles)
        {
            RtlCopyMemory(OutputBuffer, phi, OutputLength);
            Status = STATUS_SUCCESS;
        }
        else
        {
            ((PPROCESS_HANDLE_INFORMATION)OutputBuffer)->NumberOfHandles = phi->NumberOfHandles;    // ��Ring3֪����Ҫ���ٸ�
            Status = STATUS_BUFFER_TOO_SMALL;    // ��ring3�����ڴ治������Ϣ
        }

    } while (FALSE);


    if (EProcess != NULL)
    {
        ObDereferenceObject(EProcess);
        EProcess = NULL;
    }
    if (phi != NULL)
    {
        ExFreePool(phi);
        phi = NULL;
    }
    return Status;
}

