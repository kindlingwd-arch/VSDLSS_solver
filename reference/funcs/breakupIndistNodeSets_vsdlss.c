/* VSDLSS function (decompiled by Ghidra) */
/* name: breakupIndistNodeSets_vsdlss  addr: 00a225f0  size: 15a */
#include "vsdlss_ref.h"

/* signature: void breakupIndistNodeSets_vsdlss(double param_1,int param_2,long param_3,int *param_4); */

void breakupIndistNodeSets_vsdlss(double param_1,int param_2,long param_3,int *param_4)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  long *plVar4;
  int *piVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  int *piVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  
  lVar8 = 0;
  iVar6 = 0;
  iVar2 = *param_4;
  dVar10 = 0.0;
  if (0 < iVar2) {
    do {
      lVar8 = lVar8 + 1;
      piVar9 = *(int **)(*(long *)(param_4 + 2) + lVar8 * 8);
      if (piVar9 != (int *)0x0) {
        piVar5 = (int *)0x0;
        do {
          while( true ) {
            iVar3 = *piVar9;
            dVar11 = (double)*(int *)(param_3 + (long)iVar3 * 4);
            dVar12 = dVar10;
            if (piVar5 == (int *)0x0) {
              iVar6 = 0;
              dVar12 = DAT_00ad4f20 * dVar11;
            }
            dVar10 = dVar11 * DAT_00ab6798 + dVar12;
            if ((dVar10 <= param_1) && (iVar6 < param_2)) break;
            if (piVar5 == (int *)0x0) {
              nrerror_vsdlss(1,"memutils.c",0x62);
            }
            lVar7 = (long)iVar3 * 8;
            plVar4 = (long *)(lVar7 + *(long *)(param_4 + 2));
            if (*plVar4 != 0) {
              nrerror_vsdlss(1,"memutils.c",0x67);
              plVar4 = (long *)(lVar7 + *(long *)(param_4 + 2));
            }
            *plVar4 = (long)piVar9;
            piVar5[2] = 0;
            piVar5[3] = 0;
            piVar5 = (int *)0x0;
            dVar10 = dVar12;
          }
          piVar1 = piVar9 + 2;
          iVar6 = iVar6 + 1;
          piVar5 = piVar9;
          piVar9 = *(int **)piVar1;
        } while (*(int **)piVar1 != (int *)0x0);
      }
    } while ((int)lVar8 + 1 <= iVar2);
  }
  return;
}


