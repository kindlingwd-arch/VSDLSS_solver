/* driver: ABIBQBryDXSPAIPP addr=009901d0 size=96 */
#include "vsdlss_ref.h"


undefined8 ABIBQBryDXSPAIPP(long param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  lVar1 = param_1 + 0x100;
  lVar2 = 0;
  if (-1 < (long)(&DAT_00e8be00)[lVar1]) {
    do {
      lVar3 = lVar2 + 1;
      BK_yMPRIyQBAdOBBXSPAIPP(*(undefined8 *)(lVar2 * 0x10 + (&DAT_00e8be00)[param_1] + 8));
      lVar2 = lVar3;
    } while (lVar3 <= (long)(&DAT_00e8be00)[lVar1]);
  }
  BK_yMPRIyQBAdOBBXSPAIPP((&DAT_00e8be00)[param_1]);
  (&DAT_00e8be00)[param_1] = 0;
  (&DAT_00e8ce00)[param_1] = 0xffffffffffffffff;
  (&DAT_00e8d600)[param_1] = 0;
  (&DAT_00e8de00)[param_1] = 0;
  (&DAT_00e8be00)[lVar1] = 0xffffffffffffffff;
  return 0;
}


