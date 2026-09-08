/* VSDLSS function (decompiled by Ghidra) */
/* name: getFactorStatsOneChunk_vsdlss  addr: 00a160f0  size: 1e5 */
#include "vsdlss_ref.h"

/* signature: void getFactorStatsOneChunk_vsdlss(long param_1,double *param_2,double *param_3,double *param_4,double *param_5,double *param_6,double *param_7,double *param_8,double *param_9); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void getFactorStatsOneChunk_vsdlss
               (long param_1,double *param_2,double *param_3,double *param_4,double *param_5,
               double *param_6,double *param_7,double *param_8,double *param_9)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  double dVar7;
  int iVar8;
  int iVar9;
  int *piVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  
  iVar2 = *(int *)(param_1 + 0x14);
  iVar3 = *(int *)(param_1 + 8);
  lVar5 = *(long *)(param_1 + 0x28);
  lVar6 = *(long *)(param_1 + 0x20);
  *param_4 = 0.0;
  *param_7 = 0.0;
  *param_8 = 0.0;
  dVar7 = DAT_00ab7228;
  dVar14 = _DAT_00ab67f8;
  if (0 < iVar2) {
    piVar10 = (int *)(lVar6 + 4);
    iVar9 = 1;
    piVar1 = (int *)(lVar6 + 8 + (ulong)(iVar2 - 1) * 4);
    do {
      while (iVar4 = *piVar10, iVar4 != 1) {
        dVar12 = (double)iVar4;
        dVar13 = (double)*(int *)(lVar5 + (long)iVar9 * 4);
        *param_7 = (dVar13 * dVar12 - (dVar12 - dVar14) * dVar12 * dVar7) + *param_7;
        *param_4 = *param_4 + dVar13;
        if (0 < iVar4) {
          dVar12 = *param_8;
          iVar8 = 1;
          do {
            dVar11 = (double)iVar8;
            iVar8 = iVar8 + 1;
            dVar12 = dVar12 + ((dVar13 - dVar11) + dVar14) * (dVar13 - dVar11);
          } while (iVar8 != iVar4 + 1);
          *param_8 = dVar12;
        }
        piVar10 = piVar10 + 1;
        iVar9 = iVar9 + iVar4;
        if (piVar10 == piVar1) goto LAB_00a16249;
      }
      dVar12 = (double)*(int *)(lVar5 + (long)iVar9 * 4);
      piVar10 = piVar10 + 1;
      iVar9 = iVar9 + 1;
      *param_7 = *param_7 + dVar12;
      *param_4 = *param_4 + dVar12;
      *param_8 = (dVar12 - dVar14) * dVar12 + *param_8;
    } while (piVar10 != piVar1);
  }
LAB_00a16249:
  dVar7 = DAT_00ad4f20;
  dVar14 = DAT_00ab6798;
  dVar13 = (double)iVar3;
  *param_4 = *param_4 * DAT_00ad4f20;
  dVar12 = *param_7 * dVar14;
  dVar14 = dVar14 * dVar13;
  *param_5 = dVar12;
  *param_6 = dVar12 + *param_4 + (double)iVar2 * dVar7 + dVar7 * dVar13 + dVar14 + dVar14;
  dVar13 = (*param_7 + *param_7) - dVar13;
  *param_9 = dVar13 + dVar13;
  *param_2 = *param_6;
  *param_3 = *param_6;
  return;
}


