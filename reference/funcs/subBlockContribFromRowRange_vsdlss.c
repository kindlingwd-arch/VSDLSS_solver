/* VSDLSS function (decompiled by Ghidra) */
/* name: subBlockContribFromRowRange_vsdlss  addr: 009ee170  size: 166 */
#include "vsdlss_ref.h"

/* signature: void subBlockContribFromRowRange_vsdlss(int param_1,int param_2,int param_3,int param_4,undefined8 param_5,long param_6); */

void subBlockContribFromRowRange_vsdlss
               (int param_1,int param_2,int param_3,int param_4,undefined8 param_5,long param_6)

{
  int iVar1;
  int iVar2;
  long lVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  undefined8 in_stack_00000020;
  
  iVar2 = *(int *)(*(long *)(param_6 + 0x28) + (long)param_1 * 4);
  lVar3 = *(long *)(*(long *)(param_6 + 0x30) + (long)param_1 * 8);
  if ((param_4 < param_3) || (iVar5 = (param_2 - param_1) + 2, iVar2 < iVar5)) {
    return;
  }
  if (*(int *)(lVar3 + (long)iVar5 * 4) < param_3) {
    piVar4 = (int *)(lVar3 + 4 + (long)iVar5 * 4);
    do {
      iVar5 = iVar5 + 1;
      if (iVar2 < iVar5) {
        return;
      }
      iVar6 = *piVar4;
      piVar4 = piVar4 + 1;
    } while (iVar6 < param_3);
  }
  iVar6 = iVar5 + -1;
  if (iVar6 < iVar2) {
    if (*(int *)(lVar3 + ((long)iVar6 + 1) * 4) <= param_4) {
      piVar4 = (int *)(lVar3 + 4 + ((long)iVar6 + 1) * 4);
      do {
        iVar6 = iVar6 + 1;
        if (iVar2 <= iVar6) goto LAB_009ee254;
        iVar1 = *piVar4;
        piVar4 = piVar4 + 1;
      } while (iVar1 <= param_4);
    }
    if (iVar6 < iVar5) {
      return;
    }
  }
LAB_009ee254:
  copySupNodeValsToLongVec_vsdlss(param_1,param_2,iVar5,param_6,in_stack_00000020);
  subBlockContribFromRowRange_1_vsdlss(param_1,param_2,iVar5,iVar6,param_5,param_6);
  return;
}


