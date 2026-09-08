/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDFindIndistInSupAdjArr_vsdlss  addr: 00a2a750  size: 4f7 */
#include "vsdlss_ref.h"

/* signature: void MLDFindIndistInSupAdjArr_vsdlss(int param_1,long param_2,long param_3,long param_4,long param_5,int *param_6,int *param_7); */

void MLDFindIndistInSupAdjArr_vsdlss
               (int param_1,long param_2,long param_3,long param_4,long param_5,int *param_6,
               int *param_7)

{
  long *plVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  bool bVar17;
  int iVar18;
  int iVar19;
  long lVar20;
  int *piVar21;
  long lVar22;
  int *piVar23;
  int *piVar24;
  uint uVar25;
  long lVar26;
  int *piVar27;
  long lVar28;
  int *piVar29;
  int iVar30;
  int iVar31;
  int *piVar32;
  int iVar33;
  
  uVar4 = param_6[2];
  lVar11 = *(long *)(param_6 + 10);
  lVar12 = *(long *)(param_3 + 0x28);
  lVar13 = *(long *)(param_3 + 0x38);
  iVar5 = *param_6;
  lVar14 = *(long *)(param_7 + 2);
  iVar6 = param_6[1];
  lVar15 = *(long *)(param_6 + 4);
  lVar16 = *(long *)(param_6 + 6);
  iVar3 = param_1;
  do {
    if (iVar3 < 1) {
      return;
    }
    iVar18 = iVar5;
    if (iVar3 <= iVar5) {
      iVar18 = iVar3;
    }
    if (0 < iVar18) {
      lVar28 = 0;
      do {
        while( true ) {
          lVar20 = (long)*(int *)(param_2 + 4 + lVar28 * 4);
          iVar31 = *(int *)(lVar12 + lVar20 * 4);
          iVar30 = (int)lVar28;
          if (((0 < *(int *)(param_4 + lVar20 * 4)) || (param_1 < iVar31)) ||
             (*(int *)(param_5 + lVar20 * 4) == -0x7ffffff5)) break;
          lVar20 = *(long *)(lVar13 + lVar20 * 8);
          if (iVar31 < 1) {
            lVar20 = 4;
            bVar17 = false;
            iVar31 = 1;
          }
          else {
            piVar21 = (int *)(lVar20 + 4);
            uVar25 = 0;
            do {
              uVar25 = uVar25 + *piVar21;
              if (iVar6 <= (int)uVar25) {
                uVar25 = uVar25 & uVar4;
              }
              piVar21 = piVar21 + 1;
            } while (piVar21 != (int *)(lVar20 + 8 + (ulong)(iVar31 - 1) * 4));
            iVar31 = uVar25 + 1;
            bVar17 = iVar31 < 1;
            lVar20 = (long)iVar31 << 2;
          }
          if ((iVar6 < iVar31) || (bVar17)) {
            nrerror_vsdlss(1,"mldmmd.c",0x67c);
          }
          *(int *)(lVar11 + 4 + lVar28 * 4) = iVar31;
          lVar28 = lVar28 + 1;
          *(int *)(lVar15 + lVar20) = *(int *)(lVar15 + lVar20) + 1;
          if (iVar18 < iVar30 + 2) goto LAB_00a2a8b5;
        }
        lVar28 = lVar28 + 1;
      } while (iVar30 + 2 <= iVar18);
LAB_00a2a8b5:
      lVar28 = *(long *)(param_6 + 8);
      lVar20 = (ulong)(iVar18 - 1) * 4 + 4;
      lVar22 = 0;
      do {
        lVar26 = (long)*(int *)(param_2 + 4 + lVar22);
        if (((*(int *)(param_4 + lVar26 * 4) < 1) && (*(int *)(lVar12 + lVar26 * 4) <= param_1)) &&
           (*(int *)(param_5 + lVar26 * 4) != -0x7ffffff5)) {
          lVar26 = (long)*(int *)(lVar11 + 4 + lVar22);
          piVar21 = (int *)(lVar15 + lVar26 * 4);
          iVar31 = *piVar21;
          if (iVar31 != 0) {
            *(long *)(lVar16 + lVar26 * 8) = lVar28;
            lVar28 = lVar28 + (long)iVar31 * 4;
            *piVar21 = 0;
          }
        }
        lVar22 = lVar22 + 4;
      } while (lVar22 != lVar20);
      lVar28 = 0;
      do {
        iVar31 = *(int *)(param_2 + 4 + lVar28);
        lVar22 = (long)iVar31;
        if (((*(int *)(param_4 + lVar22 * 4) < 1) && (*(int *)(lVar12 + lVar22 * 4) <= param_1)) &&
           (*(int *)(param_5 + lVar22 * 4) != -0x7ffffff5)) {
          lVar22 = (long)*(int *)(lVar11 + 4 + lVar28);
          piVar21 = (int *)(lVar15 + lVar22 * 4);
          lVar22 = *(long *)(lVar16 + lVar22 * 8);
          iVar30 = *piVar21 + 1;
          *piVar21 = iVar30;
          *(int *)(lVar22 + (long)iVar30 * 4) = iVar31;
        }
        lVar28 = lVar28 + 4;
      } while (lVar28 != lVar20);
      lVar28 = 0;
      do {
        while( true ) {
          lVar20 = (long)*(int *)(param_2 + 4 + lVar28 * 4);
          iVar31 = (int)lVar28;
          if (((*(int *)(param_4 + lVar20 * 4) < 1) && (*(int *)(lVar12 + lVar20 * 4) <= param_1))
             && (*(int *)(param_5 + lVar20 * 4) != -0x7ffffff5)) break;
LAB_00a2a950:
          lVar28 = lVar28 + 1;
          if (iVar18 < iVar31 + 2) goto LAB_00a2ab4a;
        }
        lVar20 = (long)*(int *)(lVar11 + 4 + lVar28 * 4);
        piVar21 = (int *)(lVar15 + lVar20 * 4);
        iVar30 = *piVar21;
        if (iVar30 < 2) {
          *piVar21 = 0;
          *(undefined8 *)(lVar16 + lVar20 * 8) = 0;
          goto LAB_00a2a950;
        }
        lVar22 = 2;
        plVar1 = (long *)(lVar16 + lVar20 * 8);
        lVar20 = *plVar1;
        piVar29 = (int *)(lVar20 + 8);
        do {
          lVar26 = (long)piVar29[-1];
          piVar2 = (int *)(param_5 + lVar26 * 4);
          iVar33 = (int)lVar22;
          if (*piVar2 != -0x7ffffff5) {
            iVar7 = *(int *)(lVar12 + lVar26 * 4);
            lVar26 = *(long *)(lVar13 + lVar26 * 8);
            iVar8 = *param_7;
            if (0 < iVar7) {
              piVar23 = (int *)(lVar26 + 4);
              do {
                iVar9 = *piVar23;
                piVar23 = piVar23 + 1;
                *(int *)(lVar14 + (long)iVar9 * 4) = iVar8;
              } while (piVar23 != (int *)(lVar26 + 8 + (ulong)(iVar7 - 1) * 4));
            }
            if (iVar33 <= iVar30) {
              piVar23 = (int *)(lVar20 + 4 + ((ulong)(uint)(iVar30 - iVar33) + lVar22) * 4);
              piVar24 = piVar29;
              do {
                while( true ) {
                  lVar26 = (long)*piVar24;
                  if (iVar7 == *(int *)(lVar12 + lVar26 * 4)) break;
LAB_00a2aa68:
                  piVar24 = piVar24 + 1;
                  if (piVar24 == piVar23) goto LAB_00a2aaf0;
                }
                piVar32 = (int *)(lVar26 * 4 + param_5);
                iVar9 = *piVar32;
                if (iVar9 == -0x7ffffff5) goto LAB_00a2aa68;
                lVar26 = *(long *)(lVar13 + lVar26 * 8);
                if (0 < iVar7) {
                  if (iVar8 <= *(int *)(lVar14 + (long)*(int *)(lVar26 + 4) * 4)) {
                    piVar27 = (int *)(lVar26 + 8);
                    iVar19 = 1;
                    do {
                      iVar19 = iVar19 + 1;
                      if (iVar7 < iVar19) goto LAB_00a2aacd;
                      iVar10 = *piVar27;
                      piVar27 = piVar27 + 1;
                    } while (iVar8 <= *(int *)(lVar14 + (long)iVar10 * 4));
                  }
                  goto LAB_00a2aa68;
                }
LAB_00a2aacd:
                piVar24 = piVar24 + 1;
                *piVar2 = *piVar2 + iVar9 + 0x7ffffff5;
                *piVar32 = -0x7ffffff5;
              } while (piVar24 != piVar23);
            }
LAB_00a2aaf0:
            unmarkCountingMark_vsdlss(param_7);
          }
          piVar29 = piVar29 + 1;
          lVar22 = lVar22 + 1;
        } while (iVar33 <= iVar30);
        *piVar21 = 0;
        lVar28 = lVar28 + 1;
        *plVar1 = 0;
      } while (iVar31 + 2 <= iVar18);
    }
LAB_00a2ab4a:
    iVar3 = iVar3 - iVar18;
    param_2 = param_2 + (long)iVar18 * 4;
  } while( true );
}


