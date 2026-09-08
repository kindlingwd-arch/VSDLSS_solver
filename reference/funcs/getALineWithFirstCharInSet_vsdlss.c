/* VSDLSS function (decompiled by Ghidra) */
/* name: getALineWithFirstCharInSet_vsdlss  addr: 009d2240  size: 61 */
#include "vsdlss_ref.h"

/* signature: int getALineWithFirstCharInSet_vsdlss(undefined8 param_1,char *param_2,char *param_3); */

int getALineWithFirstCharInSet_vsdlss(undefined8 param_1,char *param_2,char *param_3)

{
  bool bVar1;
  int iVar2;
  char *pcVar3;
  char local_28 [16];
  
  do {
    iVar2 = getALine_vsdlss(param_3,0x3ff,param_1);
    bVar1 = false;
    if (iVar2 != 0) {
      return iVar2;
    }
    do {
      if (bVar1) {
        return 0;
      }
      local_28[0] = *param_3;
      local_28[1] = 0;
      pcVar3 = strpbrk(local_28,param_2);
      bVar1 = true;
    } while (pcVar3 != (char *)0x0);
  } while( true );
}


