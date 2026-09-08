/* driver: MOFKQgtB_QLOrLjLDXSPAIPP addr=0098d6d0 size=7d */
#include "vsdlss_ref.h"


void MOFKQgtB_QLOrLjLDXSPAIPP(undefined8 param_1,long param_2,long param_3)

{
  long lVar1;
  long lVar2;
  
  MOFKQrLjLDXSPAIPP("\n  Begin %s of Size: %ld\n",param_1);
  MOFKQrLjLDXSPAIPP("\tIndex\tValue\n");
  MOFKQrLjLDXSPAIPP("\t-----\t-----\n");
  lVar1 = 1;
  if (0 < param_3) {
    do {
      lVar2 = lVar1 + 1;
      MOFKQrLjLDXSPAIPP(&DAT_00b1d997,lVar1,*(undefined8 *)(param_2 + lVar1 * 8));
      lVar1 = lVar2;
    } while (lVar2 <= param_3);
  }
  MOFKQrLjLDXSPAIPP("  End   %s\n\n",param_1);
  return;
}


