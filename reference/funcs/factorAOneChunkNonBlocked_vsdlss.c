/* VSDLSS function (decompiled by Ghidra) */
/* name: factorAOneChunkNonBlocked_vsdlss  addr: 00a19200  size: 291 */
#include "vsdlss_ref.h"

/* signature: void factorAOneChunkNonBlocked_vsdlss(long param_1); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void factorAOneChunkNonBlocked_vsdlss(long param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  ulong uVar10;
  long lVar11;
  long lVar12;
  int iVar13;
  long lVar14;
  int iVar15;
  long lVar16;
  double dVar17;
  double dVar18;
  long local_50;
  int local_40;
  
  uVar10 = DAT_00abd200;
  iVar1 = *(int *)(param_1 + 8);
  lVar4 = *(long *)(param_1 + 0x30);
  lVar5 = *(long *)(param_1 + 0x38);
  lVar6 = *(long *)(param_1 + 0x28);
  if (0 < iVar1) {
    local_50 = 0;
    local_40 = 1;
    do {
      lVar16 = *(long *)(lVar5 + 8 + local_50 * 2);
      dVar18 = *(double *)(lVar16 + 8);
      if (dVar18 <= 0.0) {
        nrerror_vsdlss(dVar18,0x23,"vsdlss1memutils.c",0x883,local_40);
        lVar16 = *(long *)(lVar5 + 8 + local_50 * 2);
      }
      iVar2 = *(int *)(lVar6 + 4 + local_50);
      dVar18 = _DAT_00ab67f8 / dVar18;
      if (1 < iVar2) {
        iVar13 = iVar2 + -1;
        lVar8 = *(long *)(lVar4 + 8 + local_50 * 2);
        do {
          lVar11 = (long)*(int *)(lVar8 + 8);
          iVar3 = *(int *)(lVar6 + lVar11 * 4);
          lVar7 = *(long *)(lVar5 + lVar11 * 8);
          dVar17 = (double)(*(ulong *)(lVar16 + 0x10) ^ uVar10) * dVar18;
          if (iVar13 == iVar3) {
            lVar11 = 0;
            if (0 < iVar13) {
              do {
                *(double *)(lVar7 + 8 + lVar11) =
                     *(double *)(lVar16 + 0x10 + lVar11) * dVar17 + *(double *)(lVar7 + 8 + lVar11);
                lVar11 = lVar11 + 8;
              } while (lVar11 != (ulong)(iVar13 - 1) * 8 + 8);
            }
          }
          else if (iVar13 < iVar3) {
            lVar11 = *(long *)(lVar4 + lVar11 * 8);
            if ((iVar3 < 1) || (iVar13 < 1)) {
              iVar15 = 1;
            }
            else {
              lVar12 = 0;
              iVar15 = 1;
              do {
                lVar14 = (long)iVar15;
                if (*(int *)(lVar8 + 4 + lVar14 * 4) == *(int *)(lVar11 + 4 + lVar12 * 4)) {
                  iVar15 = iVar15 + 1;
                  *(double *)(lVar7 + 8 + lVar12 * 8) =
                       *(double *)(lVar16 + 8 + lVar14 * 8) * dVar17 +
                       *(double *)(lVar7 + 8 + lVar12 * 8);
                }
                iVar9 = (int)lVar12;
                lVar12 = lVar12 + 1;
              } while (iVar15 <= iVar13 && iVar9 + 2 <= iVar3);
            }
            if (iVar15 <= iVar13) {
              nrerror_vsdlss(0x25,"vsdlss1memutils.c",0x8a5);
            }
          }
          else {
            nrerror_vsdlss(1,"vsdlss1memutils.c",0x8aa);
          }
          iVar13 = iVar13 + -1;
          lVar8 = lVar8 + 4;
          lVar16 = lVar16 + 8;
        } while ((iVar2 + 1) - iVar13 <= iVar2);
      }
      local_40 = local_40 + 1;
      local_50 = local_50 + 4;
    } while (local_40 <= iVar1);
  }
  return;
}


