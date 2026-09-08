/* VSDLSS function (decompiled by Ghidra) */
/* name: readMatrixFromFile_vsdlss  addr: 009f5b40  size: 60 */
#include "vsdlss_ref.h"

/* signature: void readMatrixFromFile_vsdlss(undefined4 *param_1); */

void readMatrixFromFile_vsdlss(undefined4 *param_1)

{
  undefined4 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  
  uVar1 = *param_1;
  uVar2 = *(undefined8 *)(param_1 + 6);
  uVar3 = *(undefined8 *)(param_1 + 10);
  uVar4 = *(undefined8 *)(param_1 + 0xc);
  readFVectorFNum_vsdlss(0x1a,param_1[1],*(undefined8 *)(param_1 + 0xe));
  readIVectorFNum_vsdlss(0x1b,uVar1,uVar2);
  readIVectorFNum_vsdlss(0x1c,uVar1,uVar3);
  readFVectorFNum_vsdlss(0x1d,uVar1,uVar4);
  return;
}


