/* VSDLSS function (decompiled by Ghidra) */
/* name: writeRhsVec_vsdlss  addr: 009d2ea0  size: d7 */
#include "vsdlss_ref.h"

/* signature: void writeRhsVec_vsdlss(int param_1,int param_2,long param_3,long param_4,long param_5,FILE *param_6); */

void writeRhsVec_vsdlss(int param_1,int param_2,long param_3,long param_4,long param_5,FILE *param_6
                       )

{
  int iVar1;
  void *__ptr;
  size_t sVar2;
  long lVar3;
  
  __ptr = tc_malloc((long)(param_2 + 1) << 3);
  lVar3 = 0;
  if (0 < param_1) {
    do {
      if (*(char *)(param_4 + 1 + lVar3) == '\0') {
        iVar1 = *(int *)(param_3 + 4 + lVar3 * 4);
        if (param_2 < iVar1) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x31a,"Internal data structure integrity is lost\n");
        }
        *(undefined8 *)((long)__ptr + (long)iVar1 * 8) = *(undefined8 *)(param_5 + 8 + lVar3 * 8);
      }
      iVar1 = (int)lVar3;
      lVar3 = lVar3 + 1;
    } while (iVar1 + 2 <= param_1);
  }
  sVar2 = fwrite((void *)((long)__ptr + 8),8,(long)param_2,param_6);
  if ((int)sVar2 == param_2) {
    tc_cfree(__ptr);
    return;
  }
                    /* WARNING: Subroutine does not return */
  fatalError_vsdlss(9999,"main.c",0x324,"Write failed: Writing %d but written %d\n",param_2,
                    sVar2 & 0xffffffff);
}


