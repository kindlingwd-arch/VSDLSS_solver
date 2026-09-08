/* VSDLSS function (decompiled by Ghidra) */
/* name: insertIntoHt_vsdlss  addr: 00a2f880  size: 2b */
#include "vsdlss_ref.h"

/* signature: void insertIntoHt_vsdlss(int param_1,undefined4 param_2,int *param_3); */

void insertIntoHt_vsdlss(int param_1,undefined4 param_2,int *param_3)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)(*(long *)(param_3 + 2) + (long)((param_1 + -1) % *param_3 + 1) * 8);
  uVar2 = newCEdgelist_vsdlss(param_2,*puVar1);
  *puVar1 = uVar2;
  return;
}


