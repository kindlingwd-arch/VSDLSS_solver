/* VSDLSS function (decompiled by Ghidra) */
/* name: renumWriteRedMat_vsdlss  addr: 00a1a480  size: 2ea */
#include "vsdlss_ref.h"

/* signature: void renumWriteRedMat_vsdlss(int param_1,long param_2); */

void renumWriteRedMat_vsdlss(int param_1,long param_2)

{
  int iVar1;
  undefined4 *puVar2;
  undefined8 uVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  int *piVar11;
  FILE *__s;
  size_t sVar12;
  undefined8 *puVar13;
  undefined4 *puVar14;
  undefined4 *puVar15;
  undefined8 *puVar16;
  long lVar17;
  long lVar18;
  int iVar19;
  long lVar20;
  int iVar21;
  int local_4c [4];
  int local_3c [3];
  
  iVar4 = *(int *)(param_2 + 8);
  lVar6 = *(long *)(param_2 + 0x38);
  lVar7 = *(long *)(param_2 + 0x30);
  lVar8 = *(long *)(param_2 + 0x28);
  local_3c[0] = 0;
  iVar21 = iVar4 - param_1;
  iVar1 = iVar21 + 1;
  if (iVar1 <= iVar4) {
    lVar17 = 0;
    do {
      iVar19 = *(int *)(lVar8 + (long)iVar1 * 4 + lVar17);
      lVar20 = *(long *)(lVar7 + (long)iVar1 * 8 + lVar17 * 2);
      *(int *)(lVar20 + 4) = *(int *)(lVar20 + 4) - iVar21;
      if (1 < iVar19) {
        piVar11 = (int *)(lVar20 + 8);
        do {
          local_3c[0] = local_3c[0] + 1;
          *piVar11 = *piVar11 - iVar21;
          piVar11 = piVar11 + 1;
        } while (piVar11 != (int *)(lVar20 + 0xc + (ulong)(iVar19 - 2) * 4));
      }
      lVar17 = lVar17 + 4;
    } while (lVar17 != (ulong)(uint)((iVar4 + -1) - iVar21) * 4 + 4);
  }
  local_4c[0] = param_1;
  __s = (FILE *)openFileW_vsdlss(0x20);
  sVar12 = fwrite(local_4c,4,1,__s);
  if ((int)sVar12 != 1) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xaea,"renumWriteRedMat:WSize",1,sVar12 & 0xffffffff);
  }
  sVar12 = fwrite(local_3c,4,1,__s);
  if ((int)sVar12 != 1) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xaec,"renumWriteRedMat:nOffdiagsRed",1,
                   sVar12 & 0xffffffff);
  }
  casiFClose_vsdlss(0x20);
  puVar13 = (undefined8 *)newVector_vsdlss(0,(long)local_4c[0],0);
  *puVar13 = 0;
  puVar14 = (undefined4 *)newIVector_vsdlss((long)local_3c[0],0,0);
  *puVar14 = 0;
  puVar15 = (undefined4 *)newIVector_vsdlss((long)local_3c[0],0,0);
  *puVar15 = 0;
  puVar16 = (undefined8 *)newVector_vsdlss(0,(long)local_3c[0],0);
  *puVar16 = 0;
  local_3c[0] = 0;
  if (iVar1 <= iVar4) {
    lVar17 = (long)iVar1 * 8;
    lVar20 = 0;
    iVar19 = 1;
    do {
      lVar9 = *(long *)(lVar6 + lVar17 + lVar20 * 2);
      iVar5 = *(int *)(lVar8 + (long)iVar1 * 4 + lVar20);
      lVar10 = *(long *)(lVar7 + lVar17 + lVar20 * 2);
      *(undefined8 *)((long)puVar13 + lVar20 * 2 + lVar17 + (long)iVar21 * -8) =
           *(undefined8 *)(lVar9 + 8);
      if (1 < iVar5) {
        lVar18 = 0;
        do {
          local_3c[0] = local_3c[0] + 1;
          uVar3 = *(undefined8 *)(lVar9 + 0x10 + lVar18 * 2);
          puVar14[local_3c[0]] = iVar19;
          puVar2 = (undefined4 *)(lVar10 + 8 + lVar18);
          lVar18 = lVar18 + 4;
          puVar15[local_3c[0]] = *puVar2;
          puVar16[local_3c[0]] = uVar3;
        } while (lVar18 != (ulong)(iVar5 - 2) * 4 + 4);
      }
      lVar20 = lVar20 + 4;
      iVar19 = iVar19 + 1;
    } while (iVar19 != (iVar4 + 1) - iVar21);
  }
  writeDVectorFNum_vsdlss(0x21,local_4c[0],puVar13);
  casiFree_vsdlss(puVar13);
  writeIVectorFNum_vsdlss(0x22,local_3c[0],puVar14);
  casiFree_vsdlss(puVar14);
  writeIVectorFNum_vsdlss(0x23,local_3c[0],puVar15);
  casiFree_vsdlss(puVar15);
  writeDVectorFNum_vsdlss(0x24,local_3c[0],puVar16);
  casiFree_vsdlss(puVar16);
  return;
}


