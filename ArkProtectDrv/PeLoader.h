#pragma once
#include <ntifs.h>
#include <ntimage.h>
#include <ntstrsafe.h>
#include "NtStructs.h"

//************************************
// 函数名:   APMappingFileInKernelSpace
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN PWCHAR wzFileFullPath               文件完整路径
// 参数：    OUT PVOID * MappingBaseAddress         映射后的基地址
// 说明：    将PE文件映射到内核空间
//************************************
NTSTATUS APMappingFileInKernelSpace(IN PWCHAR  wzFileFullPath, OUT PVOID * ppMappingBaseAddress);

//************************************
// 函数名:   APGetFileBuffer
// 权限：    public 
// 返回值:   PVOID                                  读取文件到内存的首地址
// 参数：    IN PUNICODE_STRING uniFilePath         文件路径
// 说明：    读取文件到内存
//************************************
PVOID APGetFileBuffer(IN PUNICODE_STRING uniFilePath);

//************************************
// 函数名:   APGetModuleHandle
// 权限：    public 
// 返回值:   PVOID                                   模块在内存中首地址
// 参数：    IN PCHAR szModuleName                   模块名称
// 说明：    通过遍历Ldr枚举模块
//************************************
PVOID APGetModuleHandle(IN PCHAR szModuleName);

//************************************
// 函数名:   APGetProcAddress
// 权限：    public 
// 返回值:   PVOID                                   导出函数地址
// 参数：    IN PVOID ModuleBase                     导出模块基地址 （PVOID）
// 参数：    IN PCHAR szFunctionName                 导出函数名称   （PCHAR）
// 说明：    获得导出函数地址（处理转发）
//************************************
PVOID APGetProcAddress(IN PVOID ModuleBase, IN PCHAR szFunctionName);

//************************************
// 函数名:   APFixImportAddressTable
// 权限：    public 
// 返回值:   VOID
// 参数：    IN PVOID ImageBase                新模块加载基地址 （PVOID）
// 说明：    修正导入表  IAT 填充函数地址
//************************************
VOID APFixImportAddressTable(IN PVOID ImageBase);

//************************************
// 函数名:   APFixRelocBaseTable
// 权限：    public 
// 返回值:   VOID
// 参数：    IN PVOID ImageBase               新模块加载基地址 （PVOID）
// 参数：    IN PVOID OriginalBase            原模块加载基地址 （PVOID）
// 说明：    修正重定向表
//************************************
VOID APFixRelocBaseTable(IN PVOID ImageBase, IN PVOID OriginalBase);
