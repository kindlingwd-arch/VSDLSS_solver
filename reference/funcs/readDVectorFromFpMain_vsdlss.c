/* VSDLSS function (decompiled by Ghidra) */
/* name: readDVectorFromFpMain_vsdlss  addr: 009d2530  size: 3d */
#include "vsdlss_ref.h"

/* signature: void readDVectorFromFpMain_vsdlss(FILE *param_1,int param_2,void *param_3); */

void readDVectorFromFpMain_vsdlss(FILE *param_1,int param_2,void *param_3)

{
  size_t sVar1;
  
  sVar1 = fread(param_3,8,(long)param_2,param_1);
  if ((int)sVar1 == param_2) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  fatalError_vsdlss(9999,"main.c",0x1df,"readDVectorFromFpMain failed: Reading %d but read %d\n",
                    param_2,sVar1 & 0xffffffff);
}


