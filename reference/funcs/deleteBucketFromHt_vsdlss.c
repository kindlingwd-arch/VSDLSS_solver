/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteBucketFromHt_vsdlss  addr: 00a2f7f0  size: 22 */
#include "vsdlss_ref.h"

/* signature: void deleteBucketFromHt_vsdlss(int param_1,int *param_2); */

void deleteBucketFromHt_vsdlss(int param_1,int *param_2)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)(*(long *)(param_2 + 2) + (long)((param_1 + -1) % *param_2 + 1) * 8);
  uVar2 = deleteEntireCEdgelist_vsdlss(*puVar1);
  *puVar1 = uVar2;
  return;
}


