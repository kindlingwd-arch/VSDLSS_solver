/* VSDLSS function (decompiled by Ghidra) */
/* name: readDVectorFNum_vsdlss  addr: 009d9520  size: 55 */
#include "vsdlss_ref.h"

/* signature: void readDVectorFNum_vsdlss(undefined4 param_1,int param_2,long param_3); */

void readDVectorFNum_vsdlss(undefined4 param_1,int param_2,long param_3)

{
  FILE *__stream;
  size_t sVar1;
  
  __stream = (FILE *)openFileR_vsdlss();
  sVar1 = fread((void *)(param_3 + 8),8,(long)param_2,__stream);
  if ((int)sVar1 != param_2) {
    nrerror_vsdlss(0x10,"utils.c",0x6da,"readDVectorFNum:v",param_2,sVar1 & 0xffffffff);
  }
  casiFClose_vsdlss(param_1);
  return;
}


