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
// ������:   GetFuncAddress
// Ȩ�ޣ�    public 
// ����ֵ:   ULONG_PTR
// ������    PCHAR szFuncName              ������
// ˵����    ��ȡSSDT�к����ĵ�ַ
//************************************
ULONG_PTR  GetFuncAddress(PCHAR szFuncName);

//************************************
// ������:   GetSSDTApiFunIndex
// Ȩ�ޣ�    public 
// ����ֵ:   LONG
// ������    IN LPSTR lpszFunName         ������
// ˵����    ��ȡSSDT�к��������Ժ�
//************************************
LONG GetSSDTApiFunIndex(IN LPSTR lpszFunName);

//************************************
// ������:   MapFileInUserSpace
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN LPWSTR lpszFileName
// ������    IN HANDLE ProcessHandle OPTIONAL
// ������    OUT PVOID * ppBaseAddress
// ������    OUT PSIZE_T ViewSize OPTIONAL
// ˵����    
//************************************
NTSTATUS MapFileInUserSpace(IN LPWSTR lpszFileName,IN HANDLE ProcessHandle OPTIONAL,OUT PVOID *ppBaseAddress,OUT PSIZE_T ViewSize OPTIONAL);


//************************************
// ������:   GetKeServiceDescriptorTable64
// Ȩ�ޣ�    public 
// ����ֵ:   ULONG_PTR
// ˵����    ��ȡSSDT��ʼ��ַ
//************************************
ULONG_PTR GetKeServiceDescriptorTable64();

//************************************
// ������:   GetSSDTApiFunAddress
// Ȩ�ޣ�    public 
// ����ֵ:   ULONG_PTR
// ������    ULONG_PTR ulIndex                  ������
// ������    ULONG_PTR SSDTDescriptor           SSDT��ʼ��ַ
// ˵����    ��ȡSSDT������ַ
//************************************
ULONG_PTR GetSSDTApiFunAddress(ULONG_PTR ulIndex,ULONG_PTR SSDTDescriptor);

ULONG_PTR GetSSDTFunctionAddress_7601(ULONG_PTR ulIndex, ULONG_PTR SSDTDescriptor);


