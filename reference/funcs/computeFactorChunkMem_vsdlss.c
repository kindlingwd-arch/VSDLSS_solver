/* VSDLSS function (decompiled by Ghidra) */
/* name: computeFactorChunkMem_vsdlss  addr: 00a227c0  size: 4e */
#include "vsdlss_ref.h"

/* signature: double computeFactorChunkMem_vsdlss(double param_1,double param_2,double param_3,int param_4); */

double computeFactorChunkMem_vsdlss(double param_1,double param_2,double param_3,int param_4)

{
  double dVar1;
  
  dVar1 = ((param_1 - param_2) - param_3) * DAT_00abc218;
  if ((double)param_4 * DAT_00ab6798 <= dVar1) {
    return dVar1;
  }
  nrerror_vsdlss(0x28,"memutils.c",0xb6);
  return dVar1;
}


