/* VSDLSS function (decompiled by Ghidra) */
/* name: readAndExtendEaOffDiagVec_vsdlss  addr: 00a12000  size: 10e */
#include "vsdlss_ref.h"

/* signature: void readAndExtendEaOffDiagVec_vsdlss(int param_1,int param_2,long param_3,long param_4); */

void readAndExtendEaOffDiagVec_vsdlss(int param_1,int param_2,long param_3,long param_4)

{
  int iVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  
  iVar1 = *(int *)(param_3 + 0x10);
  lVar3 = *(long *)(param_3 + 0x30);
  lVar6 = newFVector_vsdlss(0,(long)param_1,0);
  *(long *)(param_4 + 0x30) = lVar6;
  readFVectorFNum_vsdlss(0x1d,param_1 - param_2,lVar6);
  if (iVar1 < 1) {
    iVar9 = 0;
  }
  else {
    lVar4 = *(long *)(param_3 + 0x40);
    lVar8 = 0;
    iVar9 = 0;
    do {
      iVar2 = *(int *)(lVar3 + 4 + lVar8);
      lVar5 = *(long *)(*(long *)(lVar4 + 8 + lVar8 * 2) + 8);
      if (0 < iVar2) {
        lVar7 = 0;
        do {
          *(undefined8 *)(lVar6 + (long)(param_1 - param_2) * 8 + (long)iVar9 * 8 + 8 + lVar7) =
               *(undefined8 *)(lVar5 + 8 + lVar7);
          lVar7 = lVar7 + 8;
        } while (lVar7 != (ulong)(iVar2 - 1) * 8 + 8);
        iVar9 = iVar9 + iVar2;
      }
      lVar8 = lVar8 + 4;
    } while (lVar8 != (ulong)(iVar1 - 1) * 4 + 4);
  }
  if (iVar9 != param_2) {
    nrerror_vsdlss(1,"reduceproblem.c",0x573);
    return;
  }
  return;
}


