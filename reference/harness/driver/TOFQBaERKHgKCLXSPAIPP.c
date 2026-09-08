/* driver: TOFQBaERKHgKCLXSPAIPP addr=009a8d70 size=534 */
#include "vsdlss_ref.h"


/* WARNING: Type propagation algorithm not settling */

void TOFQBaERKHgKCLXSPAIPP(char *param_1)

{
  long lVar1;
  undefined8 uVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  FILE *__s;
  size_t sVar6;
  long lVar7;
  long local_70 [3];
  long local_58;
  long local_50;
  long local_48;
  
  uVar2 = *(undefined8 *)(param_1 + 8);
  lVar3 = *(long *)(param_1 + 0x10);
  lVar4 = *(long *)(param_1 + 0x18);
  lVar5 = *(long *)(param_1 + 0x20);
  __s = (FILE *)LMBKdFIBuXSPAIPP(6);
  if (*param_1 == '\0') {
    KOBOOLOXSPAIPP(0x2b,"readwriteutils.c",0x245);
  }
  local_70[1] = 0;
  local_70[0] = 4;
  local_70[2] = uVar2;
  local_58 = lVar3;
  local_50 = lVar4;
  local_48 = lVar5;
  sVar6 = fwrite(local_70,8,1,__s);
  if (sVar6 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x250,"writeIVector:n",1,sVar6);
  }
  sVar6 = fwrite(local_70 + 1,8,local_70[0] + 1,__s);
  if (local_70[0] + 1U != sVar6) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x250,"writeIVector:v",local_70[0] + 1U,sVar6);
  }
  lVar7 = 1;
  if (0 < lVar4) {
    do {
      lVar1 = lVar7 * 8;
      lVar7 = lVar7 + 1;
      TOFQBatB_QLO(*(undefined8 *)(*(long *)(param_1 + 0x28) + lVar1),lVar5 + 1,__s);
    } while (lVar7 <= lVar4);
  }
  local_70[0] = lVar3;
  sVar6 = fwrite(local_70,8,1,__s);
  if (sVar6 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x256,"writeIVector:n",1,sVar6);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x30),8,local_70[0] + 1,__s);
  if (local_70[0] + 1U != sVar6) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x256,"writeIVector:v",local_70[0] + 1U,sVar6);
  }
  local_70[0] = lVar3;
  sVar6 = fwrite(local_70,8,1,__s);
  if (sVar6 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",599,"writeIVector:n",1,sVar6);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x48),8,local_70[0] + 1,__s);
  if (local_70[0] + 1U != sVar6) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",599,"writeIVector:v",local_70[0] + 1U,sVar6);
  }
  local_70[0] = lVar3;
  sVar6 = fwrite(local_70,8,1,__s);
  if (sVar6 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",600,"writeIVector:n",1,sVar6);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x50),8,local_70[0] + 1,__s);
  if (local_70[0] + 1U != sVar6) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",600,"writeIVector:v",local_70[0] + 1U,sVar6);
  }
  local_70[0] = lVar3;
  sVar6 = fwrite(local_70,8,1,__s);
  if (sVar6 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x259,"writeIVector:n",1,sVar6);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x58),8,local_70[0] + 1,__s);
  if (local_70[0] + 1U != sVar6) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x259,"writeIVector:v",local_70[0] + 1U,sVar6);
  }
  local_70[0] = lVar3;
  sVar6 = fwrite(local_70,8,1,__s);
  if (sVar6 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x25a,"writeIVector:n",1,sVar6);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x60),8,local_70[0] + 1,__s);
  if (local_70[0] + 1U != sVar6) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x25a,"writeIVector:v",local_70[0] + 1U,sVar6);
  }
  local_70[0] = lVar3;
  sVar6 = fwrite(local_70,8,1,__s);
  if (sVar6 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x25b,"writeIVector:n",1,sVar6);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x68),8,local_70[0] + 1,__s);
  if (local_70[0] + 1U != sVar6) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x25b,"writeIVector:v",local_70[0] + 1U,sVar6);
  }
  local_70[0] = lVar3;
  sVar6 = fwrite(local_70,8,1,__s);
  if (sVar6 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x25c,"writeIVector:n",1,sVar6);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x70),8,local_70[0] + 1,__s);
  if (local_70[0] + 1U != sVar6) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x25c,"writeIVector:v",local_70[0] + 1U,sVar6);
  }
  TOFQBjgtB_QLO(*(undefined8 *)(param_1 + 0x78),lVar3,__s);
  _yPFdaILPBXSPAIPP(6);
  return;
}


