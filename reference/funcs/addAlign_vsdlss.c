/* VSDLSS function (decompiled by Ghidra) */
/* name: addAlign_vsdlss  addr: 00a191e0  size: 14 */
#include "vsdlss_ref.h"

/* signature: long addAlign_vsdlss(ulong param_1,int param_2); */

long addAlign_vsdlss(ulong param_1,int param_2)

{
  return (long)(param_2 - (int)(param_1 % (ulong)(long)param_2)) + param_1;
}


