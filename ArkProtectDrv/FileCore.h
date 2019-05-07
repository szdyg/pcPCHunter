#pragma once
#include <ntifs.h>
#include "Private.h"


//************************************
// ������:   APIrpCompleteRoutine
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN PDEVICE_OBJECT DeviceObject
// ������    IN PIRP Irp
// ������    IN PVOID Context
// ˵����    Irp������̻ص�
//************************************
NTSTATUS APIrpCompleteRoutine(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);

//************************************
// ������:   APDeleteFileByIrp
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN PWCHAR  wzFilePath
// ˵����    ͨ������Irpɾ���ļ�
//************************************
NTSTATUS APDeleteFileByIrp(IN PWCHAR  wzFilePath);

//************************************
// ������:   APDeleteFile
// Ȩ�ޣ�    public 
// ����ֵ:   NTSTATUS
// ������    IN WCHAR * wzFilePath
// ˵����    ɾ���ļ�
//************************************
NTSTATUS APDeleteFile(IN PWCHAR  wzFilePath);




