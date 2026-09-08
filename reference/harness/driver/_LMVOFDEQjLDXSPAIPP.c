/* driver: _LMVOFDEQjLDXSPAIPP addr=00965560 size=49 */
#include "vsdlss_ref.h"


void _LMVOFDEQjLDXSPAIPP(void)

{
  long lVar1;
  long lVar2;
  
  lVar2 = 1;
  do {
    while (MOFKQrLjLDXSPAIPP(&DAT_00acae72,*(undefined8 *)(_LMVOFDEQjFKBPXSPAIPP + lVar2 * 8)),
          qgjclrXSPAIPP != 0) {
      lVar2 = lVar2 + 1;
      if (lVar2 == 0x14) {
        return;
      }
    }
    lVar1 = lVar2 * 8;
    lVar2 = lVar2 + 1;
    puts(*(char **)(_LMVOFDEQjFKBPXSPAIPP + lVar1));
  } while (lVar2 != 0x14);
  return;
}


