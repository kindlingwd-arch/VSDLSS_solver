/* driver: MOFKQbtB_QLOrLjLDXSPAIPP addr=0098d750 size=82 */
#include "vsdlss_ref.h"


void MOFKQbtB_QLOrLjLDXSPAIPP(undefined8 param_1,long param_2,long param_3)

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
      MOFKQrLjLDXSPAIPP(*(undefined8 *)(param_2 + lVar1 * 8),"\t%ld\t%11.8g\n",lVar1);
      lVar1 = lVar2;
    } while (lVar2 <= param_3);
  }
  MOFKQrLjLDXSPAIPP("  End   %s\n\n",param_1);
  return;
}


