/* VSDLSS function (decompiled by Ghidra) */
/* name: solveUpperTriangPartFact_vsdlss  addr: 00a08720  size: 28c */
#include "vsdlss_ref.h"

/* signature: void solveUpperTriangPartFact_vsdlss(char *param_1,int param_2,long param_3,undefined8 param_4); */

void solveUpperTriangPartFact_vsdlss(char *param_1,int param_2,long param_3,undefined8 param_4)

{
  int iVar1;
  long lVar2;
  int *piVar3;
  double dVar4;
  int iVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  uint uVar11;
  uint uVar12;
  undefined8 *puVar13;
  long lVar14;
  undefined4 *puVar15;
  long lVar16;
  uint *puVar17;
  uint *puVar18;
  double *pdVar19;
  long lVar20;
  double dVar21;
  uint local_c38 [256];
  uint local_838 [256];
  uint local_438 [258];
  
  lVar7 = *(long *)(param_3 + 0x10);
  iVar5 = *(int *)(param_3 + 4);
  lVar8 = *(long *)(param_3 + 8);
  iVar1 = iVar5 + param_2;
  lVar9 = *(long *)(param_3 + 0x18);
  puVar13 = (undefined8 *)newVector_vsdlss(0,(long)iVar1,0);
  *puVar13 = 0;
  strcpy((char *)local_c38,param_1);
  puVar18 = local_c38;
  do {
    puVar17 = puVar18;
    uVar11 = *puVar17 + 0xfefefeff & ~*puVar17;
    uVar12 = uVar11 & 0x80808080;
    puVar18 = puVar17 + 1;
  } while (uVar12 == 0);
  if ((uVar11 & 0x8080) == 0) {
    puVar18 = (uint *)((long)puVar17 + 6);
    uVar12 = uVar12 >> 0x10;
  }
  puVar15 = (undefined4 *)((long)puVar18 + (-3 - (ulong)CARRY1((byte)uVar12,(byte)uVar12)));
  *(undefined2 *)(puVar15 + 1) = 0x6c;
  *puVar15 = 0x6f73692e;
  readVecFromFile_vsdlss(local_c38,iVar5,puVar13);
  strcpy((char *)local_838,param_1);
  puVar18 = local_838;
  do {
    puVar17 = puVar18;
    uVar11 = *puVar17 + 0xfefefeff & ~*puVar17;
    uVar12 = uVar11 & 0x80808080;
    puVar18 = puVar17 + 1;
  } while (uVar12 == 0);
  lVar20 = (long)iVar5;
  if ((uVar11 & 0x8080) == 0) {
    puVar18 = (uint *)((long)puVar17 + 6);
    uVar12 = uVar12 >> 0x10;
  }
  puVar15 = (undefined4 *)((long)puVar18 + (-3 - (ulong)CARRY1((byte)uVar12,(byte)uVar12)));
  *(undefined2 *)(puVar15 + 1) = 0x7674;
  *puVar15 = 0x726f702e;
  *(undefined1 *)((long)puVar15 + 6) = 0;
  readVecFromFile_vsdlss(local_838,param_2,puVar13 + lVar20);
  if (0 < iVar5) {
    lVar16 = 0;
    pdVar19 = (double *)(puVar13 + lVar20);
    do {
      iVar6 = *(int *)(lVar8 + lVar20 * 4 + lVar16);
      lVar10 = *(long *)(lVar9 + lVar20 * 8 + lVar16 * 2);
      dVar21 = 0.0;
      if (1 < iVar6) {
        dVar21 = 0.0;
        lVar14 = 0;
        do {
          piVar3 = (int *)(*(long *)(lVar7 + lVar20 * 8 + lVar16 * 2) + 8 + lVar14);
          lVar2 = lVar14 * 2;
          lVar14 = lVar14 + 4;
          dVar21 = dVar21 + (double)puVar13[*piVar3] * *(double *)(lVar10 + 0x10 + lVar2);
        } while (lVar14 != (ulong)(iVar6 - 2) * 4 + 4);
      }
      dVar4 = *pdVar19;
      lVar16 = lVar16 + -4;
      *pdVar19 = dVar4 - dVar21;
      *pdVar19 = (dVar4 - dVar21) / *(double *)(lVar10 + 8);
      pdVar19 = pdVar19 + -1;
    } while (lVar16 != ~(ulong)(iVar5 - 1) * 4);
  }
  permuteDVectorInPlace_vsdlss(param_4,iVar1,puVar13);
  strcpy((char *)local_438,param_1);
  puVar18 = local_438;
  do {
    puVar17 = puVar18;
    uVar11 = *puVar17 + 0xfefefeff & ~*puVar17;
    uVar12 = uVar11 & 0x80808080;
    puVar18 = puVar17 + 1;
  } while (uVar12 == 0);
  if ((uVar11 & 0x8080) == 0) {
    puVar18 = (uint *)((long)puVar17 + 6);
    uVar12 = uVar12 >> 0x10;
  }
  puVar15 = (undefined4 *)((long)puVar18 + (-3 - (ulong)CARRY1((byte)uVar12,(byte)uVar12)));
  *puVar15 = 0x6c73722e;
  *(undefined1 *)(puVar15 + 1) = 0;
  writeVecToFile_vsdlss(local_438,iVar1,puVar13);
  casiFree_vsdlss(puVar13);
  return;
}


