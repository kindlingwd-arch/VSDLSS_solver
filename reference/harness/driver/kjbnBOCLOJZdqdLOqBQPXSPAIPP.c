/* driver: kjbnBOCLOJZdqdLOqBQPXSPAIPP addr=0099b5b0 size=24b */
#include "vsdlss_ref.h"


void kjbnBOCLOJZdqdLOqBQPXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long *param_5,long param_6)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long *plVar10;
  long lVar11;
  long lVar12;
  long local_50;
  
  lVar2 = *(long *)(param_3 + 0x50);
  lVar3 = *(long *)(param_6 + 0x18);
  lVar4 = *(long *)(param_6 + 0x10);
  if (param_1 == 8) {
    lVar9 = 1;
    lVar11 = 1;
    if (*(long *)(param_2 + 8) < 1) {
      return;
    }
    do {
      if (*(long *)(param_4 + lVar11 * 8) == 0) {
        param_5[lVar11] = 1;
        if (lVar4 < lVar9) {
          KOBOOLOXSPAIPP(1,"mldorder.c",0xcbb);
        }
        *(long *)(lVar3 + lVar9 * 8) = lVar11;
        lVar9 = lVar9 + 1;
      }
      lVar11 = lVar11 + 1;
    } while (lVar11 <= *(long *)(param_2 + 8));
    lVar11 = 4;
    lVar12 = 1;
  }
  else {
    lVar11 = *(long *)(param_2 + 8) + 1;
    if (*(long *)(param_2 + 0x10) < lVar11) {
      return;
    }
    lVar9 = 1;
    do {
      if (*(long *)(param_4 + lVar11 * 8) == 0) {
        param_5[lVar11] = 6;
        if (lVar4 < lVar9) {
          KOBOOLOXSPAIPP(1,"mldorder.c",0xcca);
        }
        *(long *)(lVar3 + lVar9 * 8) = lVar11;
        lVar9 = lVar9 + 1;
      }
      lVar11 = lVar11 + 1;
    } while (lVar11 <= *(long *)(param_2 + 0x10));
    lVar11 = 3;
    lVar12 = 6;
  }
  if (lVar9 != 1) {
    local_50 = 1;
    do {
      lVar1 = local_50 + 1;
      lVar5 = *(long *)(lVar3 + local_50 * 8);
      lVar6 = *(long *)(lVar2 + lVar5 * 8);
      lVar5 = *(long *)(*(long *)(param_3 + 0x60) + lVar5 * 8);
      if (0 < lVar6) {
        lVar8 = 1;
        do {
          lVar7 = *(long *)(lVar5 + lVar8 * 8);
          if (param_5[lVar7] != lVar11) {
            param_5[lVar7] = lVar11;
            lVar7 = *(long *)(param_4 + lVar7 * 8);
            plVar10 = param_5;
            if ((lVar7 == 0) || (plVar10 = param_5 + lVar7, lVar12 == *plVar10)) {
              KOBOOLOXSPAIPP(1,"mldorder.c",0xcdf);
              *plVar10 = lVar12;
            }
            else {
              *plVar10 = lVar12;
            }
            if (lVar4 < lVar9) {
              KOBOOLOXSPAIPP(1,"mldorder.c",0xce6);
            }
            *(long *)(lVar3 + lVar9 * 8) = lVar7;
            lVar9 = lVar9 + 1;
          }
          lVar8 = lVar8 + 1;
        } while (lVar8 <= lVar6);
      }
      local_50 = lVar1;
    } while (lVar1 < lVar9);
  }
  return;
}


