/* VSDLSS function (decompiled by Ghidra) */
/* name: readAndSolvePartDataPartFact_vsdlss  addr: 00a08b40  size: 14e */
#include "vsdlss_ref.h"

/* signature: void readAndSolvePartDataPartFact_vsdlss(char *param_1,int param_2,int param_3); */

void readAndSolvePartDataPartFact_vsdlss(char *param_1,int param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint *puVar4;
  uint *puVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  uint local_838 [256];
  uint local_438 [258];
  
  strcpy((char *)local_838,param_1);
  puVar5 = local_838;
  do {
    puVar4 = puVar5;
    uVar2 = *puVar4 + 0xfefefeff & ~*puVar4;
    uVar3 = uVar2 & 0x80808080;
    puVar5 = puVar4 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar5 = (uint *)((long)puVar4 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar6 = (undefined4 *)((long)puVar5 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar6 = 0x7261702e;
  *(undefined2 *)(puVar6 + 1) = 0x66;
  puVar6 = (undefined4 *)readPartFact_vsdlss(local_838);
  iVar1 = puVar6[1];
  strcpy((char *)local_438,param_1);
  puVar5 = local_438;
  do {
    puVar4 = puVar5;
    uVar2 = *puVar4 + 0xfefefeff & ~*puVar4;
    uVar3 = uVar2 & 0x80808080;
    puVar5 = puVar4 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar5 = (uint *)((long)puVar4 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar7 = (undefined4 *)((long)puVar5 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar7 = 0x7265702e;
  *(undefined2 *)(puVar7 + 1) = 0x6d;
  puVar7 = (undefined4 *)newIVector_vsdlss((long)(iVar1 + param_2),0,0,uVar3 & 0xffffff00);
  *puVar7 = 0;
  readIVecFromFile_vsdlss(local_438,iVar1 + param_2,puVar7);
  if (param_3 == 0) {
    solveLowerTriangPartFact_vsdlss(param_1,param_2,puVar6,puVar7);
  }
  else if (param_3 == 1) {
    solveUpperTriangPartFact_vsdlss(param_1,param_2,puVar6,puVar7);
  }
  deleteTag_vsdlss(*puVar6);
  casiFree_vsdlss(puVar7);
  return;
}


