/* driver: TOFQBgKPXSPAIPP addr=00989950 size=178 */
#include "vsdlss_ref.h"


void TOFQBgKPXSPAIPP(long *param_1,FILE *param_2,void *param_3)

{
  long lVar1;
  size_t sVar2;
  long lVar3;
  long local_c0;
  undefined1 local_b8 [8];
  long local_b0;
  
  lVar1 = *param_1;
  local_c0 = 1;
  local_b0 = lVar1;
  sVar2 = fwrite(&local_c0,8,1,param_2);
  if (sVar2 != 1) {
    KOBOOLOXSPAIPP(0x10,"indistnodesetsutils.c",0x11d,"writeIVector:n",1,sVar2);
  }
  sVar2 = fwrite(local_b8,8,local_c0 + 1,param_2);
  if (local_c0 + 1U != sVar2) {
    KOBOOLOXSPAIPP(0x10,"indistnodesetsutils.c",0x11d,"writeIVector:v",local_c0 + 1U,sVar2);
  }
  lVar3 = 1;
  if (0 < lVar1) {
    do {
      local_c0 = _LMVacADBIFPQrLYOOyVXSPAIPP(*(undefined8 *)(param_1[1] + lVar3 * 8),param_3);
      sVar2 = fwrite(&local_c0,8,1,param_2);
      if (sVar2 != 1) {
        KOBOOLOXSPAIPP(0x10,"indistnodesetsutils.c",0x121,"writeIVector:n",1,sVar2);
      }
      sVar2 = fwrite(param_3,8,local_c0 + 1,param_2);
      if (local_c0 + 1U != sVar2) {
        KOBOOLOXSPAIPP(0x10,"indistnodesetsutils.c",0x121,"writeIVector:v",local_c0 + 1U,sVar2);
      }
      lVar3 = lVar3 + 1;
    } while (lVar3 <= lVar1);
  }
  return;
}


