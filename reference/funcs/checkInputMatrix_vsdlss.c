/* VSDLSS function (decompiled by Ghidra) */
/* name: checkInputMatrix_vsdlss  addr: 009f5ba0  size: 228 */
#include "vsdlss_ref.h"

/* signature: void checkInputMatrix_vsdlss(int *param_1); */

void checkInputMatrix_vsdlss(int *param_1)

{
  int iVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  bool bVar6;
  int iVar7;
  double *pdVar8;
  int iVar9;
  long lVar10;
  double dVar11;
  double local_50;
  double local_48;
  
  iVar1 = param_1[1];
  iVar2 = *param_1;
  lVar3 = *(long *)(param_1 + 6);
  lVar4 = *(long *)(param_1 + 10);
  lVar5 = *(long *)(param_1 + 0xc);
  pdVar8 = *(double **)(param_1 + 0xe);
  if (iVar1 < 1) {
    local_50 = 0.0;
  }
  else {
    iVar9 = 1;
    local_50 = 0.0;
    do {
      pdVar8 = pdVar8 + 1;
      dVar11 = *pdVar8;
      if (dVar11 < 0.0) {
        nrerror_vsdlss(0x15,"interface.c",0x194);
        dVar11 = *pdVar8;
      }
      local_50 = dVar11 + local_50;
      iVar9 = iVar9 + 1;
    } while (iVar9 <= iVar1);
  }
  if (iVar2 < 1) {
    local_48 = 0.0;
  }
  else {
    bVar6 = false;
    local_48 = 0.0;
    lVar10 = 0;
    do {
      iVar9 = *(int *)(lVar3 + 4 + lVar10 * 4);
      iVar7 = *(int *)(lVar4 + 4 + lVar10 * 4);
      if (iVar9 == iVar7) {
        nrerror_vsdlss(0x1d,"interface.c",0x19d,iVar9,iVar9,iVar1);
        iVar9 = *(int *)(lVar3 + 4 + lVar10 * 4);
        iVar7 = *(int *)(lVar4 + 4 + lVar10 * 4);
      }
      if ((iVar9 < 1) || (iVar1 < iVar9)) {
        nrerror_vsdlss(0x1d,"interface.c",0x1a4,iVar9,iVar7,iVar1);
        iVar7 = *(int *)(lVar4 + 4 + lVar10 * 4);
      }
      if ((iVar7 < 1) || (iVar1 < iVar7)) {
        nrerror_vsdlss(0x1d,"interface.c",0x1ab,*(undefined4 *)(lVar3 + 4 + lVar10 * 4),iVar7,iVar1)
        ;
      }
      dVar11 = *(double *)(lVar5 + 8 + lVar10 * 8);
      if (0.0 < dVar11) {
        if (bVar6) {
          bVar6 = true;
        }
        else {
          bVar6 = true;
          casiWarning_vsdlss(0x21,"interface.c",0x1b3,*(undefined4 *)(lVar3 + 4 + lVar10 * 4),
                             *(undefined4 *)(lVar4 + 4 + lVar10 * 4));
          dVar11 = *(double *)(lVar5 + 8 + lVar10 * 8);
        }
      }
      iVar9 = (int)lVar10;
      local_48 = dVar11 + dVar11 + local_48;
      lVar10 = lVar10 + 1;
    } while (iVar9 + 2 <= iVar2);
  }
  if (0.0 < local_50 + local_48) {
    return;
  }
  nrerror_vsdlss(0x1c,"interface.c",0x1c4);
  return;
}


