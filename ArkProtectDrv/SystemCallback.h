#ifndef CXX_SystemCallback_H
#define CXX_SystemCallback_H

#include <ntifs.h>
#include "Private.h"

#ifdef _WIN64

#define PSP_MAX_CREATE_PROCESS_NOTIFY 64
#define PSP_MAX_CREATE_THREAD_NOTIFY  64
#define PSP_MAX_LOAD_IMAGE_NOTIFY     64

#else

#define PSP_MAX_CREATE_PROCESS_NOTIFY  8
#define PSP_MAX_CREATE_THREAD_NOTIFY   8
#define PSP_MAX_LOAD_IMAGE_NOTIFY      8

#endif // _WIN64



typedef enum _eCallbackType
{
    ct_NotifyCreateProcess,
    ct_NotifyCreateThread,
    ct_NotifyLoadImage,
    ct_NotifyCmpCallBack,
    ct_NotifyKeBugCheckReason,
    ct_NotifyKeBugCheck,
    ct_NotifyShutdown,
    ct_NotifyLastChanceShutdown
} eCallbackType;

typedef struct _SYS_CALLBACK_ENTRY_INFORMATION
{
    eCallbackType Type;
    UINT_PTR      CallbackAddress;
    UINT_PTR      Description;
} SYS_CALLBACK_ENTRY_INFORMATION, *PSYS_CALLBACK_ENTRY_INFORMATION;

typedef struct _SYS_CALLBACK_INFORMATION
{
    UINT_PTR                       NumberOfCallbacks;
    SYS_CALLBACK_ENTRY_INFORMATION CallbackEntry[1];
} SYS_CALLBACK_INFORMATION, *PSYS_CALLBACK_INFORMATION;


typedef struct _CM_NOTIFY_ENTRY
{
    LIST_ENTRY        ListEntryHead;
    UINT32            UnKnown1;
    UINT32            UnKnown2;
    LARGE_INTEGER    Cookie;
    UINT64            Context;
    UINT64            Function;
} CM_NOTIFY_ENTRY, *PCM_NOTIFY_ENTRY;


//************************************
// ������:   APGetPspCreateProcessNotifyRoutineAddress
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ˵����    PspCreateProcessNotifyRoutine
//************************************
UINT_PTR APGetPspCreateProcessNotifyRoutineAddress();

//************************************
// ������:   APGetCreateProcessCallbackNotify
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    OUT PSYS_CALLBACK_INFORMATION sci
// ������    IN UINT32 CallbackCount
// ˵����    ö�ٴ������̻ص�
//************************************
BOOLEAN APGetCreateProcessCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 CallbackCount);

//************************************
// ������:   APGetPspCreateThreadNotifyRoutineAddress
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ˵����    ���PspCreateThreadNotifyRoutine
//************************************
UINT_PTR APGetPspCreateThreadNotifyRoutineAddress();

//************************************
// ������:   APGetCreateThreadCallbackNotify
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    OUT PSYS_CALLBACK_INFORMATION sci
// ������    IN UINT32 NumberOfCallbacks
// ˵����    ö�ٴ����̻߳ص�
//************************************
BOOLEAN APGetCreateThreadCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 NumberOfCallbacks);

//************************************
// ������:   APGetPspLoadImageNotifyRoutineAddress
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ˵����    ���PspLoadImageNotifyRoutine
//************************************
UINT_PTR APGetPspLoadImageNotifyRoutineAddress();

//************************************
// ������:   APGetLoadImageCallbackNotify
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    OUT PSYS_CALLBACK_INFORMATION sci
// ������    IN UINT32 NumberOfCallbacks
// ˵����    ö�ټ���ģ��ص�
//************************************
BOOLEAN APGetLoadImageCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 NumberOfCallbacks);

//************************************
// ������:   APGetCallbackListHeadAddress
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ˵����    ���CallbackListHead
//************************************
UINT_PTR APGetCallbackListHeadAddress();

//************************************
// ������:   APGetRegisterCallbackNotify
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    OUT PSYS_CALLBACK_INFORMATION sci
// ������    IN UINT32 NumberOfCallbacks
// ˵����    ö��ע���ص�
//************************************
BOOLEAN APGetRegisterCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 NumberOfCallbacks);

//************************************
// ������:   APGetKeBugCheckCallbackListHeadAddress
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ˵����    ���KeBugCheckCallbackListHead
//************************************
UINT_PTR APGetKeBugCheckCallbackListHeadAddress();

//************************************
// ������:   APGetBugCheckCallbackNotify
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    OUT PSYS_CALLBACK_INFORMATION sci
// ������    IN UINT32 NumberOfCallbacks
// ˵����    ö�������ص�
//************************************
BOOLEAN APGetBugCheckCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 NumberOfCallbacks);

//************************************
// ������:   APGetKeBugCheckReasonCallbackListHeadAddress
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ˵����    ���KeBugCheckReasonCallbackListHead
//************************************
UINT_PTR APGetKeBugCheckReasonCallbackListHeadAddress();

//************************************
// ������:   APGetBugCheckReasonCallbackNotify
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    OUT PSYS_CALLBACK_INFORMATION sci
// ������    IN UINT32 NumberOfCallbacks
// ˵����    ö�ٴ�ԭ��Ĵ�����ص�
//************************************
BOOLEAN APGetBugCheckReasonCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 NumberOfCallbacks);

//************************************
// ������:   APGetIopNotifyShutdownQueueHeadAddress
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ˵����    ���IopNotifyShutdownQueueHead
//************************************
UINT_PTR APGetIopNotifyShutdownQueueHeadAddress();

//************************************
// ������:   APGetShutdownDispatch
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ������    IN PDEVICE_OBJECT DeviceObject
// ˵����    ͨ���豸�����ùػ���ǲ����
//************************************
UINT_PTR APGetShutdownDispatch(IN PDEVICE_OBJECT DeviceObject);

//************************************
// ������:   APGetShutDownCallbackNotify
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    OUT PSYS_CALLBACK_INFORMATION sci
// ������    IN UINT32 NumberOfCallbacks
// ˵����    ö�ٹػ��ص�
//************************************
BOOLEAN APGetShutDownCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 NumberOfCallbacks);

//************************************
// ������:   APGetIopNotifyLastChanceShutdownQueueHeadAddress
// Ȩ�ޣ�    public 
// ����ֵ:   UINT_PTR
// ˵����    ���IopNotifyLastChanceShutdownQueueHead
//************************************
UINT_PTR APGetIopNotifyLastChanceShutdownQueueHeadAddress();

//************************************
// ������:   APGetLastChanceShutDownCallbackNotify
// Ȩ�ޣ�    public 
// ����ֵ:   BOOLEAN
// ������    OUT PSYS_CALLBACK_INFORMATION sci
// ������    IN UINT32 CallbackCount
// ˵����    ö��������Ĺػ��ص�
//************************************
BOOLEAN APGetLastChanceShutDownCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 CallbackCount);

//************************************
// ������:   APEnumSystemCallback
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    OUT PVOID OutputBuffer          R3Buffer��ַ
// ������    IN UINT32 OutputLength          R3Buffer����
// ˵����    ö�����е�ϵͳ�ص�
//************************************
NTSTATUS APEnumSystemCallback(OUT PVOID OutputBuffer, IN UINT32 OutputLength);



#endif // !CXX_SystemCallback_H


