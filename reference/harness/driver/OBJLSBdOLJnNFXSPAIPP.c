/* driver: OBJLSBdOLJnNFXSPAIPP addr=009a7ad0 size=99 */
#include "vsdlss_ref.h"


void OBJLSBdOLJnNFXSPAIPP(long param_1,long param_2)

{
  undefined8 *puVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  
  lVar5 = *(long *)(*(long *)(param_2 + 0x18) + param_1 * 8);
  if (-1 < lVar5) {
    lVar4 = *(long *)(param_2 + 0x10);
    if (lVar4 < 1) {
      KOBOOLOXSPAIPP(5,"priorityqutils_i.c",0x297);
      lVar5 = *(long *)(*(long *)(param_2 + 0x18) + param_1 * 8);
      lVar4 = *(long *)(param_2 + 0x10);
    }
    lVar2 = *(long *)(*(long *)(param_2 + 0x20) + lVar4 * 8);
    puVar1 = (undefined8 *)(*(long *)(param_2 + 0x28) + lVar2 * 8);
    uVar3 = *puVar1;
    *puVar1 = *(undefined8 *)(*(long *)(param_2 + 0x28) + param_1 * 8);
    MNFqTyMXSPAIPP(lVar5,lVar4,param_2);
    *(long *)(param_2 + 0x10) = *(long *)(param_2 + 0x10) + -1;
    RMAyQBnNFXSPAIPP(lVar2,uVar3,param_2);
    *(undefined8 *)(*(long *)(param_2 + 0x18) + param_1 * 8) = 0xffffffffffffffff;
  }
  return;
}


