/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDCreateCoarseGraph_vsdlss  addr: 009fe160  size: 5d9 */
#include "vsdlss_ref.h"

/* signature: void MLDCreateCoarseGraph_vsdlss(undefined4 param_1,int param_2,long param_3,long param_4,long param_5,undefined8 param_6,long param_7); */

void MLDCreateCoarseGraph_vsdlss
               (undefined4 param_1,int param_2,long param_3,long param_4,long param_5,
               undefined8 param_6,long param_7)

{
  uint *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  long lVar19;
  long lVar20;
  long lVar21;
  long lVar22;
  long lVar23;
  int iVar24;
  int iVar25;
  int iVar26;
  long lVar27;
  long lVar28;
  long lVar29;
  long lVar30;
  int *piVar31;
  long lVar32;
  long lVar33;
  int iVar34;
  int *piVar35;
  int *piVar36;
  int *piVar37;
  int *piVar38;
  uint uVar39;
  
  lVar11 = *(long *)(param_3 + 0x28);
  iVar3 = *(int *)(param_3 + 8);
  lVar12 = *(long *)(param_3 + 0x48);
  lVar13 = *(long *)(param_3 + 0x60);
  lVar14 = *(long *)(param_3 + 0x30);
  iVar4 = *(int *)(param_3 + 0xc);
  lVar15 = *(long *)(param_3 + 0x38);
  lVar16 = *(long *)(param_3 + 0x58);
  if ((lVar12 == 0) || (lVar16 == 0)) {
    nrerror_vsdlss(1,"mldorder.c",0x643);
  }
  lVar27 = tagNewMLDGraph_vsdlss(param_1,*(int *)(param_3 + 4) + 1,param_2);
  lVar17 = *(long *)(lVar27 + 0x28);
  lVar18 = *(long *)(lVar27 + 0x40);
  lVar19 = *(long *)(lVar27 + 0x50);
  lVar20 = *(long *)(lVar27 + 0x30);
  lVar21 = *(long *)(lVar27 + 0x58);
  lVar22 = *(long *)(lVar27 + 0x38);
  lVar23 = *(long *)(lVar27 + 0x48);
  initIVector_vsdlss(param_7,(long)param_2,0);
  if (iVar3 < 1) {
    iVar34 = 0;
    uVar39 = 0;
    iVar24 = 0;
LAB_009fe4cd:
    if (((int)uVar39 <= ((param_2 - iVar3) + iVar4) * 2) && (iVar24 == param_2)) goto LAB_009fe4f5;
  }
  else {
    piVar35 = (int *)(param_5 + 4);
    uVar39 = 0;
    piVar2 = (int *)(param_5 + 8 + (ulong)(iVar3 - 1) * 4);
    iVar34 = 1;
    iVar24 = 0;
    do {
      while( true ) {
        iVar5 = *piVar35;
        lVar32 = (long)iVar5;
        iVar6 = *(int *)(lVar13 + lVar32 * 4);
        if (iVar6 != iVar24 + 1) break;
        lVar28 = (long)iVar6;
        iVar7 = 0;
        *(long *)(lVar22 + lVar28 * 8) = lVar18 + (long)(int)uVar39 * 4;
        *(long *)(lVar23 + lVar28 * 8) = (long)(int)uVar39 * 4 + lVar19;
        iVar24 = *(int *)(param_4 + lVar32 * 4);
        if (iVar5 != iVar24) {
          iVar7 = *(int *)(lVar11 + (long)iVar24 * 4);
        }
        piVar37 = *(int **)(lVar12 + lVar32 * 8);
        iVar26 = *(int *)(lVar11 + lVar32 * 4);
        lVar33 = *(long *)(lVar15 + lVar32 * 8);
        piVar38 = (int *)(lVar21 + lVar28 * 4);
        piVar31 = (int *)(lVar28 * 4 + lVar20);
        *piVar38 = *(int *)(lVar16 + lVar32 * 4);
        *piVar31 = *(int *)(lVar14 + lVar32 * 4);
        if (iVar26 + iVar7 < 0x10) {
          if (0 < iVar26) {
            piVar36 = (int *)(lVar33 + 4);
            do {
              piVar37 = piVar37 + 1;
              iVar9 = *(int *)(lVar13 + (long)*piVar36 * 4);
              if (iVar6 != iVar9) {
                if ((int)uVar39 < iVar34) {
LAB_009fe5c1:
                  uVar39 = uVar39 + 1;
                  *(int *)(lVar18 + (long)(int)uVar39 * 4) = iVar9;
                  *(int *)(lVar19 + (long)(int)uVar39 * 4) = *piVar37;
                }
                else {
                  iVar10 = *(int *)(lVar18 + (long)iVar34 * 4);
                  lVar32 = (long)iVar34 * 4;
                  iVar25 = iVar34;
                  while (iVar10 != iVar9) {
                    lVar32 = lVar32 + 4;
                    iVar25 = iVar25 + 1;
                    if ((int)uVar39 < iVar25) goto LAB_009fe5c1;
                    iVar10 = *(int *)(lVar18 + lVar32);
                  }
                  *(int *)(lVar19 + lVar32) = *(int *)(lVar19 + lVar32) + *piVar37;
                }
              }
              piVar36 = piVar36 + 1;
            } while (piVar36 != (int *)(lVar33 + 8 + (ulong)(iVar26 - 1) * 4));
          }
          if (iVar5 != iVar24) {
            lVar33 = (long)iVar24;
            piVar37 = *(int **)(lVar12 + lVar33 * 8);
            lVar32 = *(long *)(lVar15 + lVar33 * 8);
            *piVar38 = *piVar38 + *(int *)(lVar16 + lVar33 * 4);
            *piVar31 = *piVar31 + *(int *)(lVar14 + lVar33 * 4);
            if (0 < iVar7) {
              piVar38 = (int *)(lVar32 + 4);
              do {
                piVar37 = piVar37 + 1;
                iVar24 = *(int *)(lVar13 + (long)*piVar38 * 4);
                if (iVar6 == iVar24) {
                  iVar24 = *piVar31;
                  iVar5 = *piVar37;
                  *piVar31 = iVar24 - iVar5;
                  *piVar31 = (iVar24 - iVar5) - *piVar37;
                }
                else if ((int)uVar39 < iVar34) {
LAB_009fe6bd:
                  uVar39 = uVar39 + 1;
                  *(int *)(lVar18 + (long)(int)uVar39 * 4) = iVar24;
                  *(int *)(lVar19 + (long)(int)uVar39 * 4) = *piVar37;
                }
                else {
                  iVar5 = *(int *)(lVar18 + (long)iVar34 * 4);
                  lVar33 = (long)iVar34 * 4;
                  iVar26 = iVar34;
                  while (iVar5 != iVar24) {
                    lVar33 = lVar33 + 4;
                    iVar26 = iVar26 + 1;
                    if ((int)uVar39 < iVar26) goto LAB_009fe6bd;
                    iVar5 = *(int *)(lVar18 + lVar33);
                  }
                  *(int *)(lVar19 + lVar33) = *(int *)(lVar19 + lVar33) + *piVar37;
                }
                piVar38 = piVar38 + 1;
              } while (piVar38 != (int *)(lVar32 + 8 + (ulong)(iVar7 - 1) * 4));
            }
          }
        }
        else {
          if (0 < iVar26) {
            lVar32 = (ulong)(iVar26 - 1) * 4 + 4;
            lVar29 = 0;
LAB_009fe3a0:
            do {
              iVar26 = *(int *)(lVar13 + (long)*(int *)(lVar33 + 4 + lVar29) * 4);
              if (iVar6 != iVar26) {
                puVar1 = (uint *)(param_7 + (long)iVar26 * 4);
                uVar8 = *puVar1;
                if (iVar34 <= (int)uVar8) {
                  lVar30 = lVar29 + 4;
                  lVar29 = lVar29 + 4;
                  piVar36 = (int *)(lVar19 + (long)(int)uVar8 * 4);
                  *piVar36 = *piVar36 + *(int *)((long)piVar37 + lVar30);
                  if (lVar29 == lVar32) break;
                  goto LAB_009fe3a0;
                }
                uVar39 = uVar39 + 1;
                *puVar1 = uVar39;
                *(int *)(lVar18 + (long)(int)uVar39 * 4) = iVar26;
                *(undefined4 *)(lVar19 + (long)(int)uVar39 * 4) =
                     *(undefined4 *)((long)piVar37 + lVar29 + 4);
              }
              lVar29 = lVar29 + 4;
            } while (lVar29 != lVar32);
          }
          lVar32 = (long)iVar24;
          if (iVar5 != iVar24) {
            lVar33 = *(long *)(lVar15 + lVar32 * 8);
            lVar29 = *(long *)(lVar12 + lVar32 * 8);
            *piVar38 = *piVar38 + *(int *)(lVar16 + lVar32 * 4);
            *piVar31 = *piVar31 + *(int *)(lVar14 + lVar32 * 4);
            if (0 < iVar7) {
              lVar32 = (ulong)(iVar7 - 1) * 4 + 4;
              lVar30 = 0;
LAB_009fe460:
              do {
                iVar24 = *(int *)(lVar13 + (long)*(int *)(lVar33 + 4 + lVar30) * 4);
                if (iVar6 == iVar24) {
                  iVar24 = *piVar31 - *(int *)(lVar29 + 4 + lVar30);
                  *piVar31 = iVar24;
                  *piVar31 = iVar24 - *(int *)(lVar29 + 4 + lVar30);
                }
                else {
                  puVar1 = (uint *)(param_7 + (long)iVar24 * 4);
                  uVar8 = *puVar1;
                  if (iVar34 <= (int)uVar8) {
                    piVar37 = (int *)(lVar29 + 4 + lVar30);
                    lVar30 = lVar30 + 4;
                    piVar38 = (int *)(lVar19 + (long)(int)uVar8 * 4);
                    *piVar38 = *piVar38 + *piVar37;
                    if (lVar30 == lVar32) break;
                    goto LAB_009fe460;
                  }
                  uVar39 = uVar39 + 1;
                  *puVar1 = uVar39;
                  *(int *)(lVar18 + (long)(int)uVar39 * 4) = iVar24;
                  *(undefined4 *)(lVar19 + (long)(int)uVar39 * 4) =
                       *(undefined4 *)(lVar29 + 4 + lVar30);
                }
                lVar30 = lVar30 + 4;
              } while (lVar30 != lVar32);
            }
          }
        }
        piVar35 = piVar35 + 1;
        iVar24 = uVar39 - iVar34;
        iVar34 = uVar39 + 1;
        *(int *)(lVar17 + lVar28 * 4) = iVar24 + 1;
        iVar24 = iVar6;
        if (piVar35 == piVar2) goto LAB_009fe4c2;
      }
      piVar35 = piVar35 + 1;
    } while (piVar35 != piVar2);
LAB_009fe4c2:
    iVar34 = (int)uVar39 >> 1;
    if ((uVar39 & 1) == 0) goto LAB_009fe4cd;
  }
  nrerror_vsdlss(1,"mldorder.c",0x6ce);
LAB_009fe4f5:
  *(int *)(lVar27 + 0xc) = iVar34;
  return;
}


