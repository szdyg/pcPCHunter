#pragma once
#include <ntifs.h>
#include "Private.h"


//************************************
// 函数名:   APIrpCompleteRoutine
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN PDEVICE_OBJECT DeviceObject
// 参数：    IN PIRP Irp
// 参数：    IN PVOID Context
// 说明：    Irp完成例程回调
//************************************
NTSTATUS APIrpCompleteRoutine(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);

//************************************
// 函数名:   APDeleteFileByIrp
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN PWCHAR  wzFilePath
// 说明：    通过构建Irp删除文件
//************************************
NTSTATUS APDeleteFileByIrp(IN PWCHAR  wzFilePath);

//************************************
// 函数名:   APDeleteFile
// 权限：    public 
// 返回值:   NTSTATUS
// 参数：    IN WCHAR * wzFilePath
// 说明：    删除文件
//************************************
NTSTATUS APDeleteFile(IN PWCHAR  wzFilePath);




