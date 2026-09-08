/* driver: OByAlBUQdy_QLOZIL_HXSPAIPP addr=00986880 size=30c */
#include "vsdlss_ref.h"


void OByAlBUQdy_QLOZIL_HXSPAIPP(FILE *param_1,long param_2,long param_3,long *param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  size_t sVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long local_d0;
  undefined1 local_c8 [8];
  long local_c0;
  
  lVar10 = param_4[7];
  lVar9 = param_4[6];
  lVar1 = param_4[8];
  lVar2 = param_4[9];
  sVar5 = fread(&local_d0,8,1,param_1);
  if (sVar5 != 1) {
    KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x18e,"readIVectorNoNewVec:n",1,sVar5);
  }
  sVar5 = fread(local_c8,8,local_d0 + 1,param_1);
  if (local_d0 + 1U != sVar5) {
    KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x18e,"readIVectorNoNewVec:v",local_d0 + 1U,sVar5);
  }
  if (local_c0 != param_2) {
    KOBOOLOXSPAIPP(0x24,"factorchunkutils.c",400);
  }
  lVar3 = *(long *)(*(long *)(param_3 + 0x18) + param_2 * 8);
  lVar4 = *(long *)(*(long *)(param_3 + 0x10) + param_2 * 8);
  *param_4 = lVar3;
  param_4[1] = lVar4;
  param_4[2] = lVar3 + -1 + lVar4;
  lVar4 = *(long *)(*(long *)(param_3 + 0x20) + param_2 * 8);
  param_4[3] = lVar4;
  lVar6 = _LJMRQBtyIZRCCqFWBXSPAIPP(lVar4,lVar3);
  if (0 < lVar3) {
    lVar7 = 0;
    do {
      *(long *)(lVar1 + 8 + lVar7) = lVar9;
      *(long *)(lVar2 + 8 + lVar7) = lVar10;
      lVar8 = (lVar4 * 8 + 8) - lVar7;
      lVar7 = lVar7 + 8;
      lVar9 = lVar9 + 8;
      lVar10 = lVar10 + lVar8;
    } while (lVar7 != lVar3 * 8);
  }
  sVar5 = fread(&local_d0,8,1,param_1);
  if (sVar5 != 1) {
    KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x1aa,"readIVectorNoNewVec:n",1,sVar5);
  }
  sVar5 = fread(*(void **)(lVar1 + 8),8,local_d0 + 1,param_1);
  if (local_d0 + 1U != sVar5) {
    KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x1aa,"readIVectorNoNewVec:v",local_d0 + 1U,sVar5);
  }
  if (((param_4[4] < lVar4) || (param_4[5] < lVar6)) || (lVar4 != local_d0)) {
    KOBOOLOXSPAIPP(0x24,"factorchunkutils.c",0x1af);
  }
  lVar10 = 1;
  if (0 < lVar3) {
    do {
      sVar5 = fread(&local_d0,8,1,param_1);
      if (sVar5 != 1) {
        KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x1b5,"readFVectorNoNewVec:n",1,sVar5);
      }
      sVar5 = fread(*(void **)(lVar2 + lVar10 * 8),8,local_d0 + 1,param_1);
      if (local_d0 + 1U != sVar5) {
        KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x1b5,"readFVectorNoNewVec:v",local_d0 + 1U,sVar5);
      }
      lVar10 = lVar10 + 1;
    } while (lVar10 <= lVar3);
  }
  return;
}


