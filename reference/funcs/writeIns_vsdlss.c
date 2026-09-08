/* VSDLSS function (decompiled by Ghidra) */
/* name: writeIns_vsdlss  addr: 00a22240  size: 180 */
#include "vsdlss_ref.h"

/* signature: void writeIns_vsdlss(int *param_1,FILE *param_2,void *param_3); */

void writeIns_vsdlss(int *param_1,FILE *param_2,void *param_3)

{
  int iVar1;
  size_t sVar2;
  int iVar3;
  long lVar4;
  int local_8c;
  undefined1 local_88 [4];
  int local_84;
  
  iVar1 = *param_1;
  local_8c = 1;
  local_84 = iVar1;
  sVar2 = fwrite(&local_8c,4,1,param_2);
  if ((int)sVar2 != 1) {
    nrerror_vsdlss(0x10,"indistnodesetsutils.c",0x11d,"writeIVector:n",1,sVar2 & 0xffffffff);
  }
  sVar2 = fwrite(local_88,4,(long)(local_8c + 1),param_2);
  if (local_8c + 1 != (int)sVar2) {
    nrerror_vsdlss(0x10,"indistnodesetsutils.c",0x11d,"writeIVector:v",local_8c + 1,
                   sVar2 & 0xffffffff);
  }
  lVar4 = 8;
  iVar3 = 1;
  if (0 < iVar1) {
    do {
      local_8c = copyCEdgelistToArray_vsdlss
                           (*(undefined8 *)(*(long *)(param_1 + 2) + lVar4),param_3);
      sVar2 = fwrite(&local_8c,4,1,param_2);
      if ((int)sVar2 != 1) {
        nrerror_vsdlss(0x10,"indistnodesetsutils.c",0x121,"writeIVector:n",1,sVar2 & 0xffffffff);
      }
      sVar2 = fwrite(param_3,4,(long)(local_8c + 1),param_2);
      if (local_8c + 1 != (int)sVar2) {
        nrerror_vsdlss(0x10,"indistnodesetsutils.c",0x121,"writeIVector:v",local_8c + 1,
                       sVar2 & 0xffffffff);
      }
      iVar3 = iVar3 + 1;
      lVar4 = lVar4 + 8;
    } while (iVar3 <= iVar1);
  }
  return;
}


