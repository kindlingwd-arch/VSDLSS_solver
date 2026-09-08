/* VSDLSS function (decompiled by Ghidra) */
/* name: scaleTime_vsdlss  addr: 009d7990  size: 25 */
#include "vsdlss_ref.h"

/* signature: void scaleTime_vsdlss(double param_1,long *param_2,long *param_3); */

void scaleTime_vsdlss(double param_1,long *param_2,long *param_3)

{
  long lVar1;
  
  lVar1 = param_2[1];
  *param_3 = (long)((double)*param_2 * param_1);
  param_3[1] = (long)((double)lVar1 * param_1);
  return;
}


