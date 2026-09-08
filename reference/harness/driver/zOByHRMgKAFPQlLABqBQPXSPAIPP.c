/* driver: zOByHRMgKAFPQlLABqBQPXSPAIPP addr=00990570 size=179 */
#include "vsdlss_ref.h"


void zOByHRMgKAFPQlLABqBQPXSPAIPP(double param_1,long param_2,long param_3,long *param_4)

{
  long lVar1;
  long *plVar2;
  long lVar3;
  long *plVar4;
  long *plVar5;
  long lVar6;
  long lVar7;
  double dVar8;
  long local_48;
  
  lVar1 = *param_4;
  local_48 = 1;
  if (0 < lVar1) {
    do {
      plVar4 = *(long **)(param_4[1] + local_48 * 8);
      if (plVar4 != (long *)0x0) {
LAB_009905d0:
        plVar5 = (long *)0x0;
        lVar3 = *plVar4;
        lVar7 = lVar3 * 8;
        do {
          dVar8 = (double)*(long *)(param_3 + lVar3 * 8) * DAT_00ab6798;
          lVar6 = 0;
          dVar8 = dVar8 + dVar8;
          plVar2 = plVar5;
          plVar5 = plVar4;
          while( true ) {
            if ((param_1 < dVar8) || (param_2 <= lVar6)) {
              if (plVar2 == (long *)0x0) {
                KOBOOLOXSPAIPP(1,"memutils.c",0x62);
              }
              plVar4 = (long *)(lVar7 + param_4[1]);
              if (*plVar4 != 0) {
                KOBOOLOXSPAIPP(1,"memutils.c",0x67);
                plVar4 = (long *)(lVar7 + param_4[1]);
              }
              *plVar4 = (long)plVar5;
              plVar2[1] = 0;
              plVar4 = plVar5;
              goto LAB_009905d0;
            }
            plVar4 = (long *)plVar5[1];
            lVar6 = lVar6 + 1;
            if (plVar4 == (long *)0x0) goto LAB_009906a0;
            lVar3 = *plVar4;
            lVar7 = lVar3 * 8;
            if (plVar5 == (long *)0x0) break;
            dVar8 = (double)*(long *)(param_3 + lVar3 * 8) * DAT_00ab6798 + dVar8;
            plVar2 = plVar5;
            plVar5 = plVar4;
          }
        } while( true );
      }
LAB_009906a0:
      local_48 = local_48 + 1;
    } while (local_48 <= lVar1);
  }
  return;
}


