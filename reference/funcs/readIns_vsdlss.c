/* VSDLSS function (decompiled by Ghidra) */
/* name: readIns_vsdlss  addr: 00a223c0  size: 186 */
#include "vsdlss_ref.h"

/* signature: long readIns_vsdlss(FILE *param_1,void *param_2); */

long readIns_vsdlss(FILE *param_1,void *param_2)

{
  size_t sVar1;
  long lVar2;
  undefined8 uVar3;
  int iVar4;
  long lVar5;
  undefined8 *puVar6;
  int local_8c;
  undefined1 local_88 [4];
  int local_84;
  
  sVar1 = fread(&local_8c,4,1,param_1);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"indistnodesetsutils.c",0x139,"readIVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(local_88,4,(long)(local_8c + 1),param_1);
  if (local_8c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"indistnodesetsutils.c",0x139,"readIVectorNoNewVec:v",local_8c + 1,
                   sVar1 & 0xffffffff);
  }
  lVar5 = 8;
  iVar4 = 1;
  lVar2 = newIndistNodeSets_vsdlss(local_84);
  if (0 < local_84) {
    do {
      sVar1 = fread(&local_8c,4,1,param_1);
      if ((int)sVar1 != 1) {
        nrerror_vsdlss(0x10,"indistnodesetsutils.c",0x13e,"readIVectorNoNewVec:n",1,
                       sVar1 & 0xffffffff);
      }
      sVar1 = fread(param_2,4,(long)(local_8c + 1),param_1);
      if (local_8c + 1 != (int)sVar1) {
        nrerror_vsdlss(0x10,"indistnodesetsutils.c",0x13e,"readIVectorNoNewVec:v",local_8c + 1,
                       sVar1 & 0xffffffff);
      }
      iVar4 = iVar4 + 1;
      puVar6 = (undefined8 *)(lVar5 + *(long *)(lVar2 + 8));
      lVar5 = lVar5 + 8;
      uVar3 = copyArrayToCEdgelist_vsdlss(param_2,local_8c);
      *puVar6 = uVar3;
    } while (iVar4 <= local_84);
  }
  return lVar2;
}


