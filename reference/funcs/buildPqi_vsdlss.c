/* VSDLSS function (decompiled by Ghidra) */
/* name: buildPqi_vsdlss  addr: 00a0d310  size: 157 */
#include "vsdlss_ref.h"

/* signature: void buildPqi_vsdlss(long param_1,int param_2); */

void buildPqi_vsdlss(long param_1,int param_2)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  int iVar11;
  int iVar12;
  int *piVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  
  *(int *)(param_1 + 8) = param_2;
  if (0 < param_2) {
    piVar13 = *(int **)(param_1 + 0x18);
    lVar8 = *(long *)(param_1 + 0x10);
    iVar11 = 1;
    do {
      piVar13 = piVar13 + 1;
      *(int *)(lVar8 + (long)*piVar13 * 4) = iVar11;
      iVar11 = iVar11 + 1;
    } while (iVar11 != param_2 + 1);
    param_2 = *(int *)(param_1 + 8);
  }
  iVar11 = param_2 / 2;
  if (iVar11 < 1) {
    return;
  }
  iVar17 = iVar11 * 2;
  do {
    if (iVar17 <= param_2) {
      lVar8 = *(long *)(param_1 + 0x18);
      lVar9 = *(long *)(param_1 + 0x20);
      iVar3 = iVar17;
      iVar15 = iVar11;
      do {
        iVar12 = iVar3;
        if (iVar3 < param_2) {
          piVar1 = (int *)(lVar8 + (long)iVar3 * 4);
          piVar2 = (int *)(lVar8 + 4 + (long)iVar3 * 4);
          iVar4 = *piVar1;
          iVar5 = *piVar2;
          iVar6 = *(int *)(lVar9 + (long)iVar4 * 4);
          iVar7 = *(int *)(lVar9 + (long)iVar5 * 4);
          piVar13 = piVar1;
          iVar14 = iVar4;
          iVar16 = iVar6;
          if (((iVar7 <= iVar6) &&
              (piVar13 = piVar2, iVar14 = iVar5, iVar16 = iVar7, iVar12 = iVar3 + 1, iVar6 <= iVar7)
              ) && (piVar13 = piVar1, iVar14 = iVar4, iVar16 = iVar6, iVar12 = iVar3, iVar5 < iVar4)
             ) {
            piVar13 = piVar2;
            iVar14 = iVar5;
            iVar16 = iVar7;
            iVar12 = iVar3 + 1;
          }
        }
        else {
          piVar13 = (int *)(lVar8 + (long)iVar3 * 4);
          iVar14 = *piVar13;
          iVar16 = *(int *)(lVar9 + (long)iVar14 * 4);
        }
        piVar1 = (int *)(lVar8 + (long)iVar15 * 4);
        iVar3 = *piVar1;
        iVar4 = *(int *)(lVar9 + (long)iVar3 * 4);
        if (((iVar4 <= iVar16) && ((iVar4 < iVar16 || (iVar3 < iVar14)))) &&
           (piVar13 = piVar1, iVar14 = iVar3, iVar12 != iVar15)) break;
        *piVar1 = iVar14;
        *piVar13 = iVar3;
        lVar10 = *(long *)(param_1 + 0x10);
        *(int *)(lVar10 + (long)*piVar1 * 4) = iVar15;
        iVar3 = iVar12 * 2;
        *(int *)(lVar10 + (long)*piVar13 * 4) = iVar12;
        param_2 = *(int *)(param_1 + 8);
        iVar15 = iVar12;
      } while (iVar3 <= param_2);
    }
    iVar17 = iVar17 + -2;
    iVar11 = iVar11 + -1;
    if (iVar11 == 0) {
      return;
    }
    param_2 = *(int *)(param_1 + 8);
  } while( true );
}


