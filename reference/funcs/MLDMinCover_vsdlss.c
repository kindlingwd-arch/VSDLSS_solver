/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDMinCover_vsdlss  addr: 00a01770  size: 532 */
#include "vsdlss_ref.h"

/* signature: undefined4 MLDMinCover_vsdlss(long param_1,long param_2,long param_3,undefined8 param_4); */

undefined4 MLDMinCover_vsdlss(long param_1,long param_2,long param_3,undefined8 param_4)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  undefined4 *puVar15;
  long lVar16;
  int iVar17;
  undefined4 uVar18;
  long lVar19;
  long lVar20;
  int *piVar21;
  int iVar22;
  int iVar23;
  undefined4 *puVar24;
  long lVar25;
  int iVar26;
  int iVar27;
  long lVar28;
  
  iVar2 = *(int *)(param_1 + 8);
  iVar3 = *(int *)(param_1 + 4);
  lVar9 = *(long *)(param_2 + 0x28);
  lVar10 = *(long *)(param_3 + 0x30);
  *(undefined8 *)(lVar10 + 0x10) = *(undefined8 *)(param_3 + 0x60);
  *(int *)(lVar10 + 8) = iVar2;
  initQ_vsdlss();
  lVar11 = *(long *)(lVar10 + 0x10);
  iVar4 = *(int *)(lVar10 + 8);
  lVar12 = *(long *)(param_3 + 0x48);
  *(undefined8 *)(param_3 + 0x48) = 0;
  lVar13 = *(long *)(param_3 + 0x68);
  lVar14 = *(long *)(param_3 + 0x70);
  *(undefined8 *)(param_3 + 0x68) = 0;
  *(undefined8 *)(param_3 + 0x70) = 0;
  puVar15 = *(undefined4 **)(param_3 + 0x78);
  *(undefined8 *)(param_3 + 0x78) = 0;
  lVar16 = *(long *)(param_3 + 0x58);
  *(undefined8 *)(param_3 + 0x58) = 0;
  lVar19 = (long)iVar2;
  initIVector_vsdlss(lVar13,lVar19,0);
  if (0 < iVar3) {
    lVar25 = 0;
    iVar26 = 1;
    lVar28 = *(long *)(param_2 + 0x38);
    do {
      while( true ) {
        iVar23 = *(int *)(lVar9 + 4 + lVar25);
        lVar20 = *(long *)(lVar28 + 8 + lVar25 * 2);
        if (iVar23 < 1) break;
        iVar27 = *(int *)(lVar20 + 4);
        piVar21 = (int *)(lVar20 + 8);
        piVar1 = (int *)(lVar13 + (long)iVar27 * 4);
        iVar17 = 1;
        iVar5 = *piVar1;
        while (iVar5 != 0) {
          iVar17 = iVar17 + 1;
          if (iVar23 < iVar17) goto LAB_00a018d0;
          iVar27 = *piVar21;
          piVar21 = piVar21 + 1;
          piVar1 = (int *)(lVar13 + (long)iVar27 * 4);
          iVar5 = *piVar1;
        }
        *(int *)(lVar13 + 4 + lVar25) = iVar27;
        *piVar1 = iVar26;
        iVar26 = iVar26 + 1;
        lVar25 = lVar25 + 4;
        if (iVar26 == iVar3 + 1) goto LAB_00a018dd;
      }
LAB_00a018d0:
      iVar26 = iVar26 + 1;
      lVar25 = lVar25 + 4;
    } while (iVar26 != iVar3 + 1);
  }
LAB_00a018dd:
  initIVector_vsdlss(lVar14,lVar19,0);
  initIVector_vsdlss(lVar16,lVar19,0);
  initCVector_vsdlss(lVar12,lVar19,0);
  if (0 < iVar3) {
    while( true ) {
      iVar23 = 1;
      iVar26 = 1;
      lVar28 = 0;
      do {
        if (*(int *)(lVar13 + 4 + lVar28 * 4) == 0) {
          if (iVar4 < iVar26) {
            nrerror_vsdlss(1,"mldorder.c",0xd6f);
          }
          lVar25 = (long)iVar26;
          iVar26 = iVar26 + 1;
          *(int *)(lVar11 + lVar25 * 4) = iVar23;
          *(undefined4 *)(lVar14 + 4 + lVar28 * 4) = 1;
        }
        iVar23 = (int)lVar28 + 2;
        lVar28 = lVar28 + 1;
      } while (iVar23 <= iVar3);
      if (iVar26 == 1) break;
      lVar28 = 0;
      iVar23 = 0;
      iVar27 = iVar2 + 1;
      do {
        iVar5 = *(int *)(lVar11 + 4 + lVar28 * 4);
        iVar17 = (int)lVar28;
        lVar25 = (long)iVar5;
        piVar1 = (int *)(lVar14 + lVar25 * 4);
        if (*piVar1 <= iVar27) {
          *(undefined1 *)(lVar12 + lVar25) = 1;
          iVar6 = *(int *)(lVar9 + lVar25 * 4);
          if (0 < iVar6) {
            piVar21 = (int *)(*(long *)(*(long *)(param_2 + 0x38) + lVar25 * 8) + 4);
            iVar22 = 1;
            do {
              while( true ) {
                iVar7 = *piVar21;
                lVar25 = (long)iVar7;
                if (*(char *)(lVar12 + lVar25) == '\0') break;
LAB_00a01aee:
                iVar22 = iVar22 + 1;
                piVar21 = piVar21 + 1;
                if (iVar6 < iVar22) goto LAB_00a01b7d;
              }
              *(char *)(lVar12 + lVar25) = '\x01';
              *(int *)(lVar16 + lVar25 * 4) = iVar5;
              iVar8 = *(int *)(lVar13 + lVar25 * 4);
              if (iVar8 == 0) {
                iVar23 = iVar23 + 1;
                iVar27 = *piVar1;
                puVar15[iVar23] = iVar7;
                goto LAB_00a01aee;
              }
              if (iVar27 <= *piVar1) goto LAB_00a01aee;
              lVar25 = (long)iVar8;
              if (*(char *)(lVar12 + lVar25) != '\0') {
                nrerror_vsdlss(1,"mldorder.c",0xd8c);
              }
              *(int *)(lVar16 + lVar25 * 4) = iVar7;
              if (iVar4 < iVar26) {
                nrerror_vsdlss(1,"mldorder.c",0xd93);
              }
              lVar20 = (long)iVar26;
              iVar22 = iVar22 + 1;
              iVar26 = iVar26 + 1;
              piVar21 = piVar21 + 1;
              *(int *)(lVar11 + lVar20 * 4) = iVar8;
              *(int *)(lVar14 + lVar25 * 4) = *piVar1 + 1;
            } while (iVar22 <= iVar6);
          }
        }
LAB_00a01b7d:
        lVar28 = lVar28 + 1;
      } while (iVar17 + 2 < iVar26);
      if (iVar23 == 0) break;
      iVar26 = 1;
      puVar24 = puVar15;
      do {
        puVar24 = puVar24 + 1;
        iVar26 = iVar26 + 1;
        MLDMinCoverAugment_vsdlss(*puVar24,lVar16,lVar12,lVar13);
      } while (iVar26 <= iVar23);
      initIVector_vsdlss(lVar14,lVar19,0);
      initIVector_vsdlss(lVar16,lVar19,0);
      initCVector_vsdlss(lVar12,lVar19,0);
    }
  }
  uVar18 = MLDMinCoverDecompose_vsdlss(param_1,param_2,lVar13,param_4,lVar14,lVar10);
  *(undefined4 **)(param_3 + 0x78) = puVar15;
  *(long *)(param_3 + 0x48) = lVar12;
  *(long *)(param_3 + 0x68) = lVar13;
  *(long *)(param_3 + 0x70) = lVar14;
  *(long *)(param_3 + 0x58) = lVar16;
  setQToNull_vsdlss(lVar10);
  return uVar18;
}


