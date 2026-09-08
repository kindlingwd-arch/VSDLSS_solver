/* VSDLSS function (decompiled by Ghidra) */
/* name: copyVectorRange_vsdlss  addr: 009d7be0  size: 35 */
#include "vsdlss_ref.h"

/* signature: void copyVectorRange_vsdlss(int param_1,int param_2,long param_3,long param_4); */

void copyVectorRange_vsdlss(int param_1,int param_2,long param_3,long param_4)

{
  long lVar1;
  
  if (param_1 <= param_2) {
    lVar1 = 0;
    do {
      *(undefined8 *)(param_4 + (long)param_1 * 8 + lVar1) =
           *(undefined8 *)(param_3 + (long)param_1 * 8 + lVar1);
      lVar1 = lVar1 + 8;
    } while (lVar1 != (ulong)(uint)(param_2 - param_1) * 8 + 8);
  }
  return;
}


