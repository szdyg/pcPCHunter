#pragma once
#include <ntifs.h>
#include "Private.h"
#include "NtStructs.h"


typedef struct _IO_TIMER_ENTRY_INFORMATION
{
    UINT_PTR TimerObject;
    UINT_PTR DeviceObject;
    UINT_PTR TimeDispatch;
    UINT_PTR TimerEntry;        // 与ListCtrl的Item关联，便于判断
    UINT32   Status;
} IO_TIMER_ENTRY_INFORMATION, *PIO_TIMER_ENTRY_INFORMATION;

typedef struct _IO_TIMER_INFORMATION
{
    UINT_PTR                   NumberOfIoTimers;
    IO_TIMER_ENTRY_INFORMATION IoTimerEntry[1];
} IO_TIMER_INFORMATION, *PIO_TIMER_INFORMATION;



UINT_PTR APGetIopTimerQueueHead();

//************************************
// 函数名:   APEnumIoTimerByIterateIopTimerQueueHead
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    OUT PIO_TIMER_INFORMATION iti              缓存地址
// 参数：    IN UINT32 IoTimerCount                     可以缓存几个对象
// 说明：    遍历IopTimerQueueHead枚举IoTimer对象信息
//************************************
NTSTATUS APEnumIoTimerByIterateIopTimerQueueHead(OUT PIO_TIMER_INFORMATION iti, IN UINT32 IoTimerCount);

//************************************
// 函数名:   APEnumIoTimer
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    OUT PVOID OutputBuffer              缓存地址
// 参数：    IN UINT32 OutputLength              缓存长度
// 说明：    枚举IoTimer对象
//************************************
NTSTATUS APEnumIoTimer(OUT PVOID OutputBuffer, IN UINT32 OutputLength);


