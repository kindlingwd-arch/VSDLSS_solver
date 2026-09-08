/* VSDLSS function (decompiled by Ghidra) */
/* name: subBlockContribFromRowRange_1_vsdlss  addr: 009ed9e0  size: 6f5 */
#include "vsdlss_ref.h"

/* signature: void subBlockContribFromRowRange_1_vsdlss(int param_1,int param_2,int param_3,int param_4,long param_5,long param_6,int param_7,undefined8 param_8,long param_9,long param_10,undefined8 param_11); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void subBlockContribFromRowRange_1_vsdlss
               (int param_1,int param_2,int param_3,int param_4,long param_5,long param_6,
               int param_7,undefined8 param_8,long param_9,long param_10,undefined8 param_11)

{
  int iVar1;
  int *piVar2;
  double *pdVar3;
  double *pdVar4;
  uint uVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  ulong uVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  undefined8 uVar19;
  long lVar20;
  long lVar21;
  int iVar22;
  int iVar23;
  int iVar24;
  int iVar25;
  double dVar26;
  long local_8d8;
  int local_8c0;
  int *local_8a8;
  undefined1 local_858 [8];
  long local_850 [130];
  double adStack_440 [130];
  
  uVar14 = DAT_00abd200;
  lVar7 = *(long *)(param_6 + 0x28);
  uVar5 = param_2 - param_1;
  lVar15 = (long)param_1;
  iVar6 = *(int *)(lVar7 + lVar15 * 4);
  lVar8 = *(long *)(param_6 + 0x38);
  lVar9 = *(long *)(param_6 + 0x30);
  lVar10 = *(long *)(lVar9 + lVar15 * 8);
  iVar1 = uVar5 + 1;
  iVar11 = uVar5 + 2;
  if (iVar1 < param_3) {
    iVar11 = param_3;
  }
  if (iVar6 <= param_4) {
    param_4 = iVar6;
  }
  if ((iVar11 <= iVar6) && (iVar11 <= param_4)) {
    if (0 < iVar1) {
      lVar16 = 0;
      do {
        *(double *)((long)adStack_440 + lVar16) =
             _DAT_00ab67f8 / *(double *)(*(long *)(lVar8 + lVar15 * 8 + lVar16) + 8);
        lVar16 = lVar16 + 8;
      } while (lVar16 != (ulong)uVar5 * 8 + 8);
    }
    lVar8 = (ulong)uVar5 * 8 + 8;
    lVar15 = (long)iVar1 * 8;
    iVar13 = iVar11;
    do {
      lVar16 = (long)iVar13 * 4;
      piVar2 = (int *)(lVar10 + lVar16);
      iVar24 = *piVar2;
      lVar17 = *(long *)(param_6 + 0x60);
      if ((lVar17 == 0) || (iVar24 <= *(int *)(param_6 + 0x68))) {
        local_850[0] = *(long *)(*(long *)(param_6 + 0x38) + (long)iVar24 * 8);
      }
      else {
        local_850[0] = *(long *)(lVar17 + (long)(iVar24 - *(int *)(param_6 + 0x68)) * 8);
      }
      local_8c0 = iVar13 + 1;
      if (param_4 < local_8c0) {
        local_8d8 = lVar16 + 4;
        iVar24 = 0;
        iVar25 = 1;
      }
      else {
        local_8d8 = lVar16 + 4;
        iVar22 = *(int *)(lVar10 + 4 + lVar16);
        lVar16 = (long)iVar22;
        iVar23 = *(int *)(param_5 + lVar16 * 4);
        if (iVar23 == *(int *)(param_5 + (long)iVar24 * 4)) {
          lVar20 = (long)local_8c0;
          lVar21 = 0;
          do {
            iVar25 = (int)lVar21 + 2;
            if ((lVar17 == 0) || (iVar22 <= *(int *)(param_6 + 0x68))) {
              local_850[lVar21 + 1] = *(long *)(*(long *)(param_6 + 0x38) + lVar16 * 8);
            }
            else {
              local_850[lVar21 + 1] =
                   *(long *)(lVar17 + (long)(iVar22 - *(int *)(param_6 + 0x68)) * 8);
            }
            if (param_4 < local_8c0 + 1) break;
            local_8c0 = local_8c0 + 1;
            iVar22 = *(int *)(lVar10 + lVar20 * 4 + 4 + lVar21 * 4);
            lVar21 = lVar21 + 1;
            lVar16 = (long)iVar22;
          } while (*(int *)(param_5 + lVar16 * 4) == iVar23);
          iVar24 = 1 - iVar25;
          local_8c0 = iVar13 + iVar25;
          local_8d8 = (long)local_8c0 * 4;
        }
        else {
          iVar24 = 0;
          iVar25 = 1;
        }
      }
      iVar22 = 0;
      lVar16 = param_10 + (long)((iVar13 - iVar11) * iVar1) * 8;
      iVar23 = 1;
      do {
        if (0 < iVar1) {
          lVar17 = 0;
          do {
            *(double *)((long)iVar22 * 8 + param_9 + 8 + lVar17) =
                 (double)(*(ulong *)(lVar16 + (long)iVar22 * 8 + 8 + lVar17) ^ uVar14) *
                 *(double *)((long)adStack_440 + lVar17);
            lVar17 = lVar17 + 8;
          } while (lVar17 != lVar8);
          iVar22 = uVar5 + 1 + iVar22;
        }
        iVar23 = iVar23 + 1;
      } while (iVar23 <= iVar25);
      if (iVar1 < 6) {
        if (iVar25 != 1) {
          iVar22 = 1;
          lVar17 = lVar16;
          local_8a8 = piVar2;
          do {
            iVar23 = *local_8a8;
            lVar20 = param_9;
            lVar21 = 0;
            do {
              dVar26 = 0.0;
              if (0 < iVar1) {
                dVar26 = 0.0;
                lVar18 = 0;
                do {
                  pdVar3 = (double *)(lVar20 + 8 + lVar18);
                  pdVar4 = (double *)(lVar17 + 8 + lVar18);
                  lVar18 = lVar18 + 8;
                  dVar26 = dVar26 + *pdVar3 * *pdVar4;
                } while (lVar18 != lVar8);
              }
              lVar20 = lVar20 + lVar15;
              pdVar3 = (double *)(local_850[lVar21] + 8 + (long)(iVar23 - piVar2[lVar21]) * 8);
              *pdVar3 = dVar26 + *pdVar3;
              iVar12 = (int)lVar21;
              lVar21 = lVar21 + 1;
            } while (iVar12 + 2 <= iVar22);
            iVar22 = iVar22 + 1;
            lVar17 = lVar17 + lVar15;
            local_8a8 = local_8a8 + 1;
          } while (iVar22 != iVar25);
          lVar16 = lVar16 + ((ulong)(iVar25 - 2) + 1) * lVar15;
          goto LAB_009edd3f;
        }
LAB_009edf7e:
        iVar22 = *(int *)(lVar10 + -4 + local_8d8);
      }
      else {
        if (((param_7 == 0) ||
            (((double)iVar25 - _DAT_00ab67f8) * (double)iVar25 * (double)iVar1 * DAT_00ab7228 <
             _DAT_00b1a158)) || (iVar25 < 6)) {
          updateLeadTriDestSupNodeRange(iVar13,iVar1,lVar10,1,iVar25 + -1,local_858,param_9,lVar16);
          lVar16 = lVar16 + (long)((iVar25 + -1) * iVar1) * 8;
          if (iVar25 == 1) goto LAB_009edf7e;
        }
        else {
          updateLeadTriDestSupNodePar(iVar13,iVar1,lVar10,iVar25,local_858,param_9,lVar16);
          lVar16 = lVar16 + (long)((iVar25 + -1) * iVar1) * 8;
        }
LAB_009edd3f:
        iVar22 = *(int *)(lVar10 + -4 + local_8d8);
        lVar17 = 0;
        do {
          local_850[lVar17] = local_850[lVar17] + (long)(iVar22 - piVar2[lVar17]) * 8;
          iVar23 = (int)lVar17;
          lVar17 = lVar17 + 1;
        } while (iVar23 + 2 < iVar25);
      }
      iVar23 = iVar24 + 1 + (iVar6 - iVar13);
      iVar24 = *(int *)(lVar7 + (long)iVar22 * 4);
      if (0 < iVar23) {
        uVar19 = 0;
        if (iVar23 != iVar24) {
          getSearchIndices_vsdlss
                    (lVar10 + -8 + ((long)iVar25 + (long)iVar13) * 4,iVar23,
                     *(undefined8 *)(lVar9 + (long)iVar22 * 8),iVar24,param_8);
          uVar19 = param_8;
        }
        if (iVar1 < 7) {
          matMatMultSrcExtLEQ6_casi_vsdlss(param_9,iVar23,iVar1,lVar16,iVar25,param_11);
          addToDestVecs_vsdlss(iVar23,uVar19,iVar24,iVar25,param_11,local_858);
        }
        else if ((param_7 == 0) || ((double)iVar23 * (double)iVar1 * (double)iVar25 < _DAT_00b1a158)
                ) {
          matMatMult_casi_vsdlss(param_9,iVar23,iVar1,lVar16,iVar25,param_11,1);
          addToDestVecsByCols_vsdlss(iVar23,uVar19,iVar24,iVar25,param_11,local_858);
        }
        else {
          matMatMult_casiParallel(param_9,iVar23,iVar1,lVar16,iVar25,param_11,1,uVar19,local_858);
        }
      }
      iVar13 = local_8c0;
    } while (local_8c0 <= param_4);
  }
  return;
}


