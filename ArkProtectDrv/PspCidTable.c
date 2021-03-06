#include "PspCidTable.h"

UINT_PTR g_PspCidTable = 0;

UINT_PTR APGetPspCidTableAddress()
{
    if (g_PspCidTable == 0)
    {
        PVOID PsLookupProcessByProcessIdAddress = NULL;

        APGetNtosExportVariableAddress(L"PsLookupProcessByProcessId", (PVOID*)&PsLookupProcessByProcessIdAddress);

        if (PsLookupProcessByProcessIdAddress != NULL)
        {
            PUINT8    StartSearchAddress = NULL;
            PUINT8    EndSearchAddress = NULL;
            PUINT8    i = NULL;
            UINT8   v1 = 0, v2 = 0, v3 = 0;
            INT32   iOffset = 0;    // 64位下使用 ffed4991 偏移不会超过4字节(大地址在前，小地址在后)

            StartSearchAddress = PsLookupProcessByProcessIdAddress;
            EndSearchAddress = StartSearchAddress + 0x200;
            /*
            3: kd> u PsLookupProcessByProcessId l 20
            nt!PsLookupProcessByProcessId:
            fffff800`041a11fc 48895c2408      mov     qword ptr [rsp+8],rbx
            fffff800`041a1201 48896c2410      mov     qword ptr [rsp+10h],rbp
            fffff800`041a1206 4889742418      mov     qword ptr [rsp+18h],rsi
            fffff800`041a120b 57              push    rdi
            fffff800`041a120c 4154            push    r12
            fffff800`041a120e 4155            push    r13
            fffff800`041a1210 4883ec20        sub     rsp,20h
            fffff800`041a1214 65488b3c2588010000 mov   rdi,qword ptr gs:[188h]
            fffff800`041a121d 4533e4          xor     r12d,r12d
            fffff800`041a1220 488bea          mov     rbp,rdx
            fffff800`041a1223 66ff8fc4010000  dec     word ptr [rdi+1C4h]
            fffff800`041a122a 498bdc          mov     rbx,r12
            fffff800`041a122d 488bd1          mov     rdx,rcx
            fffff800`041a1230 488b0d9149edff  mov     rcx,qword ptr [nt!PspCidTable (fffff800`04075bc8)]
            fffff800`041a1237 e834480200      call    nt!ExMapHandleToPointer (fffff800`041c5a70)
            */

            for (i = StartSearchAddress; i < EndSearchAddress; i++)
            {
                if (MmIsAddressValid(i) && MmIsAddressValid(i + 1) && MmIsAddressValid(i + 7))
                {
                    v1 = *i;
                    v2 = *(i + 1);
                    v3 = *(i + 7);
                    if (v1 == 0x48 && v2 == 0x8b && v3 == 0xe8)        // 488b0d后面有重复出现的，所以+7判断e8
                    {
                        RtlCopyMemory(&iOffset, i + 3, 4);
                        g_PspCidTable = iOffset + (UINT64)i + 7;
                        DbgPrint("PspCidTable :%p\r\n", g_PspCidTable);
                        break;
                    }
                }
            }
        }
    }

    return g_PspCidTable;
}
