/* VSDLSS function (decompiled by Ghidra) */
/* name: readAndExtendDiagVec_vsdlss  addr: 00a12110  size: 67 */
#include "vsdlss_ref.h"

/* signature: void readAndExtendDiagVec_vsdlss(int param_1,int param_2); */

void readAndExtendDiagVec_vsdlss(int param_1,int param_2)

{
  long lVar1;
  undefined8 *puVar2;
  int iVar3;
  
  iVar3 = param_1 - param_2;
  lVar1 = newFVector_vsdlss(0,(long)param_1,0);
  readFVectorFNum_vsdlss(0x1a,iVar3,lVar1);
  if (0 < param_2) {
    puVar2 = (undefined8 *)(lVar1 + 8 + (long)iVar3 * 8);
    do {
      *puVar2 = 0;
      puVar2 = puVar2 + 1;
    } while (puVar2 != (undefined8 *)(lVar1 + 0x10 + ((long)iVar3 + (ulong)(param_2 - 1)) * 8));
  }
  return;
}


