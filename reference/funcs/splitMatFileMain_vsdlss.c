/* VSDLSS function (decompiled by Ghidra) */
/* name: splitMatFileMain_vsdlss  addr: 009d25b0  size: 19d */
#include "vsdlss_ref.h"

/* signature: void splitMatFileMain_vsdlss(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,undefined8 param_5); */

void splitMatFileMain_vsdlss
               (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               undefined8 param_5)

{
  FILE *__stream;
  size_t sVar1;
  void *pvVar2;
  int local_40;
  int local_3c [3];
  
  __stream = (FILE *)openFileMainR_vsdlss();
  sVar1 = fread(&local_40,4,1,__stream);
  if ((int)sVar1 != 1) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x202,"splitMatFileMain failed: Reading %d but read %d\n",1,
                      sVar1 & 0xffffffff);
  }
  sVar1 = fread(local_3c,4,1,__stream);
  if ((int)sVar1 == 1) {
    pvVar2 = tc_malloc((long)local_40 << 3);
    readDVectorFromFpMain_vsdlss(__stream,local_40,pvVar2);
    writeDVectorToFnMain_vsdlss(param_2,local_40,pvVar2);
    tc_cfree(pvVar2);
    pvVar2 = tc_malloc((long)local_3c[0] << 3);
    readDVectorFromFpMain_vsdlss(__stream,local_3c[0],pvVar2);
    writeDVectorToFnMain_vsdlss(param_5,local_3c[0],pvVar2);
    tc_cfree(pvVar2);
    pvVar2 = tc_malloc((long)local_3c[0] << 2);
    readIVectorFromFpMain_vsdlss(__stream,local_3c[0],pvVar2);
    writeIVectorToFnMain_vsdlss(param_3,local_3c[0],pvVar2);
    tc_cfree(pvVar2);
    pvVar2 = tc_malloc((long)local_3c[0] << 2);
    readIVectorFromFpMain_vsdlss(__stream,local_3c[0],pvVar2);
    writeIVectorToFnMain_vsdlss(param_4,local_3c[0],pvVar2);
    tc_cfree(pvVar2);
    fclose(__stream);
    return;
  }
                    /* WARNING: Subroutine does not return */
  fatalError_vsdlss(9999,"main.c",0x20a,"splitMatFileMain failed: Reading %d but read %d\n",1,
                    sVar1 & 0xffffffff);
}


