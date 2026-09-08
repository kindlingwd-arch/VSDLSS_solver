/* VSDLSS function (decompiled by Ghidra) */
/* name: getFactorChunkFn_vsdlss  addr: 009e25b0  size: 68 */
#include "vsdlss_ref.h"

/* signature: void getFactorChunkFn_vsdlss(int param_1,char *param_2); */

void getFactorChunkFn_vsdlss(int param_1,char *param_2)

{
  if (param_1 - 1U < 9) {
    sprintf(param_2,"%s.casi00%d.sdc",baseJobName_vsdlss);
    return;
  }
  if (0x59 < param_1 - 10U) {
    if (899 < param_1 - 100U) {
      nrerror_vsdlss(0x11,"chunkinfoutils.c",0x7b);
      return;
    }
    sprintf(param_2,"%s.casi%d.sdc",baseJobName_vsdlss);
    return;
  }
  sprintf(param_2,"%s.casi0%d.sdc",baseJobName_vsdlss);
  return;
}


