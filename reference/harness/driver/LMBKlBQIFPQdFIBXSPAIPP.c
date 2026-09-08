/* driver: LMBKlBQIFPQdFIBXSPAIPP addr=0095aa90 size=43 */
#include "vsdlss_ref.h"


void LMBKlBQIFPQdFIBXSPAIPP(void)

{
  DAT_00e56348 = fopen((char *)&DAT_00e56360,"r");
  if (DAT_00e56348 != (FILE *)0x0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  CyQyIcOOLOXSPAIPP(9999,"main.c",0x155,
                    "Netlist File %s cannot be opened.\nPlease create the file/permission read.\n",
                    &DAT_00e56360);
}


