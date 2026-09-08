/* VSDLSS function (decompiled by Ghidra) */
/* name: readGraph_vsdlss  addr: 009f4890  size: 2f8 */
#include "vsdlss_ref.h"

/* signature: long readGraph_vsdlss(void); */

long readGraph_vsdlss(void)

{
  long lVar1;
  int *piVar2;
  FILE *__stream;
  size_t sVar3;
  long lVar4;
  void *__ptr;
  undefined8 *puVar5;
  undefined8 uVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  int local_90;
  int local_8c;
  undefined1 local_88 [4];
  int local_84;
  undefined4 local_80;
  int local_7c;
  int local_78;
  int local_74;
  int local_70;
  
  __stream = (FILE *)openFileR_vsdlss(0xb);
  sVar3 = fread(&local_8c,4,1,__stream);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"graphutils.c",99,"readIVectorNoNewVec:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fread(local_88,4,(long)(local_8c + 1),__stream);
  if (local_8c + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"graphutils.c",99,"readIVectorNoNewVec:v",local_8c + 1,sVar3 & 0xffffffff);
  }
  local_90 = local_8c;
  if ((local_70 == 0) || (local_7c != 0)) {
    lVar4 = newGraph_vsdlss(local_84);
    *(undefined4 *)(lVar4 + 4) = local_80;
    if (local_7c != 0) {
      uVar6 = readIVector_vsdlss(__stream,&local_90);
      *(undefined8 *)(lVar4 + 8) = uVar6;
    }
  }
  else {
    nrerror_vsdlss(1,"graphutils.c",0x6e);
    lVar4 = newGraph_vsdlss(local_84);
    *(undefined4 *)(lVar4 + 4) = local_80;
  }
  if (local_78 != 0) {
    uVar6 = readIVector_vsdlss(__stream,&local_90);
    *(undefined8 *)(lVar4 + 0x10) = uVar6;
  }
  if (local_74 != 0) {
    __ptr = (void *)newIVector_vsdlss((long)local_84,0,0);
    iVar9 = 1;
    puVar5 = (undefined8 *)newCEdgeVector_vsdlss(local_84,1,0);
    *(undefined8 **)(lVar4 + 0x18) = puVar5;
    if (0 < local_84) {
      do {
        puVar5 = puVar5 + 1;
        sVar3 = fread(&local_8c,4,1,__stream);
        if ((int)sVar3 != 1) {
          nrerror_vsdlss(0x10,"graphutils.c",0x83,"readIVectorNoNewVec:n",1,sVar3 & 0xffffffff);
        }
        sVar3 = fread(__ptr,4,(long)(local_8c + 1),__stream);
        if (local_8c + 1 != (int)sVar3) {
          nrerror_vsdlss(0x10,"graphutils.c",0x83,"readIVectorNoNewVec:v",local_8c + 1,
                         sVar3 & 0xffffffff);
        }
        iVar9 = iVar9 + 1;
        uVar6 = copyArrayToCEdgelist_vsdlss(__ptr,local_8c);
        *puVar5 = uVar6;
      } while (iVar9 <= local_84);
    }
    if (__ptr != (void *)0x0) {
      casiFree_vsdlss(__ptr);
    }
  }
  if (local_70 != 0) {
    uVar6 = readIVector_vsdlss(__stream,&local_90);
    *(undefined8 *)(lVar4 + 0x20) = uVar6;
    lVar7 = newIPVector_vsdlss((long)local_84,1,0);
    *(long *)(lVar4 + 0x28) = lVar7;
    if ((0 < local_84) &&
       (*(undefined8 *)(lVar7 + 8) = *(undefined8 *)(lVar4 + 0x20), local_84 != 1)) {
      lVar8 = 0x10;
      lVar7 = 4;
      do {
        piVar2 = (int *)(*(long *)(lVar4 + 8) + lVar7);
        lVar1 = lVar7 * 2;
        lVar7 = lVar7 + 4;
        *(long *)(*(long *)(lVar4 + 0x28) + lVar8) =
             *(long *)(*(long *)(lVar4 + 0x28) + lVar1) + (long)*piVar2 * 4;
        lVar8 = lVar8 + 8;
      } while (lVar7 != (ulong)(local_84 - 2) * 4 + 8);
    }
  }
  casiFClose_vsdlss(0xb);
  return lVar4;
}


