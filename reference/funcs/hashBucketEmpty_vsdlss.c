/* VSDLSS function (decompiled by Ghidra) */
/* name: hashBucketEmpty_vsdlss  addr: 00a2f8b0  size: 19 */
#include "vsdlss_ref.h"

/* signature: ulong hashBucketEmpty_vsdlss(int param_1,int *param_2); */

ulong hashBucketEmpty_vsdlss(int param_1,int *param_2)

{
  return CONCAT71((int7)((ulong)((long)(param_1 + -1) / (long)*param_2) >> 8),
                  *(long *)(*(long *)(param_2 + 2) + (long)((param_1 + -1) % *param_2 + 1) * 8) == 0
                 ) & 0xffffffff;
}


