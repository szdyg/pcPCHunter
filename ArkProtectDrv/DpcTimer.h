#pragma once
#include <ntifs.h>
#include "Private.h"
#include "NtStructs.h"

typedef struct _DPC_TIMER_ENTRY_INFORMATION
{
    UINT_PTR TimerObject;
    UINT_PTR RealDpc;
    UINT_PTR Cycle;       // ����
    UINT_PTR TimeDispatch;
} DPC_TIMER_ENTRY_INFORMATION, *PDPC_TIMER_ENTRY_INFORMATION;

typedef struct _DPC_TIMER_INFORMATION
{
    UINT32                      NumberOfDpcTimers;
    DPC_TIMER_ENTRY_INFORMATION DpcTimerEntry[1];
} DPC_TIMER_INFORMATION, *PDPC_TIMER_INFORMATION;


//************************************
// ������:   FindKiWaitVariableAddress
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    OUT PUINT_PTR * KiWaitNeverAddress
// ������    OUT PUINT_PTR * KiWaitAlwaysAddress
// ˵����    ͨ�� KeSetTimer ��Ӳ���� ��� δ�������� KiWaitNeverAddress & KiWaitAlwaysAddress
//************************************
BOOLEAN FindKiWaitVariableAddress(OUT PUINT_PTR* KiWaitNeverAddress, OUT PUINT_PTR* KiWaitAlwaysAddress);

//************************************
// ������:   APTransTimerDPCEx
// Ȩ�ޣ�    public 
// ����ֵ:   PKDPC
// ������    IN PKTIMER Timer
// ������    IN UINT64 KiWaitNeverAddress
// ������    IN UINT64 KiWaitAlwaysAddress
// ˵����    ת��Timer�������DPCΪ��ʵ��DPC(���ǿ��ö�����ʽ)
//************************************
PKDPC APTransTimerDPCEx(IN PKTIMER Timer, IN UINT64 KiWaitNeverAddress, IN UINT64 KiWaitAlwaysAddress);

//************************************
// ������:   APEnumDpcTimerByIterateKTimerTableEntry
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    OUT PDPC_TIMER_INFORMATION dti
// ������    IN UINT32 DpcTimerCount
// ˵����    ͨ������KTIMER_TABLE_ENTRY��������ListEntryö��DpcTimer������Ϣ
//************************************
NTSTATUS APEnumDpcTimerByIterateKTimerTableEntry(OUT PDPC_TIMER_INFORMATION dti, IN UINT32 DpcTimerCount);



//************************************
// ������:   APEnumDpcTimer
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    OUT PVOID OutputBuffer
// ������    IN UINT32 OutputLength
// ˵����    ö��DpcTimer����
//************************************
NTSTATUS APEnumDpcTimer(OUT PVOID OutputBuffer, IN UINT32 OutputLength);



