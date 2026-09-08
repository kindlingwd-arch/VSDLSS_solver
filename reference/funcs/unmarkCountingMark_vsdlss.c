/* VSDLSS function (decompiled by Ghidra) */
/* name: unmarkCountingMark_vsdlss  addr: 00a1f180  size: 21 */
#include "vsdlss_ref.h"

/* signature: void unmarkCountingMark_vsdlss(int *param_1); */

void unmarkCountingMark_vsdlss(int *param_1)

{
  if (*param_1 < param_1[1]) {
    *param_1 = *param_1 + 1;
    return;
  }
  *param_1 = 1;
  initIVector_vsdlss(*(undefined8 *)(param_1 + 2),(long)param_1[1],0);
  return;
}


