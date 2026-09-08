/* VSDLSS function (decompiled by Ghidra) */
/* name: labelWithReps_vsdlss  addr: 00a0a020  size: 119 */
#include "vsdlss_ref.h"

/* signature: void labelWithReps_vsdlss(int param_1,int param_2,int param_3,int param_4,int param_5,long param_6,long param_7); */

void labelWithReps_vsdlss
               (int param_1,int param_2,int param_3,int param_4,int param_5,long param_6,
               long param_7)

{
  int *piVar1;
  long *plVar2;
  int iVar3;
  int iVar4;
  undefined8 *puVar5;
  long lVar6;
  int *piVar7;
  int iVar8;
  bool bVar9;
  
  if (0 < param_4) {
    puVar5 = (undefined8 *)(param_7 + 8);
    do {
      *puVar5 = 0;
      puVar5 = puVar5 + 1;
    } while (puVar5 != (undefined8 *)(param_7 + 0x10 + (ulong)(param_4 - 1) * 8));
  }
  if (0 < param_3) {
    piVar7 = (int *)(param_6 + (long)param_3 * 4);
    iVar4 = param_3;
    iVar8 = param_3;
LAB_00a0a07e:
    do {
      iVar3 = *(int *)(param_6 + (long)iVar4 * 4);
      if (iVar3 != iVar4) {
        iVar4 = iVar3;
        if (iVar3 <= param_3) goto LAB_00a0a07e;
      }
      iVar4 = *piVar7;
      piVar1 = piVar7;
      if (iVar8 != *piVar7) {
        do {
          if (param_3 < iVar4) break;
          *piVar1 = iVar3;
          piVar1 = (int *)(param_6 + (long)iVar4 * 4);
          bVar9 = *piVar1 != iVar4;
          iVar4 = *piVar1;
        } while (bVar9);
      }
      piVar7 = piVar7 + -1;
      iVar8 = iVar8 + -1;
      iVar4 = iVar8;
    } while (iVar8 != 0);
    piVar7 = (int *)(param_6 + 4);
    iVar8 = 1;
    do {
      while( true ) {
        iVar4 = *piVar7;
        if ((param_5 < iVar4) || (iVar4 < 1)) {
          nrerror_vsdlss(1,"prepostpar.c",0x315);
        }
        lVar6 = (long)iVar4;
        if (param_1 < iVar8) break;
        plVar2 = (long *)(param_7 + lVar6 * 8);
        *plVar2 = *plVar2 + 1;
LAB_00a0a0d5:
        iVar8 = iVar8 + 1;
        piVar7 = piVar7 + 1;
        if (param_3 < iVar8) {
          return;
        }
      }
      if (param_2 < iVar8) {
        plVar2 = (long *)(param_7 + lVar6 * 8);
        *plVar2 = *plVar2 + 3;
        goto LAB_00a0a0d5;
      }
      iVar8 = iVar8 + 1;
      plVar2 = (long *)(param_7 + lVar6 * 8);
      *plVar2 = *plVar2 + 2;
      piVar7 = piVar7 + 1;
    } while (iVar8 <= param_3);
  }
  return;
}


