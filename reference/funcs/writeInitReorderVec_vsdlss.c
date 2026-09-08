/* VSDLSS function (decompiled by Ghidra) */
/* name: writeInitReorderVec_vsdlss  addr: 00a0e5a0  size: b8 */
#include "vsdlss_ref.h"

/* signature: void writeInitReorderVec_vsdlss(void *param_1,int param_2); */

void writeInitReorderVec_vsdlss(void *param_1,int param_2)

{
  FILE *__s;
  size_t sVar1;
  int local_1c;
  
  __s = (FILE *)openFileW_vsdlss(7);
  local_1c = param_2;
  sVar1 = fwrite(&local_1c,4,1,__s);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x217,"writeIVector:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fwrite(param_1,4,(long)(local_1c + 1),__s);
  if (local_1c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x217,"writeIVector:v",local_1c + 1,sVar1 & 0xffffffff);
  }
  casiFClose_vsdlss(7);
  return;
}


