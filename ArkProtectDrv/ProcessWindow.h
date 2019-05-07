#pragma once
#include <ntifs.h>
#include "Private.h"
#include "ProcessCore.h"
#include "Sssdt.h"

typedef struct _PROCESS_WINDOW_ENTRY_INFORMATION
{
    HWND   hWnd;
    WCHAR  wzWindowText;
    WCHAR  wzWindowClass;
    BOOL   bVisibal;
    UINT32 ProcessId;
    UINT32 ThreadId;
} PROCESS_WINDOW_ENTRY_INFORMATION, *PPROCESS_WINDOW_ENTRY_INFORMATION;

typedef struct _PROCESS_WINDOW_INFORMATION
{
    UINT32                            NumberOfWindows;
    PROCESS_WINDOW_ENTRY_INFORMATION  WindowEntry[1];
} PROCESS_WINDOW_INFORMATION, *PPROCESS_WINDOW_INFORMATION;



//************************************
// 函数名:   APEnumProcessWindowByNtUserBuildHwndList
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN UINT32 ProcessId
// 参数：    IN PEPROCESS EProcess
// 参数：    OUT PPROCESS_WINDOW_INFORMATION pwi
// 参数：    IN UINT32 WindowCount
// 说明：    通过NtUserBuildHwndList枚举进程窗口
//************************************
NTSTATUS APEnumProcessWindowByNtUserBuildHwndList(IN UINT32 ProcessId, IN PEPROCESS EProcess, OUT PPROCESS_WINDOW_INFORMATION pwi, IN UINT32 WindowCount);

//************************************
// 函数名:   APEnumProcessWindow
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN UINT32 ProcessId             进程ID
// 参数：    OUT PVOID OutputBuffer          R3内存
// 参数：    IN UINT32 OutputLength          内存长度
// 说明：    
//************************************
NTSTATUS APEnumProcessWindow(IN UINT32 ProcessId, OUT PVOID OutputBuffer, IN UINT32 OutputLength);



