/* VSDLSS function (decompiled by Ghidra) */
/* name: getAChunkFn_vsdlss  addr: 009e26c0  size: 68 */
#include "vsdlss_ref.h"

/* signature: void getAChunkFn_vsdlss(int param_1,char *param_2); */

void getAChunkFn_vsdlss(int param_1,char *param_2)

{
  if (param_1 - 1U < 9) {
    sprintf(param_2,"%s.casi00%d.sda",baseJobName_vsdlss);
    return;
  }
  if (0x59 < param_1 - 10U) {
    if (899 < param_1 - 100U) {
      nrerror_vsdlss(0x11,"chunkinfoutils.c",0xc2);
      return;
    }
    sprintf(param_2,"%s.casi%d.sda",baseJobName_vsdlss);
    return;
  }
  sprintf(param_2,"%s.casi0%d.sda",baseJobName_vsdlss);
  return;
}


