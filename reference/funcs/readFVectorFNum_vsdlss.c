/* VSDLSS function (decompiled by Ghidra) */
/* name: readFVectorFNum_vsdlss  addr: 009d94c0  size: 55 */
#include "vsdlss_ref.h"

/* signature: void readFVectorFNum_vsdlss(undefined4 param_1,int param_2,long param_3); */

void readFVectorFNum_vsdlss(undefined4 param_1,int param_2,long param_3)

{
  FILE *__stream;
  size_t sVar1;
  
  __stream = (FILE *)openFileR_vsdlss();
  sVar1 = fread((void *)(param_3 + 8),8,(long)param_2,__stream);
  if ((int)sVar1 != param_2) {
    nrerror_vsdlss(0x10,"utils.c",0x6c4,"readFVectorFNum:v",param_2,sVar1 & 0xffffffff);
  }
  casiFClose_vsdlss(param_1);
  return;
}


