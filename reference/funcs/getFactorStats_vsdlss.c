/* VSDLSS function (decompiled by Ghidra) */
/* name: getFactorStats_vsdlss  addr: 009dcfc0  size: 22e */
#include "vsdlss_ref.h"

/* signature: void getFactorStats_vsdlss(long param_1,int *param_2,double *param_3,double *param_4,double *param_5,double *param_6,double *param_7,double *param_8,double *param_9,double *param_10); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void getFactorStats_vsdlss
               (long param_1,int *param_2,double *param_3,double *param_4,double *param_5,
               double *param_6,double *param_7,double *param_8,double *param_9,double *param_10)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  double dVar10;
  double dVar11;
  int iVar12;
  long lVar13;
  long lVar14;
  double dVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  double dVar20;
  double dVar21;
  double dVar22;
  
  dVar18 = DAT_00ad4f20;
  dVar11 = DAT_00ab7228;
  dVar10 = _DAT_00ab67f8;
  dVar16 = DAT_00ab6798;
  iVar1 = param_2[1];
  iVar2 = *(int *)(param_1 + 8);
  iVar3 = *param_2;
  if (0 < iVar2) {
    lVar6 = *(long *)(param_1 + 0x38);
    lVar7 = *(long *)(param_1 + 0x48);
    lVar14 = 0;
    do {
      iVar4 = *(int *)(lVar6 + 4 + lVar14);
      iVar5 = *(int *)(lVar7 + 4 + lVar14);
      dVar22 = 0.0;
      if (iVar4 <= iVar5) {
        dVar22 = 0.0;
        lVar13 = 0;
        lVar8 = *(long *)(param_2 + 6);
        lVar9 = *(long *)(param_2 + 4);
        do {
          dVar20 = (double)*(int *)((long)iVar4 * 4 + lVar9 + lVar13);
          dVar21 = (double)*(int *)((long)iVar4 * 4 + lVar8 + lVar13);
          dVar19 = dVar21 * dVar18;
          dVar17 = dVar21 * dVar20 - (dVar20 - dVar10) * dVar20 * dVar11;
          dVar15 = dVar17 * dVar16;
          *param_8 = dVar17 + *param_8;
          dVar22 = dVar22 + dVar19 + dVar15;
          *param_5 = dVar19 + *param_5;
          *param_6 = dVar15 + *param_6;
          if (dVar10 <= dVar20) {
            dVar15 = *param_9;
            iVar12 = 1;
            dVar17 = dVar10;
            do {
              iVar12 = iVar12 + 1;
              dVar15 = dVar15 + ((dVar21 - dVar17) + dVar10) * (dVar21 - dVar17);
              dVar17 = (double)iVar12;
            } while (dVar17 <= dVar20);
            *param_9 = dVar15;
          }
          lVar13 = lVar13 + 4;
        } while (lVar13 != (ulong)(uint)(iVar5 - iVar4) * 4 + 4);
      }
      lVar14 = lVar14 + 4;
      dVar15 = *param_3;
      if (*param_3 <= dVar22) {
        dVar15 = dVar22;
      }
      *param_3 = dVar15;
      *param_4 = dVar22 + *param_4;
    } while (lVar14 != (ulong)(iVar2 - 1) * 4 + 4);
  }
  dVar18 = (double)iVar1 * dVar18;
  *param_7 = *param_5 + *param_6 + dVar18 + dVar18 + dVar16 * (double)iVar3 + *param_7;
  dVar16 = (*param_8 + *param_8) - (double)iVar3;
  *param_10 = dVar16 + dVar16;
  *param_4 = *param_4 / (double)iVar2;
  return;
}


