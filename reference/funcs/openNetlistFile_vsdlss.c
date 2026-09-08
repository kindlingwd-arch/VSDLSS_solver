/* VSDLSS function (decompiled by Ghidra) */
/* name: openNetlistFile_vsdlss  addr: 009d22b0  size: 43 */
#include "vsdlss_ref.h"

/* signature: void openNetlistFile_vsdlss(void); */

void openNetlistFile_vsdlss(void)

{
  DAT_00e96448 = fopen((char *)&DAT_00e96460,"r");
  if (DAT_00e96448 != (FILE *)0x0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  fatalError_vsdlss(9999,"main.c",0x155,
                    "Netlist File %s cannot be opened.\nPlease create the file/permission read.\n",
                    &DAT_00e96460);
}


