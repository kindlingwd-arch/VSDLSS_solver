/* VSDLSS function (decompiled by Ghidra) */
/* name: deletePqi_vsdlss  addr: 00a0cb80  size: 67 */
#include "vsdlss_ref.h"

/* signature: undefined8 deletePqi_vsdlss(int *param_1); */

undefined8 deletePqi_vsdlss(int *param_1)

{
  if (param_1 != (int *)0x0) {
    if (*param_1 == -1) {
      if (*(long *)(param_1 + 4) != 0) {
        casiFree_vsdlss();
      }
      param_1[4] = 0;
      param_1[5] = 0;
      if (*(long *)(param_1 + 6) != 0) {
        casiFree_vsdlss();
      }
      param_1[6] = 0;
      param_1[7] = 0;
      if (*(long *)(param_1 + 8) != 0) {
        casiFree_vsdlss();
      }
      param_1[8] = 0;
      param_1[9] = 0;
      casiFree_vsdlss(param_1);
      return 0;
    }
    deleteTag_vsdlss();
  }
  return 0;
}


