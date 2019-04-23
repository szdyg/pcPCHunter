#include "Dispatch.h"



NTSTATUS APIoControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    NTSTATUS            Status = STATUS_SUCCESS;
    PVOID                InputBuffer = NULL;
    PVOID               OutputBuffer = NULL;
    UINT32                InputLength = 0;
    UINT32                OutputLength = 0;
    PIO_STACK_LOCATION    IrpStack;
    UINT32                IoControlCode;

    IrpStack = IoGetCurrentIrpStackLocation(Irp);        // ��õ�ǰIrp��ջ
    InputBuffer = IrpStack->Parameters.DeviceIoControl.Type3InputBuffer;
    OutputBuffer = Irp->UserBuffer;
    InputLength = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
    OutputLength = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    KdBreakPoint();


    IoControlCode = IrpStack->Parameters.DeviceIoControl.IoControlCode;
    switch (IoControlCode)
    {
        //
        // ProcessCore
        //
        case IOCTL_ARKPROTECT_PROCESSNUM:
        {
            if (OutputBuffer)
            {
                __try
                {
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT32));
                    Status = APGetProcessNum(OutputBuffer);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            }
        }
        break;
        case IOCTL_ARKPROTECT_ENUMPROCESS:
        {
            if (OutputBuffer)
            {
                __try
                {
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT8));
                    Status = APEnumProcessInfo(OutputBuffer, OutputLength);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            }
        }
        break;
        case IOCTL_ARKPROTECT_ENUMPROCESSMODULE:
        {
            if (InputLength >= sizeof(UINT32) && InputBuffer && OutputBuffer)
            {
                __try
                {
                    ProbeForRead(InputBuffer, InputLength, sizeof(UINT32));
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT8));
                    Status = APEnumProcessModule(*(PUINT32)InputBuffer, OutputBuffer, OutputLength);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
            }
        }
        break;
        case IOCTL_ARKPROTECT_ENUMPROCESSTHREAD:
        {
            if (InputLength >= sizeof(UINT32) && InputBuffer && OutputBuffer)
            {
                __try
                {
                    ProbeForRead(InputBuffer, InputLength, sizeof(UINT32));
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT8));
                    Status = APEnumProcessThread(*(PUINT32)InputBuffer, OutputBuffer, OutputLength);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
            }
        }
        break;
        case IOCTL_ARKPROTECT_ENUMPROCESSHANDLE:
        {
            if (InputLength >= sizeof(UINT32) && InputBuffer && OutputBuffer)
            {
                __try
                {
                    ProbeForRead(InputBuffer, InputLength, sizeof(UINT32));
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT8));
                    Status = APEnumProcessHandle(*(PUINT32)InputBuffer, OutputBuffer, OutputLength);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
            }
        }
        break;
        case IOCTL_ARKPROTECT_ENUMPROCESSWINDOW:
        {
            if (InputLength >= sizeof(UINT32) && InputBuffer && OutputBuffer)
            {
                __try
                {
                    ProbeForRead(InputBuffer, InputLength, sizeof(UINT32));
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT8));
                    Status = APEnumProcessWindow(*(PUINT32)InputBuffer, OutputBuffer, OutputLength);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
            }
        }
        break;
        case IOCTL_ARKPROTECT_ENUMPROCESSMEMORY:
        {
            if (InputLength >= sizeof(UINT32) && InputBuffer && OutputBuffer)
            {
                __try
                {
                    ProbeForRead(InputBuffer, InputLength, sizeof(UINT32));
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT8));
                    Status = APEnumProcessMemory(*(PUINT32)InputBuffer, OutputBuffer, OutputLength);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
            }
        }
        break;
        case IOCTL_ARKPROTECT_TERMINATEPROCESS:
        {
            if (InputLength >= sizeof(UINT32) && InputBuffer)
            {
                __try
                {
                    ProbeForRead(InputBuffer, InputLength, sizeof(UINT32));
                    Status = APTerminateProcess(*(PUINT32)InputBuffer);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
            }
        }
        break;

        // 
        // Driver
        // 

        case IOCTL_ARKPROTECT_ENUMDRIVER:
        {
            if (OutputBuffer)
            {
                __try
                {
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT8));
                    Status = APEnumDriverInfo(OutputBuffer, OutputLength);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            }
        }
        break;
        case IOCTL_ARKPROTECT_UNLOADRIVER:
        {
            if (InputLength >= sizeof(UINT_PTR) && InputBuffer)
            {
                __try
                {
                    ProbeForRead(InputBuffer, InputLength, sizeof(UINT_PTR));
                    Status = APUnloadDriverObject(*(PUINT_PTR)InputBuffer);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
            }
        }
        break;

        //
        // KernelCore
        //
        case IOCTL_ARKPROTECT_ENUMSYSCALLBACK:
        {
            if (OutputBuffer)
            {
                __try
                {
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT8));
                    Status = APEnumSystemCallback(OutputBuffer, OutputLength);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            }
        }
        break;
        case IOCTL_ARKPROTECT_ENUMFILTERDRIVER:
        {
            if (OutputBuffer)
            {
                __try
                {
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT8));
                    Status = APEnumFilterDriver(OutputBuffer, OutputLength);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            }
        }
        break;
        case IOCTL_ARKPROTECT_ENUMDPCTIMER:
        {
            if (OutputBuffer)
            {
                __try
                {
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT8));

                    Status = APEnumDpcTimer(OutputBuffer, OutputLength);

                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            }
        }
        break;
        case IOCTL_ARKPROTECT_ENUMIOTIMER:
        {
            if (OutputBuffer)
            {
                __try
                {
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT8));
                    Status = APEnumIoTimer(OutputBuffer, OutputLength);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            }
        }
        break;
        case IOCTL_ARKPROTECT_ENUMSSDTHOOK:
        {
            if (OutputBuffer)
            {
                __try
                {
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT8));

                    Status = APEnumSsdtHook(OutputBuffer, OutputLength);

                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            }
        }
        break;
        case IOCTL_ARKPROTECT_RESUMESSDTHOOK:
        {
            if (InputLength >= sizeof(UINT32) && InputBuffer)
            {
                __try
                {
                    ProbeForRead(InputBuffer, InputLength, sizeof(UINT32));
                    Status = APResumeSsdtHook(*(PUINT32)InputBuffer);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
            }
        }
        break;
        case IOCTL_ARKPROTECT_ENUMSSSDTHOOK:
        {
            if (OutputBuffer)
            {
                __try
                {
                    ProbeForWrite(OutputBuffer, OutputLength, sizeof(UINT8));
                    Status = APEnumSssdtHook(OutputBuffer, OutputLength);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            }
        }
        break;
        case IOCTL_ARKPROTECT_RESUMESSSDTHOOK:
        {
            if (InputLength >= sizeof(UINT32) && InputBuffer)
            {
                __try
                {
                    ProbeForRead(InputBuffer, InputLength, sizeof(UINT32));

                    Status = APResumeSssdtHook(*(PUINT32)InputBuffer);

                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
            }
        }
        break;

        // 
        // File
        // 

        case IOCTL_ARKPROTECT_DELETEFILE:
        {
            if (InputLength >= sizeof(UINT32) && InputBuffer)
            {
                __try
                {
                    ProbeForRead(InputBuffer, InputLength, sizeof(UINT8));
                    Status = APDeleteFile(InputBuffer);
                    Irp->IoStatus.Status = Status;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    DbgPrint("Catch Exception\r\n");
                    Status = STATUS_UNSUCCESSFUL;
                }
            }
            else
            {
                Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
            }
        }
        break;

        default:
            Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            break;
    }


    Status = Irp->IoStatus.Status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return Status;
}


