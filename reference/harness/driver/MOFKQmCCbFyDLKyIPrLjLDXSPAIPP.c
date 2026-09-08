/* driver: MOFKQmCCbFyDLKyIPrLjLDXSPAIPP addr=0098d870 size=86 */
#include "vsdlss_ref.h"


void MOFKQmCCbFyDLKyIPrLjLDXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  long lVar2;
  
  MOFKQrLjLDXSPAIPP("\n  Begin OffDiagonals (Number: %ld)\n",param_4);
  MOFKQrLjLDXSPAIPP("\tIndex\tFrom\tTo\toffDiagonal\n");
  MOFKQrLjLDXSPAIPP("\t-----\t----\t--\t-----------\n");
  lVar1 = 1;
  if (0 < param_4) {
    do {
      lVar2 = lVar1 + 1;
      MOFKQrLjLDXSPAIPP(*(undefined8 *)(param_3 + lVar1 * 8),&DAT_00b1caba,lVar1,
                        *(undefined8 *)(param_1 + lVar1 * 8),*(undefined8 *)(param_2 + lVar1 * 8));
      lVar1 = lVar2;
    } while (lVar2 <= param_4);
  }
  MOFKQrLjLDXSPAIPP("  End   OffDiagonals\n\n");
  return;
}


