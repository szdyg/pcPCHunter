#include "ProcessWindow.h"

typedef NTSTATUS(*pfnNtUserBuildHwndList)(
     HDESK hdesk,
     HWND hwndNext,
     BOOL fEnumChildren,
     DWORD idThread,
     UINT cHwndMax,
     HWND *phwndFirst,
     PUINT pcHwndNeeded);

typedef HANDLE(*pfnNtUserQueryWindow)(HWND hwnd, DWORD WindowInfo);


NTSTATUS APEnumProcessWindowByNtUserBuildHwndList( UINT32 ProcessId,  PEPROCESS EProcess, OUT PPROCESS_WINDOW_INFORMATION pwi, IN UINT32 WindowCount)
{
    NTSTATUS Status = STATUS_UNSUCCESSFUL;
    HWND* Wnds = NULL;
    UINT32 NumberOfWindows = 0;
    UINT32 ThreadId = 0;
    UINT32 WndProcessId = 0;
    // 获得 NtUserBuildHwndList 地址
    pfnNtUserBuildHwndList NtUserBuildHwndList = (pfnNtUserBuildHwndList)APGetSssdtFunctionAddress(L"NtUserBuildHwndList");
    pfnNtUserQueryWindow   NtUserQueryWindow = (pfnNtUserQueryWindow)APGetSssdtFunctionAddress(L"NtUserQueryWindow");
    do 
    {
        if (NULL == NtUserBuildHwndList ||
            NULL == NtUserQueryWindow ||
            !MmIsAddressValid((PVOID)NtUserBuildHwndList) ||
            !MmIsAddressValid((PVOID)NtUserQueryWindow))
        {
            DbgPrint("Get NtUserBuildHwndList Failed\r\n");
            break;
        }
        Status = NtUserBuildHwndList(NULL, NULL, FALSE, 0, WindowCount, (HWND*)(pwi->WindowEntry), &pwi->NumberOfWindows);
        if (!NT_SUCCESS(Status))
        {
            break;
        }

        Wnds = (HWND*)ExAllocatePool(NonPagedPool, sizeof(HWND) * pwi->NumberOfWindows);
        if (NULL == Wnds)
        {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }

        RtlCopyMemory(Wnds, &pwi->WindowEntry, sizeof(HWND) * pwi->NumberOfWindows);
        RtlZeroMemory(&pwi->WindowEntry, sizeof(HWND) * pwi->NumberOfWindows);
        for (UINT32 i = 0; i < pwi->NumberOfWindows; i++)
        {
            WndProcessId = (UINT32)(UINT_PTR)NtUserQueryWindow(Wnds[i], 0);
            ThreadId = (UINT32)(UINT_PTR)NtUserQueryWindow(Wnds[i], 2);
            if (WndProcessId == ProcessId)
            {
                if (WindowCount >= NumberOfWindows)
                {
                    pwi->WindowEntry[NumberOfWindows].hWnd = Wnds[i];
                    pwi->WindowEntry[NumberOfWindows].ProcessId = ProcessId;
                    pwi->WindowEntry[NumberOfWindows].ThreadId = ThreadId;
                }
                NumberOfWindows++;
            }
        }
        pwi->NumberOfWindows = NumberOfWindows;
    } while (FALSE);
    if (Wnds != NULL)
    {
        ExFreePool(Wnds);
        Wnds = NULL;
    }
    return Status;
}



NTSTATUS APEnumProcessWindow(UINT32 ProcessId, PVOID OutputBuffer, UINT32 OutputLength)
{
    NTSTATUS  Status = STATUS_UNSUCCESSFUL;
    UINT32    WindowCount = (OutputLength - sizeof(PROCESS_WINDOW_INFORMATION)) / sizeof(PROCESS_WINDOW_ENTRY_INFORMATION);
    PEPROCESS EProcess = NULL;
    PPROCESS_WINDOW_INFORMATION pwi = (PPROCESS_WINDOW_INFORMATION)OutputBuffer;

    do
    {
        if (0 == ProcessId || NULL == pwi)
        {
            Status = STATUS_INVALID_PARAMETER;
            break;
        }
        Status = PsLookupProcessByProcessId((HANDLE)ProcessId, &EProcess);
        if (!NT_SUCCESS(Status) || !APIsValidProcess(EProcess))
        {
            break;
        }
        Status = APEnumProcessWindowByNtUserBuildHwndList(ProcessId, EProcess, pwi, WindowCount);
        if (!NT_SUCCESS(Status))
        {
            break;
        }
        if (WindowCount >= pwi->NumberOfWindows)
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
    }
    return Status;
}
