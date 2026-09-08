/* driver: _EB_HdLOlBDyQFSBbFyDPXSPAIPP addr=0098abe0 size=67 */
#include "vsdlss_ref.h"


void _EB_HdLOlBDyQFSBbFyDPXSPAIPP(long param_1,long param_2)

{
  double *pdVar1;
  long lVar2;
  
  lVar2 = 1;
  if (0 < param_1) {
    do {
      while (pdVar1 = (double *)(param_2 + lVar2 * 8), 0.0 < *pdVar1 || *pdVar1 == 0.0) {
        lVar2 = lVar2 + 1;
        if (param_1 < lVar2) {
          return;
        }
      }
      lVar2 = lVar2 + 1;
      KOBOOLOXSPAIPP(0x15,"interface.c",0x27b);
    } while (lVar2 <= param_1);
  }
  return;
}


