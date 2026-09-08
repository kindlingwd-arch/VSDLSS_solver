/* driver: _yPFdOBBXSPAIPP addr=0096f2f0 size=77 */
#include "vsdlss_ref.h"


void _yPFdOBBXSPAIPP(long param_1)

{
  bool bVar1;
  long *plVar2;
  long *plVar3;
  long *plVar4;
  
  bVar1 = false;
  plVar3 = (long *)0x0;
  plVar4 = DAT_00e5a528;
  while (DAT_00e5a528 != (long *)0x0) {
    while( true ) {
      plVar2 = DAT_00e5a528;
      if (bVar1) goto LAB_0096f321;
      if (*plVar2 == param_1) break;
      bVar1 = false;
      DAT_00e5a528 = (long *)plVar2[1];
      plVar3 = plVar2;
      if ((long *)plVar2[1] == (long *)0x0) goto LAB_0096f321;
    }
    DAT_00e5a528 = (long *)plVar2[1];
    if (plVar3 == (long *)0x0) {
      plVar2[1] = (long)DAT_00e5a538;
      bVar1 = true;
      DAT_00e5a538 = plVar2;
      plVar4 = DAT_00e5a528;
    }
    else {
      plVar3[1] = (long)DAT_00e5a528;
      plVar2[1] = (long)DAT_00e5a538;
      bVar1 = true;
      DAT_00e5a528 = (long *)plVar3[1];
      DAT_00e5a538 = plVar2;
    }
  }
LAB_0096f321:
  DAT_00e5a528 = plVar4;
  BK_yMPRIyQBAdOBBXSPAIPP();
  return;
}


