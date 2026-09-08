/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDUpdateNodeAdjArray_vsdlss  addr: 00a28100  size: 2b1 */
#include "vsdlss_ref.h"

/* signature: int MLDUpdateNodeAdjArray_vsdlss(int param_1,long param_2,long param_3,long param_4,int *param_5); */

int MLDUpdateNodeAdjArray_vsdlss(int param_1,long param_2,long param_3,long param_4,int *param_5)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  int iVar8;
  long lVar9;
  int *piVar10;
  long lVar11;
  long lVar12;
  int iVar13;
  int iVar14;
  long lVar15;
  int *piVar16;
  int local_40;
  
  lVar12 = *(long *)(param_3 + 0x38);
  lVar5 = *(long *)(param_5 + 2);
  lVar6 = *(long *)(param_3 + 0x28);
  lVar9 = (long)param_1;
  lVar7 = *(long *)(lVar12 + lVar9 * 8);
  if (-0x3ffffffa < *(int *)(param_2 + lVar9 * 4)) {
    nrerror_vsdlss(1,"mldmmd.c",0x9b);
  }
  piVar10 = (int *)(lVar9 * 4 + lVar6);
  iVar2 = *param_5;
  *(int *)(lVar5 + lVar9 * 4) = iVar2;
  iVar14 = *piVar10;
  if (iVar14 < 1) {
    iVar14 = 0;
    local_40 = 0;
  }
  else {
    lVar11 = 0;
    iVar13 = 0;
    local_40 = 0;
    do {
      while( true ) {
        iVar8 = *(int *)(lVar7 + 4 + lVar11 * 4);
        iVar4 = (int)lVar11;
        if (-0x3ffffffa < *(int *)(param_2 + (long)iVar8 * 4)) break;
        iVar13 = iVar13 + 1;
        *(int *)(lVar7 + (long)iVar13 * 4) = iVar8;
LAB_00a2819a:
        lVar11 = lVar11 + 1;
        if (iVar14 < iVar4 + 2) goto LAB_00a28260;
      }
      iVar8 = MLDFindWithPathCompression_vsdlss(iVar8,param_4);
      lVar15 = (long)iVar8;
      if (0x3ffffff8 < *(int *)(param_2 + lVar15 * 4) + 0x3ffffff9U) {
        nrerror_vsdlss(1,"mldmmd.c",0xb0);
      }
      iVar3 = *(int *)(lVar6 + lVar15 * 4);
      if ((iVar3 < 2) || (piVar16 = (int *)(lVar15 * 4 + lVar5), iVar2 <= *piVar16))
      goto LAB_00a2819a;
      iVar13 = iVar13 + 1;
      *piVar16 = iVar2;
      *(int *)(lVar7 + (long)iVar13 * 4) = iVar8;
      lVar15 = *(long *)(lVar12 + lVar15 * 8);
      piVar16 = (int *)(lVar15 + 4);
      do {
        piVar1 = (int *)(lVar5 + (long)*piVar16 * 4);
        if (*piVar1 < iVar2) {
          *piVar1 = iVar2;
          local_40 = local_40 + 1;
        }
        piVar16 = piVar16 + 1;
      } while (piVar16 != (int *)(lVar15 + 8 + (ulong)(iVar3 - 1) * 4));
      lVar11 = lVar11 + 1;
    } while (iVar4 + 2 <= iVar14);
LAB_00a28260:
    *piVar10 = iVar13;
    if (iVar13 == 0) {
      iVar14 = 0;
    }
    else {
      lVar12 = 0;
      iVar14 = 0;
      do {
        while( true ) {
          iVar8 = *(int *)(lVar7 + 4 + lVar12 * 4);
          iVar4 = *(int *)(param_2 + (long)iVar8 * 4);
          iVar3 = (int)lVar12;
          if (-0x3ffffffa < iVar4) break;
          if (*(int *)(lVar5 + (long)iVar8 * 4) < iVar2) {
            iVar14 = iVar14 + 1;
            local_40 = local_40 + 1;
            *(int *)(lVar7 + (long)iVar14 * 4) = iVar8;
          }
LAB_00a282a3:
          lVar12 = lVar12 + 1;
          if (iVar13 < iVar3 + 2) goto LAB_00a282ea;
        }
        if (0x3ffffff8 < iVar4 + 0x3ffffff9U) {
          nrerror_vsdlss(1,"mldmmd.c",0xdb);
          goto LAB_00a282a3;
        }
        iVar14 = iVar14 + 1;
        lVar12 = lVar12 + 1;
        *(int *)(lVar7 + (long)iVar14 * 4) = iVar8;
      } while (iVar3 + 2 <= iVar13);
    }
  }
LAB_00a282ea:
  *piVar10 = iVar14;
  *(undefined4 *)(param_4 + lVar9 * 4) = 0;
  unmarkCountingMark_vsdlss(param_5);
  return local_40;
}


