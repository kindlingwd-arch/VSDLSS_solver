/* driver: _ILPBYIIdFIBPXSPAIPP addr=009871d0 size=33 */
#include "vsdlss_ref.h"


void _ILPBYIIdFIBPXSPAIPP(void)

{
  undefined8 *puVar1;
  
  puVar1 = &DAT_00e3ab88;
  do {
    if ((FILE *)*puVar1 != (FILE *)0x0) {
      fclose((FILE *)*puVar1);
      *puVar1 = 0;
    }
    puVar1 = puVar1 + 5;
  } while (puVar1 != &DAT_00e3b560);
  return;
}


