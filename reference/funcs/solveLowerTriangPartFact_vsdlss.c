/* VSDLSS function (decompiled by Ghidra) */
/* name: solveLowerTriangPartFact_vsdlss  addr: 00a084c0  size: 256 */
#include "vsdlss_ref.h"

/* signature: void solveLowerTriangPartFact_vsdlss(char *param_1,int param_2,long param_3,undefined8 param_4); */

void solveLowerTriangPartFact_vsdlss(char *param_1,int param_2,long param_3,undefined8 param_4)

{
  long lVar1;
  int *piVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  ulong uVar9;
  uint uVar10;
  uint uVar11;
  undefined8 *puVar12;
  long lVar13;
  undefined4 *puVar14;
  uint *puVar15;
  uint *puVar16;
  int iVar17;
  long lVar18;
  double dVar19;
  uint local_c38 [256];
  uint local_838 [256];
  uint local_438 [258];
  
  iVar3 = *(int *)(param_3 + 4);
  lVar4 = *(long *)(param_3 + 8);
  lVar5 = *(long *)(param_3 + 0x10);
  lVar6 = *(long *)(param_3 + 0x18);
  iVar17 = iVar3 + param_2;
  puVar12 = (undefined8 *)newVector_vsdlss(0,(long)iVar17,0);
  *puVar12 = 0;
  strcpy((char *)local_c38,param_1);
  puVar16 = local_c38;
  do {
    puVar15 = puVar16;
    uVar10 = *puVar15 + 0xfefefeff & ~*puVar15;
    uVar11 = uVar10 & 0x80808080;
    puVar16 = puVar15 + 1;
  } while (uVar11 == 0);
  if ((uVar10 & 0x8080) == 0) {
    puVar16 = (uint *)((long)puVar15 + 6);
    uVar11 = uVar11 >> 0x10;
  }
  puVar14 = (undefined4 *)((long)puVar16 + (-3 - (ulong)CARRY1((byte)uVar11,(byte)uVar11)));
  *puVar14 = 0x7368722e;
  *(undefined1 *)(puVar14 + 1) = 0;
  readVecFromFile_vsdlss(local_c38,iVar17,puVar12);
  invPermuteDVectorInPlace_vsdlss(param_4,iVar17,puVar12);
  uVar9 = DAT_00abd200;
  if (0 < iVar3) {
    lVar18 = 0;
    do {
      lVar7 = *(long *)(lVar6 + 8 + lVar18 * 2);
      iVar17 = *(int *)(lVar4 + 4 + lVar18);
      lVar8 = *(long *)(lVar5 + 8 + lVar18 * 2);
      dVar19 = *(double *)((long)puVar12 + lVar18 * 2 + 8) / *(double *)(lVar7 + 8);
      *(double *)((long)puVar12 + lVar18 * 2 + 8) = dVar19;
      if (1 < iVar17) {
        lVar13 = 0;
        do {
          piVar2 = (int *)(lVar8 + 8 + lVar13);
          lVar1 = lVar13 * 2;
          lVar13 = lVar13 + 4;
          puVar12[*piVar2] =
               *(double *)(lVar7 + 0x10 + lVar1) * (double)((ulong)dVar19 ^ uVar9) +
               (double)puVar12[*piVar2];
        } while (lVar13 != (ulong)(iVar17 - 2) * 4 + 4);
      }
      lVar18 = lVar18 + 4;
    } while (lVar18 != (ulong)(iVar3 - 1) * 4 + 4);
  }
  strcpy((char *)local_838,param_1);
  puVar16 = local_838;
  do {
    puVar15 = puVar16;
    uVar10 = *puVar15 + 0xfefefeff & ~*puVar15;
    uVar11 = uVar10 & 0x80808080;
    puVar16 = puVar15 + 1;
  } while (uVar11 == 0);
  if ((uVar10 & 0x8080) == 0) {
    puVar16 = (uint *)((long)puVar15 + 6);
    uVar11 = uVar11 >> 0x10;
  }
  puVar14 = (undefined4 *)((long)puVar16 + (-3 - (ulong)CARRY1((byte)uVar11,(byte)uVar11)));
  *(undefined2 *)(puVar14 + 1) = 0x6c;
  *puVar14 = 0x6f73692e;
  writeVecToFile_vsdlss(local_838,iVar3,puVar12);
  strcpy((char *)local_438,param_1);
  puVar16 = local_438;
  do {
    puVar15 = puVar16;
    uVar10 = *puVar15 + 0xfefefeff & ~*puVar15;
    uVar11 = uVar10 & 0x80808080;
    puVar16 = puVar15 + 1;
  } while (uVar11 == 0);
  if ((uVar10 & 0x8080) == 0) {
    puVar16 = (uint *)((long)puVar15 + 6);
    uVar11 = uVar11 >> 0x10;
  }
  puVar14 = (undefined4 *)((long)puVar16 + (-3 - (ulong)CARRY1((byte)uVar11,(byte)uVar11)));
  *(undefined2 *)(puVar14 + 1) = 0x72;
  *puVar14 = 0x6465722e;
  writeVecToFile_vsdlss(local_438,param_2,puVar12 + iVar3);
  casiFree_vsdlss(puVar12);
  return;
}


