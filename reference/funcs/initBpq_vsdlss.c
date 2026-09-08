/* VSDLSS function (decompiled by Ghidra) */
/* name: initBpq_vsdlss  addr: 00a1ecd0  size: 62 */
#include "vsdlss_ref.h"

/* signature: void initBpq_vsdlss(int param_1,int *param_2); */

void initBpq_vsdlss(int param_1,int *param_2)

{
  long lVar1;
  
  lVar1 = (long)param_1;
  *param_2 = param_1;
  param_2[1] = param_1;
  param_2[2] = 0;
  param_2[3] = 0;
  param_2[4] = 0x7ffffff5;
  param_2[5] = 0x7ffffff5;
  initIVector_vsdlss(*(undefined8 *)(param_2 + 6),lVar1,0x7ffffff5);
  initIVector_vsdlss(*(undefined8 *)(param_2 + 8),lVar1,param_2[4]);
  initIVector_vsdlss(*(undefined8 *)(param_2 + 10),lVar1,param_2[4]);
  return;
}


