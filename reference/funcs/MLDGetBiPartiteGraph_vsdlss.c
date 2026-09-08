/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDGetBiPartiteGraph_vsdlss  addr: 00a00e70  size: 37a */
#include "vsdlss_ref.h"

/* signature: long MLDGetBiPartiteGraph_vsdlss(undefined4 param_1,long param_2,long param_3,long param_4,long param_5); */

long MLDGetBiPartiteGraph_vsdlss
               (undefined4 param_1,long param_2,long param_3,long param_4,long param_5)

{
  int iVar1;
  undefined4 uVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  int *piVar9;
  long lVar10;
  undefined8 uVar11;
  long lVar12;
  long lVar13;
  int *piVar14;
  int iVar15;
  long lVar16;
  int *piVar17;
  uint uVar18;
  int iVar19;
  int iVar20;
  int iVar21;
  long local_80;
  long local_68;
  
  iVar19 = *(int *)(param_2 + 8);
  lVar3 = *(long *)(param_3 + 0xb0);
  lVar4 = *(long *)(param_2 + 0x38);
  lVar5 = *(long *)(param_3 + 0xa0);
  lVar6 = *(long *)(param_3 + 0x80);
  iVar1 = *(int *)(param_2 + 0x10);
  lVar7 = *(long *)(param_2 + 0x28);
  *(undefined4 *)(param_4 + 4) = 0;
  *(undefined4 *)(param_4 + 8) = 0;
  if (iVar1 < 1) {
    local_68 = 0;
    local_80 = 0;
    iVar21 = 0;
  }
  else {
    iVar20 = 0;
    piVar9 = (int *)(lVar3 + 4);
    do {
      iVar21 = *piVar9;
      piVar9 = piVar9 + 1;
      piVar17 = (int *)(param_4 + (long)*(int *)(lVar5 + (long)iVar21 * 4) * 4);
      *piVar17 = *piVar17 + 1;
      iVar20 = iVar20 + *(int *)(lVar7 + (long)iVar21 * 4);
    } while (piVar9 != (int *)(lVar3 + 8 + (ulong)(iVar1 - 1) * 4));
    iVar21 = *(int *)(param_4 + 4) + *(int *)(param_4 + 8);
    local_80 = (long)iVar21;
    local_68 = (long)(int)(iVar20 - (iVar20 >> 0x1f) & 0xfffffffe);
  }
  if (iVar1 != iVar21) {
    nrerror_vsdlss(1,"mldorder.c",0xc14);
  }
  lVar10 = tagNewMLDGraphHeaderOnly_vsdlss(param_1);
  piVar9 = (int *)tagNewIVector_vsdlss(param_1,local_80,1,0);
  *(int **)(lVar10 + 0x28) = piVar9;
  *(int *)(lVar10 + 8) = iVar21;
  uVar2 = *(undefined4 *)(param_4 + 4);
  *(undefined4 *)(param_4 + 4) = 0;
  *(undefined4 *)(param_4 + 8) = uVar2;
  initIVector_vsdlss(lVar6,(long)iVar19,0);
  initIVector_vsdlss(param_5,(long)iVar1,0);
  if (0 < iVar1) {
    piVar17 = (int *)(lVar3 + 4);
    do {
      iVar19 = *piVar17;
      piVar17 = piVar17 + 1;
      lVar16 = (long)iVar19;
      piVar14 = (int *)(param_4 + (long)*(int *)(lVar5 + lVar16 * 4) * 4);
      iVar20 = *piVar14;
      *piVar14 = iVar20 + 1;
      *(int *)(lVar6 + lVar16 * 4) = iVar20 + 1;
      *(int *)(param_5 + (long)*piVar14 * 4) = iVar19;
      piVar9[*piVar14] = *(int *)(lVar7 + lVar16 * 4);
    } while (piVar17 != (int *)(lVar3 + 8 + (ulong)(iVar1 - 1) * 4));
  }
  if (*(int *)(param_4 + 8) != iVar21) {
    nrerror_vsdlss(1,"mldorder.c",0xc2e);
  }
  uVar11 = tagNewIPVector_vsdlss(param_1,local_80,1,0);
  *(undefined8 *)(lVar10 + 0x38) = uVar11;
  uVar11 = tagNewIVector_vsdlss(param_1,local_68,0,0);
  *(undefined8 *)(lVar10 + 0x40) = uVar11;
  *(undefined8 *)(*(long *)(lVar10 + 0x38) + 8) = uVar11;
  if (1 < iVar21) {
    lVar16 = 0x10;
    piVar17 = piVar9;
    do {
      piVar17 = piVar17 + 1;
      *(long *)(*(long *)(lVar10 + 0x38) + lVar16) =
           *(long *)(*(long *)(lVar10 + 0x38) + -8 + lVar16) + (long)*piVar17 * 4;
      lVar16 = lVar16 + 8;
    } while (lVar16 != (ulong)(iVar21 - 2) * 8 + 0x18);
  }
  initIVector_vsdlss(piVar9,local_80,0);
  if (iVar1 < 1) {
    iVar19 = 0;
  }
  else {
    lVar16 = *(long *)(lVar10 + 0x38);
    uVar18 = 0;
    piVar17 = (int *)(lVar3 + 4);
    do {
      lVar12 = (long)*piVar17;
      iVar19 = *(int *)(lVar7 + lVar12 * 4);
      lVar8 = *(long *)(lVar4 + lVar12 * 8);
      iVar21 = *(int *)(lVar5 + lVar12 * 4);
      lVar13 = (long)*(int *)(lVar6 + lVar12 * 4);
      lVar12 = *(long *)(lVar16 + lVar13 * 8);
      if (0 < iVar19) {
        piVar14 = (int *)(lVar8 + 4);
        do {
          iVar20 = *piVar14;
          if (*(int *)(lVar5 + (long)iVar20 * 4) != iVar21) {
            uVar18 = uVar18 + 1;
            iVar15 = piVar9[lVar13] + 1;
            piVar9[lVar13] = iVar15;
            *(undefined4 *)(lVar12 + (long)iVar15 * 4) = *(undefined4 *)(lVar6 + (long)iVar20 * 4);
          }
          piVar14 = piVar14 + 1;
        } while (piVar14 != (int *)(lVar8 + 8 + (ulong)(iVar19 - 1) * 4));
      }
      piVar17 = piVar17 + 1;
    } while (piVar17 != (int *)(lVar3 + 8 + (ulong)(iVar1 - 1) * 4));
    if ((uVar18 & 1) != 0) {
      nrerror_vsdlss(1,"mldorder.c",0xc52);
    }
    iVar19 = (int)uVar18 >> 1;
  }
  *(int *)(lVar10 + 0xc) = iVar19;
  return lVar10;
}


