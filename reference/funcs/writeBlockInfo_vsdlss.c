/* VSDLSS function (decompiled by Ghidra) */
/* name: writeBlockInfo_vsdlss  addr: 00a0f0a0  size: 39d */
#include "vsdlss_ref.h"

/* signature: void writeBlockInfo_vsdlss(int *param_1); */

/* WARNING: Type propagation algorithm not settling */

void writeBlockInfo_vsdlss(int *param_1)

{
  int iVar1;
  int iVar2;
  FILE *__s;
  size_t sVar3;
  int local_3c [3];
  int local_30;
  
  iVar1 = *param_1;
  iVar2 = param_1[1];
  __s = (FILE *)openFileW_vsdlss(1);
  local_3c[1] = 0;
  local_3c[0] = 2;
  local_3c[2] = iVar1;
  local_30 = iVar2;
  sVar3 = fwrite(local_3c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2a7,"writeIVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(local_3c + 1,4,(long)(local_3c[0] + 1),__s);
  if (local_3c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2a7,"writeIVector:v",local_3c[0] + 1,sVar3 & 0xffffffff
                  );
  }
  local_3c[0] = iVar2;
  sVar3 = fwrite(local_3c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2a9,"writeIVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(*(void **)(param_1 + 2),4,(long)(local_3c[0] + 1),__s);
  if (local_3c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2a9,"writeIVector:v",local_3c[0] + 1,sVar3 & 0xffffffff
                  );
  }
  local_3c[0] = iVar2;
  sVar3 = fwrite(local_3c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2aa,"writeIVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(*(void **)(param_1 + 4),4,(long)(local_3c[0] + 1),__s);
  if (local_3c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2aa,"writeIVector:v",local_3c[0] + 1,sVar3 & 0xffffffff
                  );
  }
  local_3c[0] = iVar2;
  sVar3 = fwrite(local_3c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2ab,"writeIVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(*(void **)(param_1 + 6),4,(long)(local_3c[0] + 1),__s);
  if (local_3c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2ab,"writeIVector:v",local_3c[0] + 1,sVar3 & 0xffffffff
                  );
  }
  writeLIVector(*(undefined8 *)(param_1 + 8),iVar2,__s);
  local_3c[0] = iVar2;
  sVar3 = fwrite(local_3c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2ad,"writeIVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(*(void **)(param_1 + 10),4,(long)(local_3c[0] + 1),__s);
  if (local_3c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2ad,"writeIVector:v",local_3c[0] + 1,sVar3 & 0xffffffff
                  );
  }
  local_3c[0] = iVar1;
  sVar3 = fwrite(local_3c,4,1,__s);
  if ((int)sVar3 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2ae,"writeIVector:n",1,sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(*(void **)(param_1 + 0xc),4,(long)(local_3c[0] + 1),__s);
  if (local_3c[0] + 1 != (int)sVar3) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2ae,"writeIVector:v",local_3c[0] + 1,sVar3 & 0xffffffff
                  );
  }
  casiFClose_vsdlss(1);
  return;
}


