#pragma once
#include <ntifs.h>
#include "Private.h"
#include "NtStructs.h"

typedef struct _DPC_TIMER_ENTRY_INFORMATION
{
    UINT_PTR TimerObject;
    UINT_PTR RealDpc;
    UINT_PTR Cycle;       // 周期
    UINT_PTR TimeDispatch;
} DPC_TIMER_ENTRY_INFORMATION, *PDPC_TIMER_ENTRY_INFORMATION;

typedef struct _DPC_TIMER_INFORMATION
{
    UINT32                      NumberOfDpcTimers;
    DPC_TIMER_ENTRY_INFORMATION DpcTimerEntry[1];
} DPC_TIMER_INFORMATION, *PDPC_TIMER_INFORMATION;


//************************************
// 函数名:   FindKiWaitVariableAddress
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    OUT PUINT_PTR * KiWaitNeverAddress
// 参数：    OUT PUINT_PTR * KiWaitAlwaysAddress
// 说明：    通过 KeSetTimer 和硬编码 获得 未导出变量 KiWaitNeverAddress & KiWaitAlwaysAddress
//************************************
BOOLEAN FindKiWaitVariableAddress(OUT PUINT_PTR* KiWaitNeverAddress, OUT PUINT_PTR* KiWaitAlwaysAddress);

//************************************
// 函数名:   APTransTimerDPCEx
// 权限：    public 
// 返回值:   PKDPC
// 参数：    IN PKTIMER Timer
// 参数：    IN UINT64 KiWaitNeverAddress
// 参数：    IN UINT64 KiWaitAlwaysAddress
// 说明：    转化Timer对象里的DPC为真实的DPC(我们看得懂的形式)
//************************************
PKDPC APTransTimerDPCEx(IN PKTIMER Timer, IN UINT64 KiWaitNeverAddress, IN UINT64 KiWaitAlwaysAddress);

//************************************
// 函数名:   APEnumDpcTimerByIterateKTimerTableEntry
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    OUT PDPC_TIMER_INFORMATION dti
// 参数：    IN UINT32 DpcTimerCount
// 说明：    通过遍历KTIMER_TABLE_ENTRY数组里面ListEntry枚举DpcTimer对象信息
//************************************
NTSTATUS APEnumDpcTimerByIterateKTimerTableEntry(OUT PDPC_TIMER_INFORMATION dti, IN UINT32 DpcTimerCount);



//************************************
// 函数名:   APEnumDpcTimer
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    OUT PVOID OutputBuffer
// 参数：    IN UINT32 OutputLength
// 说明：    枚举DpcTimer对象
//************************************
NTSTATUS APEnumDpcTimer(OUT PVOID OutputBuffer, IN UINT32 OutputLength);



