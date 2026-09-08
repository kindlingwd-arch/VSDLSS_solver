/* driver: MOFKQbFCCrFJBPXSPAIPP addr=00961580 size=5d */
#include "vsdlss_ref.h"


void MOFKQbFCCrFJBPXSPAIPP(ulong *param_1,long *param_2,undefined8 param_3)

{
  long lVar1;
  ulong uVar2;
  
  uVar2 = (long)(*param_2 - *param_1) >> 0x3f;
  *param_1 = *param_2 - *param_1 ^ uVar2;
  lVar1 = param_2[1];
  *param_1 = *param_1 - uVar2;
  uVar2 = (long)(lVar1 - param_1[1]) >> 0x3f;
  param_1[1] = lVar1 - param_1[1] ^ uVar2;
  param_1[1] = param_1[1] - uVar2;
  _LKSBOQrLbB_FqB_LKAPXSPAIPP();
  printf("%s:uTime:%ld:sTime:%ld\n",param_3,*param_1,param_1[1]);
  return;
}


