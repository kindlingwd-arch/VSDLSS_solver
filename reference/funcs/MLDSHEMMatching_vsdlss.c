/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDSHEMMatching_vsdlss  addr: 009fdd10  size: 205 */
#include "vsdlss_ref.h"

/* signature: int MLDSHEMMatching_vsdlss(int param_1,long param_2,long param_3,undefined8 param_4,long param_5,undefined8 param_6); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int MLDSHEMMatching_vsdlss
              (int param_1,long param_2,long param_3,undefined8 param_4,long param_5,
              undefined8 param_6)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  int iVar10;
  long lVar11;
  long lVar12;
  int *piVar13;
  int *piVar14;
  long lVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  
  lVar5 = *(long *)(param_2 + 0x28);
  lVar6 = *(long *)(param_2 + 0x38);
  iVar3 = *(int *)(param_2 + 8);
  iVar18 = *(int *)(param_2 + 0xc);
  lVar7 = *(long *)(param_2 + 0x58);
  lVar8 = *(long *)(param_2 + 0x48);
  if ((lVar8 == 0) || (lVar7 == 0)) {
    nrerror_vsdlss(1,"mldorder.c",0x57c);
  }
  iVar10 = (int)((double)((iVar18 * 2) / iVar3) * _DAT_00ec5da0);
  iVar18 = 1;
  if (0 < iVar10) {
    iVar18 = iVar10;
  }
  bucketNodesByDegree_vsdlss(iVar18 + 1,iVar3,param_4,lVar5,param_5,param_6);
  lVar9 = *(long *)(param_2 + 0x60);
  initIVector_vsdlss(param_3,(long)iVar3,0xffffffff);
  if (iVar3 < 1) {
    iVar18 = 0;
  }
  else {
    piVar14 = (int *)(param_5 + 4);
    iVar18 = 0;
    piVar2 = (int *)(param_5 + 8 + (ulong)(iVar3 - 1) * 4);
    do {
      while( true ) {
        iVar3 = *piVar14;
        lVar15 = (long)iVar3;
        lVar12 = lVar15 * 4;
        piVar1 = (int *)(param_3 + lVar12);
        if (*piVar1 != -1) break;
        iVar10 = *(int *)(lVar5 + lVar15 * 4);
        piVar13 = piVar1;
        iVar17 = iVar3;
        if (0 < iVar10) {
          iVar16 = 0;
          lVar12 = (ulong)(iVar10 - 1) * 4 + 4;
          lVar11 = 0;
          do {
            while( true ) {
              iVar10 = *(int *)(*(long *)(lVar6 + lVar15 * 8) + 4 + lVar11);
              iVar4 = *(int *)(*(long *)(lVar8 + lVar15 * 8) + 4 + lVar11);
              if ((*(int *)(param_3 + (long)iVar10 * 4) != -1) || (iVar4 <= iVar16)) break;
              if (*(int *)(lVar7 + (long)iVar10 * 4) <= param_1 - *(int *)(lVar7 + lVar15 * 4)) {
                iVar17 = iVar10;
                iVar16 = iVar4;
              }
              lVar11 = lVar11 + 4;
              if (lVar11 == lVar12) goto LAB_009fdea0;
            }
            lVar11 = lVar11 + 4;
          } while (lVar11 != lVar12);
LAB_009fdea0:
          lVar12 = (long)iVar17 * 4;
          piVar13 = (int *)(param_3 + lVar12);
        }
        iVar18 = iVar18 + 1;
        piVar14 = piVar14 + 1;
        *(int *)(lVar9 + lVar15 * 4) = iVar18;
        *(int *)(lVar9 + lVar12) = iVar18;
        *piVar13 = iVar3;
        *piVar1 = iVar17;
        if (piVar14 == piVar2) {
          return iVar18;
        }
      }
      piVar14 = piVar14 + 1;
    } while (piVar14 != piVar2);
  }
  return iVar18;
}


