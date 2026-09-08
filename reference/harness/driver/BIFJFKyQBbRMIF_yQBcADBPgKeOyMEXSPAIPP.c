/* driver: BIFJFKyQBbRMIF_yQBcADBPgKeOyMEXSPAIPP addr=00989310 size=db */
#include "vsdlss_ref.h"


void BIFJFKyQBbRMIF_yQBcADBPgKeOyMEXSPAIPP(long *param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  undefined8 uVar5;
  long *plVar6;
  long *plVar7;
  long lVar8;
  long lVar9;
  
  lVar8 = 0;
  lVar2 = *param_1;
  lVar3 = KBTjFPQkyOHXSPAIPP(lVar2);
  if (0 < lVar2) {
    lVar9 = 1;
    do {
      lVar1 = lVar9 * 8;
      plVar6 = (long *)(lVar1 + param_1[4]);
      plVar7 = (long *)*plVar6;
      if (plVar7 == (long *)0x0) {
        lVar4 = 0;
      }
      else {
        do {
          while( true ) {
            if (*(char *)(*(long *)(lVar3 + 8) + *plVar7) != '\0') break;
            lVar8 = lVar8 + 1;
            JyOHjFPQkyOHXSPAIPP(lVar3);
            plVar7 = (long *)plVar7[1];
            if (plVar7 == (long *)0x0) goto LAB_0098939b;
          }
          plVar7 = (long *)plVar7[1];
        } while (plVar7 != (long *)0x0);
LAB_0098939b:
        plVar6 = (long *)(lVar1 + param_1[4]);
        lVar4 = *plVar6;
      }
      lVar9 = lVar9 + 1;
      lVar4 = ABIBQBcKQFOBacADBIFPQXSPAIPP(lVar4);
      *plVar6 = lVar4;
      lVar4 = param_1[4];
      uVar5 = RKJyOHjFPQkyOHpBQROKjFPQXSPAIPP();
      *(undefined8 *)(lVar1 + lVar4) = uVar5;
    } while (lVar9 <= lVar2);
  }
  ABIBQBjFPQkyOHXSPAIPP(lVar3);
  param_1[1] = lVar8;
  return;
}


