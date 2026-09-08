/* driver: TOFQBbBD3nCXSPAIPP addr=00976020 size=4b4 */
#include "vsdlss_ref.h"


/* WARNING: Type propagation algorithm not settling */

void TOFQBbBD3nCXSPAIPP(undefined8 *param_1)

{
  undefined8 uVar1;
  long lVar2;
  FILE *__s;
  size_t sVar3;
  long local_c0 [3];
  long local_a8;
  
  uVar1 = *param_1;
  lVar2 = param_1[1];
  __s = (FILE *)LMBKdFIBuXSPAIPP(4);
  local_c0[1] = 0;
  local_c0[0] = 2;
  local_c0[2] = uVar1;
  local_a8 = lVar2;
  sVar3 = fwrite(local_c0,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x195,"writeIVector:n",1,sVar3);
  }
  sVar3 = fwrite(local_c0 + 1,8,local_c0[0] + 1,__s);
  if (local_c0[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x195,"writeIVector:v",local_c0[0] + 1U,sVar3);
  }
  local_c0[0] = lVar2;
  sVar3 = fwrite(local_c0,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x197,"writeFVector:n",1,sVar3);
  }
  sVar3 = fwrite((void *)param_1[2],8,local_c0[0] + 1,__s);
  if (local_c0[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x197,"writeFVector:v",local_c0[0] + 1U,sVar3);
  }
  local_c0[0] = lVar2;
  sVar3 = fwrite(local_c0,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x198,"writeIVector:n",1,sVar3);
  }
  sVar3 = fwrite((void *)param_1[3],8,local_c0[0] + 1,__s);
  if (local_c0[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x198,"writeIVector:v",local_c0[0] + 1U,sVar3);
  }
  local_c0[0] = lVar2;
  sVar3 = fwrite(local_c0,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x199,"writeFVector:n",1,sVar3);
  }
  sVar3 = fwrite((void *)param_1[4],8,local_c0[0] + 1,__s);
  if (local_c0[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x199,"writeFVector:v",local_c0[0] + 1U,sVar3);
  }
  local_c0[0] = lVar2;
  sVar3 = fwrite(local_c0,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x19a,"writeIVector:n",1,sVar3);
  }
  sVar3 = fwrite((void *)param_1[5],8,local_c0[0] + 1,__s);
  if (local_c0[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x19a,"writeIVector:v",local_c0[0] + 1U,sVar3);
  }
  local_c0[0] = lVar2;
  sVar3 = fwrite(local_c0,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x19b,"writeFVector:n",1,sVar3);
  }
  sVar3 = fwrite((void *)param_1[6],8,local_c0[0] + 1,__s);
  if (local_c0[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x19b,"writeFVector:v",local_c0[0] + 1U,sVar3);
  }
  local_c0[0] = lVar2;
  sVar3 = fwrite(local_c0,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x19c,"writeIVector:n",1,sVar3);
  }
  sVar3 = fwrite((void *)param_1[7],8,local_c0[0] + 1,__s);
  if (local_c0[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x19c,"writeIVector:v",local_c0[0] + 1U,sVar3);
  }
  local_c0[0] = lVar2;
  sVar3 = fwrite(local_c0,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x19d,"writeFVector:n",1,sVar3);
  }
  sVar3 = fwrite((void *)param_1[8],8,local_c0[0] + 1,__s);
  if (local_c0[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x19d,"writeFVector:v",local_c0[0] + 1U,sVar3);
  }
  _yPFdaILPBXSPAIPP(4);
  return;
}


