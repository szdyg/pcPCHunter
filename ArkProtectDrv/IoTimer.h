#pragma once
#include <ntifs.h>
#include "Private.h"
#include "NtStructs.h"


typedef struct _IO_TIMER_ENTRY_INFORMATION
{
    UINT_PTR TimerObject;
    UINT_PTR DeviceObject;
    UINT_PTR TimeDispatch;
    UINT_PTR TimerEntry;        // ��ListCtrl��Item�����������ж�
    UINT32   Status;
} IO_TIMER_ENTRY_INFORMATION, *PIO_TIMER_ENTRY_INFORMATION;

typedef struct _IO_TIMER_INFORMATION
{
    UINT_PTR                   NumberOfIoTimers;
    IO_TIMER_ENTRY_INFORMATION IoTimerEntry[1];
} IO_TIMER_INFORMATION, *PIO_TIMER_INFORMATION;



UINT_PTR APGetIopTimerQueueHead();

//************************************
// ������:   APEnumIoTimerByIterateIopTimerQueueHead
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    OUT PIO_TIMER_INFORMATION iti              �����ַ
// ������    IN UINT32 IoTimerCount                     ���Ի��漸������
// ˵����    ����IopTimerQueueHeadö��IoTimer������Ϣ
//************************************
NTSTATUS APEnumIoTimerByIterateIopTimerQueueHead(OUT PIO_TIMER_INFORMATION iti, IN UINT32 IoTimerCount);

//************************************
// ������:   APEnumIoTimer
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    OUT PVOID OutputBuffer              �����ַ
// ������    IN UINT32 OutputLength              ���泤��
// ˵����    ö��IoTimer����
//************************************
NTSTATUS APEnumIoTimer(OUT PVOID OutputBuffer, IN UINT32 OutputLength);


