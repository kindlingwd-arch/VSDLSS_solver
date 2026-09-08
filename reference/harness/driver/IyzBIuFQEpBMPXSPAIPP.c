/* driver: IyzBIuFQEpBMPXSPAIPP addr=009a40e0 size=111 */
#include "vsdlss_ref.h"


void IyzBIuFQEpBMPXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,long param_6,
               long param_7)

{
  undefined8 *puVar1;
  long *plVar2;
  long lVar3;
  undefined8 *puVar4;
  long lVar5;
  long lVar6;
  long *plVar7;
  bool bVar8;
  
  puVar4 = (undefined8 *)(param_7 + 8);
  puVar1 = puVar4 + param_4;
  if (0 < param_4) {
    do {
      *puVar4 = 0;
      puVar4 = puVar4 + 1;
    } while (puVar4 != puVar1);
  }
  plVar7 = (long *)(param_6 + param_3 * 8);
  lVar6 = param_3;
  lVar5 = param_3;
  if (0 < param_3) {
LAB_009a4148:
    do {
      lVar3 = *(long *)(param_6 + lVar6 * 8);
      if (lVar3 != lVar6) {
        lVar6 = lVar3;
        if (lVar3 <= param_3) goto LAB_009a4148;
      }
      lVar6 = *plVar7;
      plVar2 = plVar7;
      if (lVar5 != *plVar7) {
        do {
          if (param_3 < lVar6) break;
          *plVar2 = lVar3;
          plVar2 = (long *)(param_6 + lVar6 * 8);
          bVar8 = *plVar2 != lVar6;
          lVar6 = *plVar2;
        } while (bVar8);
      }
      plVar7 = plVar7 + -1;
      lVar5 = lVar5 + -1;
      lVar6 = lVar5;
    } while (lVar5 != 0);
    lVar6 = 1;
    do {
      while( true ) {
        lVar5 = *(long *)(param_6 + lVar6 * 8);
        if ((param_5 < lVar5) || (lVar5 < 1)) {
          KOBOOLOXSPAIPP(1,"prepostpar.c",0x315);
        }
        if (param_1 < lVar6) break;
        plVar7 = (long *)(param_7 + lVar5 * 8);
        *plVar7 = *plVar7 + 1;
LAB_009a419d:
        lVar6 = lVar6 + 1;
        if (param_3 < lVar6) {
          return;
        }
      }
      if (param_2 < lVar6) {
        plVar7 = (long *)(param_7 + lVar5 * 8);
        *plVar7 = *plVar7 + 3;
        goto LAB_009a419d;
      }
      lVar6 = lVar6 + 1;
      plVar7 = (long *)(param_7 + lVar5 * 8);
      *plVar7 = *plVar7 + 2;
    } while (lVar6 <= param_3);
  }
  return;
}


