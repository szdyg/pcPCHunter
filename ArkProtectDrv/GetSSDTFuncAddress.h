#pragma once
#include "ZwQueryVirtualMemory.h"
#include <ntimage.h>

#define MEM_IMAGE         SEC_IMAGE
#define SEC_IMAGE 0x01000000

typedef struct _SYSTEM_SERVICE_TABLE64
{
    PVOID          ServiceTableBase; 
    PVOID          ServiceCounterTableBase; 
    ULONG64        NumberOfServices; 
    PVOID          ParamTableBase; 
} SYSTEM_SERVICE_TABLE64, *PSYSTEM_SERVICE_TABLE64;

typedef struct _SYSTEM_SERVICE_TABLE32 
{
    PVOID          ServiceTableBase;
    PVOID          ServiceCounterTableBase;
    ULONG32        NumberOfServices;
    PVOID          ParamTableBase;
} SYSTEM_SERVICE_TABLE32, *PSYSTEM_SERVICE_TABLE32;

NTSYSAPI PIMAGE_NT_HEADERS NTAPI RtlImageNtHeader(PVOID Base);


//************************************
// 函数名:   GetFuncAddress
// 权限：    public 
// 返回值:   ULONG_PTR
// 参数：    PCHAR szFuncName              函数名
// 说明：    获取SSDT中函数的地址
//************************************
ULONG_PTR  GetFuncAddress(PCHAR szFuncName);

//************************************
// 函数名:   GetSSDTApiFunIndex
// 权限：    public 
// 返回值:   LONG
// 参数：    IN LPSTR lpszFunName         函数名
// 说明：    获取SSDT中函数的所以号
//************************************
LONG GetSSDTApiFunIndex(IN LPSTR lpszFunName);

//************************************
// 函数名:   MapFileInUserSpace
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN LPWSTR lpszFileName
// 参数：    IN HANDLE ProcessHandle OPTIONAL
// 参数：    OUT PVOID * ppBaseAddress
// 参数：    OUT PSIZE_T ViewSize OPTIONAL
// 说明：    
//************************************
NTSTATUS MapFileInUserSpace(IN LPWSTR lpszFileName,IN HANDLE ProcessHandle OPTIONAL,OUT PVOID *ppBaseAddress,OUT PSIZE_T ViewSize OPTIONAL);


//************************************
// 函数名:   GetKeServiceDescriptorTable64
// 权限：    public 
// 返回值:   ULONG_PTR
// 说明：    获取SSDT起始地址
//************************************
ULONG_PTR GetKeServiceDescriptorTable64();

//************************************
// 函数名:   GetSSDTApiFunAddress
// 权限：    public 
// 返回值:   ULONG_PTR
// 参数：    ULONG_PTR ulIndex                  索引号
// 参数：    ULONG_PTR SSDTDescriptor           SSDT起始地址
// 说明：    获取SSDT函数地址
//************************************
ULONG_PTR GetSSDTApiFunAddress(ULONG_PTR ulIndex,ULONG_PTR SSDTDescriptor);

ULONG_PTR GetSSDTFunctionAddress_7601(ULONG_PTR ulIndex, ULONG_PTR SSDTDescriptor);


