#pragma once
#include <ntifs.h>
#include <devioctl.h>

typedef unsigned long DWORD;




// typedef struct _MEMORY_BASIC_INFORMATION
// {
//     PVOID       BaseAddress;           //��ѯ�ڴ����ռ�ĵ�һ��ҳ�����ַ
//     PVOID       AllocationBase;        //�ڴ����ռ�ĵ�һ���������ַ��С�ڵ���BaseAddress��
//     DWORD       AllocationProtect;     //���򱻳��α���ʱ����ı�������
//     ULONG_PTR   RegionSize;            //��BaseAddress��ʼ��������ͬ���Ե�ҳ��Ĵ�С��
//     DWORD       State;                 //ҳ���״̬�������ֿ���ֵMEM_COMMIT��MEM_FREE��MEM_RESERVE
//     DWORD       Protect;               //ҳ������ԣ�����ܵ�ȡֵ��AllocationProtect��ͬ
//     DWORD       Type;                  //���ڴ������ͣ������ֿ���ֵ��MEM_IMAGE��MEM_MAPPED��MEM_PRIVATE
// } MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

typedef NTSTATUS(*pfnNtQueryVirtualMemory)(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    MEMORY_INFORMATION_CLASS MemoryInformationClass,
    PVOID MemoryInformation,
    ULONG_PTR MemoryInformationLength,
    PSIZE_T ReturnLength);

NTSTATUS EnumMoudleByNtQueryVirtualMemory(ULONG ProcessId);


