/* VSDLSS function (decompiled by Ghidra) */
/* name: writeDeg3Pf_vsdlss  addr: 009e3ce0  size: 4a4 */
#include "vsdlss_ref.h"

/* signature: void writeDeg3Pf_vsdlss(undefined4 *param_1); */

/* WARNING: Type propagation algorithm not settling */

void writeDeg3Pf_vsdlss(undefined4 *param_1)

{
  undefined4 uVar1;
  int iVar2;
  FILE *__s;
  size_t sVar3;
  int local_7c [3];
  int local_70;
  
  uVar1 = *param_1;
  iVar2 = param_1[1];
  __s = (FILE *)openFileW_vsdlss(4);
  local_7c[1] = 0;
  local_7c[0] = 2;
  local_7c[2] = uVar1;
  local_70 = iVar2;
  sVar3 = fwrite(local_7c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x195,"writeIVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(local_7c + 1,4,(long)(local_7c[0] + 1),__s);
  if (local_7c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x195,"writeIVector:v",local_7c[0] + 1,sVar3 & 0xffffffff)
    ;
  }
  local_7c[0] = iVar2;
  sVar3 = fwrite(local_7c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x197,"writeFVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(*(void **)(param_1 + 2),8,(long)(local_7c[0] + 1),__s);
  if (local_7c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x197,"writeFVector:v",local_7c[0] + 1,sVar3 & 0xffffffff)
    ;
  }
  local_7c[0] = iVar2;
  sVar3 = fwrite(local_7c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x198,"writeIVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(*(void **)(param_1 + 4),4,(long)(local_7c[0] + 1),__s);
  if (local_7c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x198,"writeIVector:v",local_7c[0] + 1,sVar3 & 0xffffffff)
    ;
  }
  local_7c[0] = iVar2;
  sVar3 = fwrite(local_7c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x199,"writeFVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(*(void **)(param_1 + 6),8,(long)(local_7c[0] + 1),__s);
  if (local_7c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x199,"writeFVector:v",local_7c[0] + 1,sVar3 & 0xffffffff)
    ;
  }
  local_7c[0] = iVar2;
  sVar3 = fwrite(local_7c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x19a,"writeIVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(*(void **)(param_1 + 8),4,(long)(local_7c[0] + 1),__s);
  if (local_7c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x19a,"writeIVector:v",local_7c[0] + 1,sVar3 & 0xffffffff)
    ;
  }
  local_7c[0] = iVar2;
  sVar3 = fwrite(local_7c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x19b,"writeFVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(*(void **)(param_1 + 10),8,(long)(local_7c[0] + 1),__s);
  if (local_7c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x19b,"writeFVector:v",local_7c[0] + 1,sVar3 & 0xffffffff)
    ;
  }
  local_7c[0] = iVar2;
  sVar3 = fwrite(local_7c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x19c,"writeIVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(*(void **)(param_1 + 0xc),4,(long)(local_7c[0] + 1),__s);
  if (local_7c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x19c,"writeIVector:v",local_7c[0] + 1,sVar3 & 0xffffffff)
    ;
  }
  local_7c[0] = iVar2;
  sVar3 = fwrite(local_7c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x19d,"writeFVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(*(void **)(param_1 + 0xe),8,(long)(local_7c[0] + 1),__s);
  if (local_7c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x19d,"writeFVector:v",local_7c[0] + 1,sVar3 & 0xffffffff)
    ;
  }
  casiFClose_vsdlss(4);
  return;
}


