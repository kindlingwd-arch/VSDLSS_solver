/* VSDLSS function (decompiled by Ghidra) */
/* name: deletePfResAndRhsVecs_vsdlss  addr: 00a10290  size: 68 */
#include "vsdlss_ref.h"

/* signature: void deletePfResAndRhsVecs_vsdlss(void); */

void deletePfResAndRhsVecs_vsdlss(void)

{
  if (DAT_00ec5e98 != 0) {
    casiFree_vsdlss();
  }
  DAT_00ec5e98 = 0;
  if (DAT_00ec5e90 != 0) {
    casiFree_vsdlss();
  }
  DAT_00ec5e90 = 0;
  if (DAT_00ec5e88 != 0) {
    casiFree_vsdlss();
  }
  DAT_00ec5e88 = 0;
  DAT_00ec5e80 = 0;
  return;
}


