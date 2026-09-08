/* VSDLSS function (decompiled by Ghidra) */
/* name: writeGraph_vsdlss  addr: 009f4440  size: 431 */
#include "vsdlss_ref.h"

/* signature: void writeGraph_vsdlss(int *param_1); */

void writeGraph_vsdlss(int *param_1)

{
  int iVar1;
  long lVar2;
  long lVar3;
  FILE *__s;
  size_t sVar4;
  void *__ptr;
  int iVar5;
  undefined8 *puVar6;
  uint uVar7;
  bool bVar8;
  int local_8c;
  undefined1 local_88 [4];
  int local_84;
  int local_80;
  uint local_7c;
  uint local_78;
  uint local_74;
  uint local_70;
  
  __s = (FILE *)openFileW_vsdlss(0xb);
  lVar2 = *(long *)(param_1 + 2);
  puVar6 = *(undefined8 **)(param_1 + 6);
  iVar1 = *param_1;
  lVar3 = *(long *)(param_1 + 4);
  bVar8 = *(long *)(param_1 + 10) != 0;
  if ((lVar2 == 0 && bVar8) || (local_84 = iVar1, bVar8 != (*(long *)(param_1 + 8) != 0))) {
    nrerror_vsdlss(1,"graphutils.c",0x22);
    local_84 = *param_1;
  }
  local_80 = param_1[1];
  uVar7 = (uint)(lVar2 != 0);
  local_8c = 6;
  local_7c = uVar7;
  local_78 = (uint)(lVar3 != 0);
  local_74 = (uint)(puVar6 != (undefined8 *)0x0);
  local_70 = (uint)bVar8;
  sVar4 = fwrite(&local_8c,4,1,__s);
  if ((int)sVar4 != 1) {
    nrerror_vsdlss(0x10,"graphutils.c",0x2e,"writeIVector:n",1,sVar4 & 0xffffffff);
  }
  sVar4 = fwrite(local_88,4,(long)(local_8c + 1),__s);
  if (local_8c + 1 != (int)sVar4) {
    nrerror_vsdlss(0x10,"graphutils.c",0x2e,"writeIVector:v",local_8c + 1,sVar4 & 0xffffffff);
  }
  if (uVar7 != 0) {
    local_8c = iVar1;
    sVar4 = fwrite(&local_8c,4,1,__s);
    if ((int)sVar4 != 1) {
      nrerror_vsdlss(0x10,"graphutils.c",0x31,"writeIVector:n",1,sVar4 & 0xffffffff);
    }
    sVar4 = fwrite(*(void **)(param_1 + 2),4,(long)(local_8c + 1),__s);
    if (local_8c + 1 != (int)sVar4) {
      nrerror_vsdlss(0x10,"graphutils.c",0x31,"writeIVector:v",local_8c + 1,sVar4 & 0xffffffff);
    }
  }
  if ((lVar3 != 0) != 0) {
    local_8c = iVar1;
    sVar4 = fwrite(&local_8c,4,1,__s);
    if ((int)sVar4 != 1) {
      nrerror_vsdlss(0x10,"graphutils.c",0x35,"writeIVector:n",1,sVar4 & 0xffffffff);
    }
    sVar4 = fwrite(*(void **)(param_1 + 4),4,(long)(local_8c + 1),__s);
    if (local_8c + 1 != (int)sVar4) {
      nrerror_vsdlss(0x10,"graphutils.c",0x35,"writeIVector:v",local_8c + 1,sVar4 & 0xffffffff);
    }
  }
  if ((puVar6 != (undefined8 *)0x0) != 0) {
    __ptr = (void *)newIVector_vsdlss((long)iVar1,0,0);
    iVar5 = 1;
    if (0 < iVar1) {
      do {
        puVar6 = puVar6 + 1;
        local_8c = copyCEdgelistToArray_vsdlss(*puVar6,__ptr);
        sVar4 = fwrite(&local_8c,4,1,__s);
        if ((int)sVar4 != 1) {
          nrerror_vsdlss(0x10,"graphutils.c",0x3c,"writeIVector:n",1,sVar4 & 0xffffffff);
        }
        sVar4 = fwrite(__ptr,4,(long)(local_8c + 1),__s);
        if (local_8c + 1 != (int)sVar4) {
          nrerror_vsdlss(0x10,"graphutils.c",0x3c,"writeIVector:v",local_8c + 1,sVar4 & 0xffffffff);
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 <= iVar1);
    }
    if (__ptr != (void *)0x0) {
      casiFree_vsdlss(__ptr);
    }
  }
  if (bVar8 != 0) {
    local_8c = param_1[1] * 2;
    sVar4 = fwrite(&local_8c,4,1,__s);
    if ((int)sVar4 != 1) {
      nrerror_vsdlss(0x10,"graphutils.c",0x42,"writeIVector:n",1,sVar4 & 0xffffffff);
    }
    sVar4 = fwrite(*(void **)(param_1 + 8),4,(long)(local_8c + 1),__s);
    if (local_8c + 1 != (int)sVar4) {
      nrerror_vsdlss(0x10,"graphutils.c",0x42,"writeIVector:v",local_8c + 1,sVar4 & 0xffffffff);
    }
  }
  casiFClose_vsdlss(0xb);
  return;
}


