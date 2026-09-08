/* VSDLSS function (decompiled by Ghidra) */
/* name: writeDVectorToFnMain_vsdlss  addr: 009d2470  size: 55 */
#include "vsdlss_ref.h"

/* signature: void writeDVectorToFnMain_vsdlss(undefined8 param_1,int param_2,void *param_3); */

void writeDVectorToFnMain_vsdlss(undefined8 param_1,int param_2,void *param_3)

{
  FILE *__s;
  size_t sVar1;
  
  __s = (FILE *)openFileMainW_vsdlss();
  sVar1 = fwrite(param_3,8,(long)param_2,__s);
  if ((int)sVar1 == param_2) {
    fclose(__s);
    return;
  }
                    /* WARNING: Subroutine does not return */
  fatalError_vsdlss(9999,"main.c",0x1bd,"writeDVectorToFnMain failed: Writing %d but written %d\n",
                    param_2,sVar1 & 0xffffffff);
}


