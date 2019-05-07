#pragma once
#include <ntifs.h>
#include <ntimage.h>
#include <ntstrsafe.h>
#include "NtStructs.h"

//************************************
// ������:   APMappingFileInKernelSpace
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN PWCHAR wzFileFullPath               �ļ�����·��
// ������    OUT PVOID * MappingBaseAddress         ӳ���Ļ���ַ
// ˵����    ��PE�ļ�ӳ�䵽�ں˿ռ�
//************************************
NTSTATUS APMappingFileInKernelSpace(IN PWCHAR  wzFileFullPath, OUT PVOID * ppMappingBaseAddress);

//************************************
// ������:   APGetFileBuffer
// Ȩ�ޣ�    public 
// ����ֵ:   PVOID                                  ��ȡ�ļ����ڴ���׵�ַ
// ������    IN PUNICODE_STRING uniFilePath         �ļ�·��
// ˵����    ��ȡ�ļ����ڴ�
//************************************
PVOID APGetFileBuffer(IN PUNICODE_STRING uniFilePath);

//************************************
// ������:   APGetModuleHandle
// Ȩ�ޣ�    public 
// ����ֵ:   PVOID                                   ģ�����ڴ����׵�ַ
// ������    IN PCHAR szModuleName                   ģ������
// ˵����    ͨ������Ldrö��ģ��
//************************************
PVOID APGetModuleHandle(IN PCHAR szModuleName);

//************************************
// ������:   APGetProcAddress
// Ȩ�ޣ�    public 
// ����ֵ:   PVOID                                   ����������ַ
// ������    IN PVOID ModuleBase                     ����ģ�����ַ ��PVOID��
// ������    IN PCHAR szFunctionName                 ������������   ��PCHAR��
// ˵����    ��õ���������ַ������ת����
//************************************
PVOID APGetProcAddress(IN PVOID ModuleBase, IN PCHAR szFunctionName);

//************************************
// ������:   APFixImportAddressTable
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN PVOID ImageBase                ��ģ����ػ���ַ ��PVOID��
// ˵����    ���������  IAT ��亯����ַ
//************************************
VOID APFixImportAddressTable(IN PVOID ImageBase);

//************************************
// ������:   APFixRelocBaseTable
// Ȩ�ޣ�    public 
// ����ֵ:   VOID
// ������    IN PVOID ImageBase               ��ģ����ػ���ַ ��PVOID��
// ������    IN PVOID OriginalBase            ԭģ����ػ���ַ ��PVOID��
// ˵����    �����ض����
//************************************
VOID APFixRelocBaseTable(IN PVOID ImageBase, IN PVOID OriginalBase);
