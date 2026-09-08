/* VSDLSS function (decompiled by Ghidra) */
/* name: markListMark_vsdlss  addr: 009f6ae0  size: 76 */
#include "vsdlss_ref.h"

/* signature: void markListMark_vsdlss(int *param_1,int param_2); */

void markListMark_vsdlss(int *param_1,int param_2)

{
  char cVar1;
  char *pcVar2;
  undefined8 uVar3;
  
  if ((param_2 < 1) || (*param_1 < param_2)) {
    nrerror_vsdlss(1,"listmarkutils.c",0x70);
    pcVar2 = (char *)((long)param_2 + *(long *)(param_1 + 2));
    cVar1 = *pcVar2;
  }
  else {
    pcVar2 = (char *)((long)param_2 + *(long *)(param_1 + 2));
    cVar1 = *pcVar2;
  }
  if (cVar1 == '\0') {
    *pcVar2 = '\x01';
    uVar3 = newCEdgelist_vsdlss(param_2,0);
    if (*(long *)(param_1 + 6) != 0) {
      *(undefined8 *)(*(long *)(param_1 + 6) + 8) = uVar3;
      *(undefined8 *)(param_1 + 6) = uVar3;
      return;
    }
    *(undefined8 *)(param_1 + 6) = uVar3;
    *(undefined8 *)(param_1 + 4) = uVar3;
  }
  return;
}


