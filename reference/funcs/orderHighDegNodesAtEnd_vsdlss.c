/* VSDLSS function (decompiled by Ghidra) */
/* name: orderHighDegNodesAtEnd_vsdlss  addr: 00a04ee0  size: 2b6 */
#include "vsdlss_ref.h"

/* signature: int orderHighDegNodesAtEnd_vsdlss(long param_1,int param_2,long param_3,int *param_4,long param_5); */

int orderHighDegNodesAtEnd_vsdlss(long param_1,int param_2,long param_3,int *param_4,long param_5)

{
  int iVar1;
  int iVar2;
  long lVar3;
  int *piVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  long lVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  long lVar14;
  double dVar15;
  
  iVar11 = 0;
  iVar1 = *(int *)(param_1 + 8);
  lVar10 = *(long *)(param_1 + 0x28);
  if (999 < iVar1) {
    lVar3 = newCVector_vsdlss((long)iVar1,1,0);
    if (0 < param_2) {
      piVar4 = (int *)(param_3 + 4);
      do {
        iVar11 = *piVar4;
        piVar4 = piVar4 + 1;
        *(undefined1 *)(lVar3 + iVar11) = 1;
      } while (piVar4 != (int *)(param_3 + 8 + (ulong)(param_2 - 1) * 4));
    }
    iVar12 = ((*(int *)(param_1 + 0xc) * 2) / iVar1) * 0x1e;
    dVar15 = pow((double)iVar1,DAT_00ab7228);
    dVar15 = floor(dVar15);
    iVar11 = 10;
    if (9 < iVar12) {
      iVar11 = iVar12;
    }
    lVar5 = 0;
    iVar12 = 0;
    do {
      if (iVar11 < *(int *)(lVar10 + 4 + lVar5 * 4)) {
        iVar12 = iVar12 + (uint)(*(char *)(lVar3 + 1 + lVar5) == '\0');
      }
      lVar5 = lVar5 + 1;
    } while (lVar5 != (ulong)(iVar1 - 1) + 1);
    lVar14 = (long)iVar12;
    lVar5 = newIVector_vsdlss(lVar14,0,0);
    lVar6 = newIVector_vsdlss(lVar14,0,0);
    lVar7 = newIVector_vsdlss(lVar14,0,0);
    lVar14 = newIVector_vsdlss(lVar14,0,0);
    iVar13 = 0;
    lVar8 = 0;
    iVar9 = 1;
    do {
      iVar2 = *(int *)(lVar10 + 4 + lVar8 * 4);
      if ((iVar11 < iVar2) && (*(char *)(lVar3 + 1 + lVar8) == '\0')) {
        iVar13 = iVar13 + 1;
        *(int *)(lVar14 + (long)iVar13 * 4) = iVar2;
        *(int *)(lVar7 + (long)iVar13 * 4) = iVar9;
      }
      iVar9 = iVar9 + 1;
      lVar8 = lVar8 + 1;
    } while (iVar9 != iVar1 + 1);
    if (iVar12 != iVar13) {
      nrerror_vsdlss(1,"mldorder.c",0x13e3);
    }
    iVar11 = 0;
    if (iVar12 != 0) {
      casiRadixSortPerm_vsdlss(iVar12,lVar5,lVar14,lVar7,lVar6);
      iVar11 = 10;
      if (9 < (int)dVar15) {
        iVar11 = (int)dVar15;
      }
      if (iVar12 <= iVar11) {
        iVar11 = iVar12;
      }
      lVar10 = 0;
      do {
        *(undefined4 *)
         (param_5 + (long)iVar11 * -4 + (long)((iVar1 + 1) - param_2) * 4 + lVar10 * 4) =
             *(undefined4 *)((long)iVar11 * -4 + (long)(iVar12 + 1) * 4 + lVar7 + lVar10 * 4);
        iVar9 = (int)lVar10;
        lVar10 = lVar10 + 1;
      } while (iVar9 + 2 <= iVar11);
      *param_4 = *param_4 + iVar11;
    }
    if (lVar3 != 0) {
      casiFree_vsdlss(lVar3);
    }
    if (lVar5 != 0) {
      casiFree_vsdlss();
    }
    if (lVar6 != 0) {
      casiFree_vsdlss();
    }
    if (lVar7 != 0) {
      casiFree_vsdlss(lVar7);
    }
    if (lVar14 != 0) {
      casiFree_vsdlss(lVar14);
    }
  }
  return iVar11;
}


