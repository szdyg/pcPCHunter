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
// 函数名:   APGetPspCreateProcessNotifyRoutineAddress
// 权限：    public 
// 返回值:   UINT_PTR
// 说明：    PspCreateProcessNotifyRoutine
//************************************
UINT_PTR APGetPspCreateProcessNotifyRoutineAddress();

//************************************
// 函数名:   APGetCreateProcessCallbackNotify
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    OUT PSYS_CALLBACK_INFORMATION sci
// 参数：    IN UINT32 CallbackCount
// 说明：    枚举创建进程回调
//************************************
BOOLEAN APGetCreateProcessCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 CallbackCount);

//************************************
// 函数名:   APGetPspCreateThreadNotifyRoutineAddress
// 权限：    public 
// 返回值:   UINT_PTR
// 说明：    获得PspCreateThreadNotifyRoutine
//************************************
UINT_PTR APGetPspCreateThreadNotifyRoutineAddress();

//************************************
// 函数名:   APGetCreateThreadCallbackNotify
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    OUT PSYS_CALLBACK_INFORMATION sci
// 参数：    IN UINT32 NumberOfCallbacks
// 说明：    枚举创建线程回调
//************************************
BOOLEAN APGetCreateThreadCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 NumberOfCallbacks);

//************************************
// 函数名:   APGetPspLoadImageNotifyRoutineAddress
// 权限：    public 
// 返回值:   UINT_PTR
// 说明：    获得PspLoadImageNotifyRoutine
//************************************
UINT_PTR APGetPspLoadImageNotifyRoutineAddress();

//************************************
// 函数名:   APGetLoadImageCallbackNotify
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    OUT PSYS_CALLBACK_INFORMATION sci
// 参数：    IN UINT32 NumberOfCallbacks
// 说明：    枚举加载模块回调
//************************************
BOOLEAN APGetLoadImageCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 NumberOfCallbacks);

//************************************
// 函数名:   APGetCallbackListHeadAddress
// 权限：    public 
// 返回值:   UINT_PTR
// 说明：    获得CallbackListHead
//************************************
UINT_PTR APGetCallbackListHeadAddress();

//************************************
// 函数名:   APGetRegisterCallbackNotify
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    OUT PSYS_CALLBACK_INFORMATION sci
// 参数：    IN UINT32 NumberOfCallbacks
// 说明：    枚举注册表回调
//************************************
BOOLEAN APGetRegisterCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 NumberOfCallbacks);

//************************************
// 函数名:   APGetKeBugCheckCallbackListHeadAddress
// 权限：    public 
// 返回值:   UINT_PTR
// 说明：    获得KeBugCheckCallbackListHead
//************************************
UINT_PTR APGetKeBugCheckCallbackListHeadAddress();

//************************************
// 函数名:   APGetBugCheckCallbackNotify
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    OUT PSYS_CALLBACK_INFORMATION sci
// 参数：    IN UINT32 NumberOfCallbacks
// 说明：    枚举蓝屏回调
//************************************
BOOLEAN APGetBugCheckCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 NumberOfCallbacks);

//************************************
// 函数名:   APGetKeBugCheckReasonCallbackListHeadAddress
// 权限：    public 
// 返回值:   UINT_PTR
// 说明：    获得KeBugCheckReasonCallbackListHead
//************************************
UINT_PTR APGetKeBugCheckReasonCallbackListHeadAddress();

//************************************
// 函数名:   APGetBugCheckReasonCallbackNotify
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    OUT PSYS_CALLBACK_INFORMATION sci
// 参数：    IN UINT32 NumberOfCallbacks
// 说明：    枚举带原因的错误检测回调
//************************************
BOOLEAN APGetBugCheckReasonCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 NumberOfCallbacks);

//************************************
// 函数名:   APGetIopNotifyShutdownQueueHeadAddress
// 权限：    public 
// 返回值:   UINT_PTR
// 说明：    获得IopNotifyShutdownQueueHead
//************************************
UINT_PTR APGetIopNotifyShutdownQueueHeadAddress();

//************************************
// 函数名:   APGetShutdownDispatch
// 权限：    public 
// 返回值:   UINT_PTR
// 参数：    IN PDEVICE_OBJECT DeviceObject
// 说明：    通过设备对象获得关机派遣函数
//************************************
UINT_PTR APGetShutdownDispatch(IN PDEVICE_OBJECT DeviceObject);

//************************************
// 函数名:   APGetShutDownCallbackNotify
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    OUT PSYS_CALLBACK_INFORMATION sci
// 参数：    IN UINT32 NumberOfCallbacks
// 说明：    枚举关机回调
//************************************
BOOLEAN APGetShutDownCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 NumberOfCallbacks);

//************************************
// 函数名:   APGetIopNotifyLastChanceShutdownQueueHeadAddress
// 权限：    public 
// 返回值:   UINT_PTR
// 说明：    获得IopNotifyLastChanceShutdownQueueHead
//************************************
UINT_PTR APGetIopNotifyLastChanceShutdownQueueHeadAddress();

//************************************
// 函数名:   APGetLastChanceShutDownCallbackNotify
// 权限：    public 
// 返回值:   BOOLEAN
// 参数：    OUT PSYS_CALLBACK_INFORMATION sci
// 参数：    IN UINT32 CallbackCount
// 说明：    枚举最后机会的关机回调
//************************************
BOOLEAN APGetLastChanceShutDownCallbackNotify(OUT PSYS_CALLBACK_INFORMATION sci, IN UINT32 CallbackCount);

//************************************
// 函数名:   APEnumSystemCallback
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    OUT PVOID OutputBuffer          R3Buffer地址
// 参数：    IN UINT32 OutputLength          R3Buffer长度
// 说明：    枚举所有的系统回调
//************************************
NTSTATUS APEnumSystemCallback(OUT PVOID OutputBuffer, IN UINT32 OutputLength);



#endif // !CXX_SystemCallback_H


