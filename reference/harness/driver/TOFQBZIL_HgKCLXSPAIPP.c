/* driver: TOFQBZIL_HgKCLXSPAIPP addr=009a9780 size=3a5 */
#include "vsdlss_ref.h"


/* WARNING: Type propagation algorithm not settling */

void TOFQBZIL_HgKCLXSPAIPP(long *param_1)

{
  long lVar1;
  long lVar2;
  FILE *__s;
  size_t sVar3;
  long local_50 [3];
  long local_38;
  
  lVar1 = *param_1;
  lVar2 = param_1[1];
  __s = (FILE *)LMBKdFIBuXSPAIPP(1);
  local_50[1] = 0;
  local_50[0] = 2;
  local_50[2] = lVar1;
  local_38 = lVar2;
  sVar3 = fwrite(local_50,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2a7,"writeIVector:n",1,sVar3);
  }
  sVar3 = fwrite(local_50 + 1,8,local_50[0] + 1,__s);
  if (local_50[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2a7,"writeIVector:v",local_50[0] + 1U,sVar3);
  }
  local_50[0] = lVar2;
  sVar3 = fwrite(local_50,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2a9,"writeIVector:n",1,sVar3);
  }
  sVar3 = fwrite((void *)param_1[2],8,local_50[0] + 1,__s);
  if (local_50[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2a9,"writeIVector:v",local_50[0] + 1U,sVar3);
  }
  local_50[0] = lVar2;
  sVar3 = fwrite(local_50,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2aa,"writeIVector:n",1,sVar3);
  }
  sVar3 = fwrite((void *)param_1[3],8,local_50[0] + 1,__s);
  if (local_50[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2aa,"writeIVector:v",local_50[0] + 1U,sVar3);
  }
  local_50[0] = lVar2;
  sVar3 = fwrite(local_50,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2ab,"writeIVector:n",1,sVar3);
  }
  sVar3 = fwrite((void *)param_1[4],8,local_50[0] + 1,__s);
  if (local_50[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2ab,"writeIVector:v",local_50[0] + 1U,sVar3);
  }
  TOFQBjgtB_QLO(param_1[5],lVar2,__s);
  local_50[0] = lVar2;
  sVar3 = fwrite(local_50,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2ad,"writeIVector:n",1,sVar3);
  }
  sVar3 = fwrite((void *)param_1[6],8,local_50[0] + 1,__s);
  if (local_50[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2ad,"writeIVector:v",local_50[0] + 1U,sVar3);
  }
  local_50[0] = lVar1;
  sVar3 = fwrite(local_50,8,1,__s);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2ae,"writeIVector:n",1,sVar3);
  }
  sVar3 = fwrite((void *)param_1[7],8,local_50[0] + 1,__s);
  if (local_50[0] + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2ae,"writeIVector:v",local_50[0] + 1U,sVar3);
  }
  _yPFdaILPBXSPAIPP(1);
  return;
}


