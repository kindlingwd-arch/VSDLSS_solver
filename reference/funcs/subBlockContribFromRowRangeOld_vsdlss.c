/* VSDLSS function (decompiled by Ghidra) */
/* name: subBlockContribFromRowRangeOld_vsdlss  addr: 009ed220  size: 7ad */
#include "vsdlss_ref.h"

/* signature: void subBlockContribFromRowRangeOld_vsdlss(int param_1,int param_2,int param_3,int param_4,long param_5,long param_6,int param_7,undefined8 param_8,long param_9,long param_10,undefined8 param_11); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void subBlockContribFromRowRangeOld_vsdlss
               (int param_1,int param_2,int param_3,int param_4,long param_5,long param_6,
               int param_7,undefined8 param_8,long param_9,long param_10,undefined8 param_11)

{
  int iVar1;
  double *pdVar2;
  double *pdVar3;
  uint uVar4;
  int iVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  int iVar10;
  ulong uVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  int iVar16;
  int iVar17;
  long lVar18;
  int iVar19;
  int *piVar20;
  long lVar21;
  int iVar22;
  int iVar23;
  undefined8 uVar24;
  long lVar25;
  double dVar26;
  long local_8d0;
  int local_8bc;
  int local_8b0;
  int *local_898;
  undefined1 local_858 [8];
  long local_850 [130];
  double adStack_440 [130];
  
  lVar6 = *(long *)(param_6 + 0x28);
  uVar4 = param_2 - param_1;
  lVar12 = (long)param_1;
  iVar5 = *(int *)(lVar6 + lVar12 * 4);
  lVar7 = *(long *)(param_6 + 0x30);
  lVar8 = *(long *)(param_6 + 0x38);
  lVar9 = *(long *)(lVar7 + lVar12 * 8);
  iVar1 = uVar4 + 1;
  local_8b0 = uVar4 + 2;
  if (local_8b0 <= iVar5) {
    lVar25 = (long)local_8b0;
    iVar16 = *(int *)(lVar9 + lVar25 * 4);
    if (iVar16 < param_3) {
      piVar20 = (int *)(lVar9 + 4 + lVar25 * 4);
      do {
        local_8b0 = local_8b0 + 1;
        if (iVar5 < local_8b0) {
          return;
        }
        iVar16 = *piVar20;
        piVar20 = piVar20 + 1;
      } while (iVar16 < param_3);
      lVar25 = (long)local_8b0;
    }
    if (0 < iVar1) {
      lVar18 = 0;
      do {
        *(double *)((long)adStack_440 + lVar18) =
             _DAT_00ab67f8 / *(double *)(*(long *)(lVar12 * 8 + lVar8 + lVar18) + 8);
        lVar18 = lVar18 + 8;
      } while (lVar18 != (ulong)uVar4 * 8 + 8);
    }
    lVar18 = (ulong)uVar4 * 8 + 8;
    lVar25 = lVar25 << 3;
    iVar22 = 0;
    iVar23 = local_8b0;
    do {
      if (0 < iVar1) {
        lVar15 = 0;
        do {
          *(undefined8 *)(param_10 + (long)iVar22 * 8 + 8 + lVar15) =
               *(undefined8 *)(*(long *)(lVar12 * 8 + lVar8 + lVar15) + (lVar25 - lVar15));
          lVar15 = lVar15 + 8;
        } while (lVar15 != lVar18);
        iVar22 = uVar4 + 1 + iVar22;
      }
      uVar11 = DAT_00abd200;
      iVar23 = iVar23 + 1;
      lVar25 = lVar25 + 8;
    } while (iVar23 <= iVar5);
    lVar8 = (long)iVar1 * 8;
    iVar22 = local_8b0;
    while (lVar12 = (long)iVar22, iVar16 <= param_4) {
      lVar25 = *(long *)(param_6 + 0x60);
      if ((lVar25 == 0) || (iVar16 <= *(int *)(param_6 + 0x68))) {
        local_850[0] = *(long *)(*(long *)(param_6 + 0x38) + (long)iVar16 * 8);
      }
      else {
        local_850[0] = *(long *)(lVar25 + (long)(iVar16 - *(int *)(param_6 + 0x68)) * 8);
      }
      local_8bc = iVar22 + 1;
      lVar15 = (long)local_8bc;
      if (iVar5 < local_8bc) {
        local_8d0 = lVar15 << 2;
        iVar23 = 0;
        iVar16 = 1;
      }
      else {
        local_8d0 = lVar15 * 4;
        iVar23 = *(int *)(lVar9 + lVar15 * 4);
        if (iVar23 <= param_4) {
          lVar15 = (long)iVar23;
          iVar17 = *(int *)(param_5 + lVar15 * 4);
          if (iVar17 == *(int *)(param_5 + (long)iVar16 * 4)) {
            lVar14 = 0;
            do {
              iVar19 = (int)lVar14;
              iVar16 = iVar19 + 2;
              if ((lVar25 != 0) && (*(int *)(param_6 + 0x68) < iVar23)) {
                local_850[lVar14 + 1] =
                     *(long *)(lVar25 + (long)(iVar23 - *(int *)(param_6 + 0x68)) * 8);
                if (iVar19 + iVar22 + 2 <= iVar5) goto LAB_009ed47c;
LAB_009ed4c8:
                iVar23 = 1 - iVar16;
                local_8bc = iVar22 + iVar16;
                local_8d0 = (long)local_8bc * 4;
                goto LAB_009ed4eb;
              }
              local_850[lVar14 + 1] = *(long *)(*(long *)(param_6 + 0x38) + lVar15 * 8);
              if (iVar5 < iVar19 + iVar22 + 2) goto LAB_009ed4c8;
LAB_009ed47c:
              iVar23 = *(int *)(local_8d0 + lVar9 + 4 + lVar14 * 4);
              if (param_4 < iVar23) goto LAB_009ed4c8;
              lVar15 = (long)iVar23;
              lVar14 = lVar14 + 1;
            } while (*(int *)(param_5 + lVar15 * 4) == iVar17);
            iVar23 = 1 - iVar16;
            local_8bc = iVar22 + iVar16;
            local_8d0 = (long)local_8bc * 4;
            goto LAB_009ed4eb;
          }
        }
        iVar23 = 0;
        iVar16 = 1;
      }
LAB_009ed4eb:
      iVar19 = 1;
      lVar25 = param_10 + (long)((iVar22 - local_8b0) * iVar1) * 8;
      iVar17 = 0;
      do {
        if (0 < iVar1) {
          lVar15 = 0;
          do {
            *(double *)((long)iVar17 * 8 + param_9 + 8 + lVar15) =
                 (double)(*(ulong *)(lVar25 + (long)iVar17 * 8 + 8 + lVar15) ^ uVar11) *
                 *(double *)((long)adStack_440 + lVar15);
            lVar15 = lVar15 + 8;
          } while (lVar15 != lVar18);
          iVar17 = uVar4 + 1 + iVar17;
        }
        iVar19 = iVar19 + 1;
      } while (iVar19 <= iVar16);
      if (iVar1 < 6) {
        if (iVar16 != 1) {
          piVar20 = (int *)(lVar9 + lVar12 * 4);
          iVar17 = 1;
          lVar15 = lVar25;
          local_898 = piVar20;
          do {
            iVar19 = *local_898;
            lVar14 = param_9;
            lVar21 = 0;
            do {
              dVar26 = 0.0;
              if (0 < iVar1) {
                dVar26 = 0.0;
                lVar13 = 0;
                do {
                  pdVar2 = (double *)(lVar14 + 8 + lVar13);
                  pdVar3 = (double *)(lVar15 + 8 + lVar13);
                  lVar13 = lVar13 + 8;
                  dVar26 = dVar26 + *pdVar2 * *pdVar3;
                } while (lVar13 != lVar18);
              }
              lVar14 = lVar14 + lVar8;
              pdVar2 = (double *)(local_850[lVar21] + 8 + (long)(iVar19 - piVar20[lVar21]) * 8);
              *pdVar2 = dVar26 + *pdVar2;
              iVar10 = (int)lVar21;
              lVar21 = lVar21 + 1;
            } while (iVar10 + 2 <= iVar17);
            iVar17 = iVar17 + 1;
            lVar15 = lVar15 + lVar8;
            local_898 = local_898 + 1;
          } while (iVar17 != iVar16);
          lVar25 = lVar25 + ((ulong)(iVar16 - 2) + 1) * lVar8;
          goto LAB_009ed628;
        }
LAB_009ed85c:
        iVar17 = *(int *)(lVar9 + -4 + local_8d0);
      }
      else {
        if (((param_7 == 0) ||
            (((double)iVar16 - _DAT_00ab67f8) * (double)iVar16 * (double)iVar1 * DAT_00ab7228 <
             _DAT_00b1a158)) || (iVar16 < 6)) {
          updateLeadTriDestSupNodeRange(iVar22,iVar1,lVar9,1,iVar16 + -1,local_858,param_9,lVar25);
          lVar25 = lVar25 + (long)((iVar16 + -1) * iVar1) * 8;
          if (iVar16 == 1) goto LAB_009ed85c;
        }
        else {
          updateLeadTriDestSupNodePar(iVar22,iVar1,lVar9,iVar16,local_858,param_9,lVar25);
          lVar25 = lVar25 + (long)((iVar16 + -1) * iVar1) * 8;
        }
LAB_009ed628:
        iVar17 = *(int *)(lVar9 + -4 + local_8d0);
        lVar15 = 0;
        do {
          local_850[lVar15] =
               local_850[lVar15] + (long)(iVar17 - *(int *)(lVar9 + lVar12 * 4 + lVar15 * 4)) * 8;
          iVar19 = (int)lVar15;
          lVar15 = lVar15 + 1;
        } while (iVar19 + 2 < iVar16);
      }
      iVar23 = iVar23 + 1 + (iVar5 - iVar22);
      iVar22 = *(int *)(lVar6 + (long)iVar17 * 4);
      if (0 < iVar23) {
        uVar24 = 0;
        if (iVar23 != iVar22) {
          getSearchIndices_vsdlss
                    (lVar9 + -8 + (iVar16 + lVar12) * 4,iVar23,
                     *(undefined8 *)(lVar7 + (long)iVar17 * 8),iVar22,param_8);
          uVar24 = param_8;
        }
        if (iVar1 < 7) {
          matMatMultSrcExtLEQ6_casi_vsdlss(param_9,iVar23,iVar1,lVar25,iVar16,param_11);
          addToDestVecs_vsdlss(iVar23,uVar24,iVar22,iVar16,param_11,local_858);
        }
        else if ((param_7 == 0) || ((double)iVar23 * (double)iVar1 * (double)iVar16 < _DAT_00b1a158)
                ) {
          matMatMult_casi_vsdlss(param_9,iVar23,iVar1,lVar25,iVar16,param_11,1);
          addToDestVecsByCols_vsdlss(iVar23,uVar24,iVar22,iVar16,param_11,local_858);
        }
        else {
          matMatMult_casiParallel(param_9,iVar23,iVar1,lVar25,iVar16,param_11,1,uVar24,local_858);
        }
      }
      if (iVar5 < local_8bc) {
        return;
      }
      iVar22 = local_8bc;
      iVar16 = *(int *)(lVar9 + local_8d0);
    }
  }
  return;
}


