/* VSDLSS function (decompiled by Ghidra) */
/* name: writeIVectorFNum_vsdlss  addr: 009d96a0  size: 55 */
#include "vsdlss_ref.h"

/* signature: void writeIVectorFNum_vsdlss(undefined4 param_1,int param_2,long param_3); */

void writeIVectorFNum_vsdlss(undefined4 param_1,int param_2,long param_3)

{
  FILE *__s;
  size_t sVar1;
  
  __s = (FILE *)openFileW_vsdlss();
  sVar1 = fwrite((void *)(param_3 + 4),4,(long)param_2,__s);
  if ((int)sVar1 != param_2) {
    nrerror_vsdlss(0x10,"utils.c",0x71b,"writeIVectorFNum:v",param_2,sVar1 & 0xffffffff);
  }
  casiFClose_vsdlss(param_1);
  return;
}


