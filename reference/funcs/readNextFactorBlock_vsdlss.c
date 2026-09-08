/* VSDLSS function (decompiled by Ghidra) */
/* name: readNextFactorBlock_vsdlss  addr: 009f30b0  size: 317 */
#include "vsdlss_ref.h"

/* signature: void readNextFactorBlock_vsdlss(FILE *param_1,int param_2,long param_3,int *param_4); */

void readNextFactorBlock_vsdlss(FILE *param_1,int param_2,long param_3,int *param_4)

{
  int iVar1;
  long lVar2;
  int iVar3;
  size_t sVar4;
  long lVar5;
  long lVar6;
  int iVar7;
  long lVar8;
  long lVar9;
  undefined8 *puVar10;
  int local_8c;
  undefined1 local_88 [4];
  int local_84;
  
  lVar9 = *(long *)(param_4 + 8);
  lVar8 = *(long *)(param_4 + 6);
  lVar2 = *(long *)(param_4 + 10);
  puVar10 = *(undefined8 **)(param_4 + 0xc);
  sVar4 = fread(&local_8c,4,1,param_1);
  if ((int)sVar4 != 1) {
    nrerror_vsdlss(0x10,"factorchunkutils.c",0x18e,"readIVectorNoNewVec:n",1,sVar4 & 0xffffffff);
  }
  sVar4 = fread(local_88,4,(long)(local_8c + 1),param_1);
  if (local_8c + 1 != (int)sVar4) {
    nrerror_vsdlss(0x10,"factorchunkutils.c",0x18e,"readIVectorNoNewVec:v",local_8c + 1,
                   sVar4 & 0xffffffff);
  }
  if (local_84 != param_2) {
    nrerror_vsdlss(0x24,"factorchunkutils.c",400);
  }
  lVar5 = (long)param_2;
  iVar1 = *(int *)(*(long *)(param_3 + 0x10) + lVar5 * 4);
  iVar7 = *(int *)(*(long *)(param_3 + 8) + lVar5 * 4);
  *param_4 = iVar1;
  param_4[1] = iVar7;
  param_4[2] = iVar1 + -1 + iVar7;
  iVar7 = *(int *)(*(long *)(param_3 + 0x18) + lVar5 * 4);
  param_4[3] = iVar7;
  iVar3 = computeValBuffSize_vsdlss(iVar7,iVar1);
  if (0 < iVar1) {
    lVar5 = 0;
    do {
      *(long *)(lVar2 + 8 + lVar5) = lVar8;
      *(long *)((long)puVar10 + lVar5 + 8) = lVar9;
      lVar6 = ((long)iVar7 * 8 + 8) - lVar5;
      lVar5 = lVar5 + 8;
      lVar8 = lVar8 + 4;
      lVar9 = lVar9 + lVar6;
    } while (lVar5 != (ulong)(iVar1 - 1) * 8 + 8);
  }
  sVar4 = fread(&local_8c,4,1,param_1);
  if ((int)sVar4 != 1) {
    nrerror_vsdlss(0x10,"factorchunkutils.c",0x1aa,"readIVectorNoNewVec:n",1,sVar4 & 0xffffffff);
  }
  sVar4 = fread(*(void **)(lVar2 + 8),4,(long)(local_8c + 1),param_1);
  if (local_8c + 1 != (int)sVar4) {
    nrerror_vsdlss(0x10,"factorchunkutils.c",0x1aa,"readIVectorNoNewVec:v",local_8c + 1,
                   sVar4 & 0xffffffff);
  }
  if (((param_4[4] < iVar7) || (param_4[5] < iVar3)) || (iVar7 != local_8c)) {
    nrerror_vsdlss(0x24,"factorchunkutils.c",0x1af);
  }
  iVar7 = 1;
  if (0 < iVar1) {
    do {
      puVar10 = puVar10 + 1;
      sVar4 = fread(&local_8c,4,1,param_1);
      if ((int)sVar4 != 1) {
        nrerror_vsdlss(0x10,"factorchunkutils.c",0x1b5,"readFVectorNoNewVec:n",1,sVar4 & 0xffffffff)
        ;
      }
      sVar4 = fread((void *)*puVar10,8,(long)(local_8c + 1),param_1);
      if (local_8c + 1 != (int)sVar4) {
        nrerror_vsdlss(0x10,"factorchunkutils.c",0x1b5,"readFVectorNoNewVec:v",local_8c + 1,
                       sVar4 & 0xffffffff);
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 <= iVar1);
  }
  return;
}


