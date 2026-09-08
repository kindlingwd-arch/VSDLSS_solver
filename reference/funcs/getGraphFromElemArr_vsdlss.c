/* VSDLSS function (decompiled by Ghidra) */
/* name: getGraphFromElemArr_vsdlss  addr: 009e7230  size: ee */
#include "vsdlss_ref.h"

/* signature: long getGraphFromElemArr_vsdlss(undefined4 param_1,int param_2,long param_3); */

long getGraphFromElemArr_vsdlss(undefined4 param_1,int param_2,long param_3)

{
  undefined8 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  long lVar5;
  undefined8 uVar6;
  int iVar7;
  int local_30;
  int local_2c;
  
  uVar2 = *(undefined4 *)(param_3 + 8);
  uVar3 = *(undefined4 *)(param_3 + 0xc);
  lVar5 = newGraph_vsdlss();
  uVar6 = newCEdgeVector_vsdlss(param_1,1,0);
  *(undefined8 *)(lVar5 + 0x18) = uVar6;
  iVar7 = 0;
  initReadFromTo_vsdlss(uVar2,uVar3,param_2);
  iVar4 = getNextFromTo_vsdlss(&local_30,&local_2c);
  while (iVar4 == 0) {
    iVar7 = iVar7 + 2;
    puVar1 = (undefined8 *)(*(long *)(lVar5 + 0x18) + (long)local_30 * 8);
    uVar6 = newCEdgelist_vsdlss(local_2c,*puVar1);
    *puVar1 = uVar6;
    puVar1 = (undefined8 *)(*(long *)(lVar5 + 0x18) + (long)local_2c * 8);
    uVar6 = newCEdgelist_vsdlss(local_30,*puVar1);
    *puVar1 = uVar6;
    iVar4 = getNextFromTo_vsdlss(&local_30,&local_2c);
  }
  finishReadFromTo_vsdlss();
  if (param_2 * 2 != iVar7) {
    nrerror_vsdlss(1,"elemarrutils.c",0x26c);
  }
  *(int *)(lVar5 + 4) = iVar7;
  eliminateDuplicateEdgesInGraph_vsdlss(lVar5);
  return lVar5;
}


