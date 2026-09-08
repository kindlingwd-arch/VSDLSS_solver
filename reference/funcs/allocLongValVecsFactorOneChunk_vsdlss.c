/* VSDLSS function (decompiled by Ghidra) */
/* name: allocLongValVecsFactorOneChunk_vsdlss  addr: 00a15f00  size: 1e0 */
#include "vsdlss_ref.h"

/* signature: void allocLongValVecsFactorOneChunk_vsdlss(undefined4 *param_1); */

void allocLongValVecsFactorOneChunk_vsdlss(undefined4 *param_1)

{
  undefined4 uVar1;
  int iVar2;
  long lVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  int iVar8;
  ulong uVar9;
  long *plVar10;
  int iVar11;
  int iVar12;
  ulong uVar13;
  int iVar14;
  int *piVar15;
  int *piVar16;
  int iVar17;
  long lVar18;
  int iVar19;
  
  uVar1 = *param_1;
  iVar17 = param_1[2];
  piVar15 = *(int **)(param_1 + 8);
  iVar2 = param_1[5];
  lVar3 = *(long *)(param_1 + 10);
  iVar4 = iVar17 >> 10;
  if (iVar17 < 0x400) {
    iVar4 = 1;
  }
  lVar5 = tagNewFPVector_vsdlss(uVar1,(long)iVar17,1,0);
  *(long *)(param_1 + 0xe) = lVar5;
  if (0 < iVar2) {
    iVar19 = 0;
    iVar8 = 1;
    lVar6 = 0;
    iVar17 = 1;
    while( true ) {
      piVar15 = piVar15 + 1;
      iVar12 = *piVar15;
      lVar18 = (long)iVar17;
      piVar16 = (int *)(lVar3 + lVar18 * 4);
      iVar14 = *piVar16;
      iVar11 = iVar12 * iVar14 - ((iVar12 + -1) * iVar12) / 2;
      lVar7 = lVar6;
      if (iVar19 < iVar11) {
        iVar14 = iVar11 + 1;
        if (iVar11 < iVar4) {
          iVar14 = iVar4;
        }
        iVar19 = iVar14 + -1;
        lVar7 = newBlock_vsdlss(uVar1,(long)iVar14 << 3);
        iVar14 = *piVar16;
      }
      uVar13 = (ulong)iVar11;
      iVar19 = iVar19 - iVar11;
      uVar9 = 1;
      lVar6 = lVar7 + uVar13 * 8;
      if (uVar13 != 0) {
        do {
          *(undefined8 *)(lVar7 + uVar9 * 8) = 0;
          uVar9 = uVar9 + 1;
        } while (uVar9 <= uVar13);
      }
      *(long *)(lVar5 + lVar18 * 8) = lVar7;
      iVar12 = iVar12 + iVar17;
      iVar11 = iVar11 - iVar14;
      if (iVar17 + 1 < iVar12) {
        plVar10 = (long *)(lVar18 * 8 + lVar5);
        do {
          iVar14 = *piVar16;
          lVar7 = *plVar10;
          plVar10 = plVar10 + 1;
          iVar11 = iVar11 - piVar16[1];
          piVar16 = piVar16 + 1;
          *plVar10 = lVar7 + (long)iVar14 * 8;
        } while (plVar10 !=
                 (long *)(lVar5 + ((long)(iVar17 + 1) + (ulong)(uint)((-2 - iVar17) + iVar12)) * 8))
        ;
      }
      if (iVar11 != 0) {
        nrerror_vsdlss(1,"vsdlss1memutils.c",0x249);
      }
      if (iVar2 < iVar8 + 1) break;
      iVar8 = iVar8 + 1;
      iVar17 = iVar12;
    }
  }
  return;
}


