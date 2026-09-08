/* VSDLSS function (decompiled by Ghidra) */
/* name: writeChunkInfo_vsdlss  addr: 00a0e6a0  size: 539 */
#include "vsdlss_ref.h"

/* signature: void writeChunkInfo_vsdlss(char *param_1); */

/* WARNING: Type propagation algorithm not settling */

void writeChunkInfo_vsdlss(char *param_1)

{
  undefined8 *puVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  FILE *__s;
  size_t sVar6;
  int iVar7;
  long lVar8;
  int local_5c [3];
  int local_50;
  int local_4c;
  int local_48;
  
  uVar2 = *(undefined4 *)(param_1 + 4);
  iVar3 = *(int *)(param_1 + 8);
  iVar4 = *(int *)(param_1 + 0xc);
  iVar5 = *(int *)(param_1 + 0x10);
  __s = (FILE *)openFileW_vsdlss(6);
  if (*param_1 == '\0') {
    nrerror_vsdlss(0x2b,"readwriteutils.c",0x245);
  }
  local_5c[1] = 0;
  local_5c[0] = 4;
  local_5c[2] = uVar2;
  local_50 = iVar3;
  local_4c = iVar4;
  local_48 = iVar5;
  sVar6 = fwrite(local_5c,4,1,__s);
  if ((int)sVar6 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x250,"writeIVector:n",1,sVar6 & 0xffffffff);
  }
  sVar6 = fwrite(local_5c + 1,4,(long)(local_5c[0] + 1),__s);
  if (local_5c[0] + 1 != (int)sVar6) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x250,"writeIVector:v",local_5c[0] + 1,sVar6 & 0xffffffff
                  );
  }
  lVar8 = 8;
  iVar7 = 1;
  if (0 < iVar4) {
    do {
      iVar7 = iVar7 + 1;
      puVar1 = (undefined8 *)(*(long *)(param_1 + 0x18) + lVar8);
      lVar8 = lVar8 + 8;
      writeCVector(*puVar1,iVar5 + 1,__s);
    } while (iVar7 <= iVar4);
  }
  local_5c[0] = iVar3;
  sVar6 = fwrite(local_5c,4,1,__s);
  if ((int)sVar6 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x256,"writeIVector:n",1,sVar6 & 0xffffffff);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x20),4,(long)(local_5c[0] + 1),__s);
  if (local_5c[0] + 1 != (int)sVar6) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x256,"writeIVector:v",local_5c[0] + 1,sVar6 & 0xffffffff
                  );
  }
  local_5c[0] = iVar3;
  sVar6 = fwrite(local_5c,4,1,__s);
  if ((int)sVar6 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",599,"writeIVector:n",1,sVar6 & 0xffffffff);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x38),4,(long)(local_5c[0] + 1),__s);
  if (local_5c[0] + 1 != (int)sVar6) {
    nrerror_vsdlss(0x10,"readwriteutils.c",599,"writeIVector:v",local_5c[0] + 1,sVar6 & 0xffffffff);
  }
  local_5c[0] = iVar3;
  sVar6 = fwrite(local_5c,4,1,__s);
  if ((int)sVar6 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",600,"writeIVector:n",1,sVar6 & 0xffffffff);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x40),4,(long)(local_5c[0] + 1),__s);
  if (local_5c[0] + 1 != (int)sVar6) {
    nrerror_vsdlss(0x10,"readwriteutils.c",600,"writeIVector:v",local_5c[0] + 1,sVar6 & 0xffffffff);
  }
  local_5c[0] = iVar3;
  sVar6 = fwrite(local_5c,4,1,__s);
  if ((int)sVar6 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x259,"writeIVector:n",1,sVar6 & 0xffffffff);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x48),4,(long)(local_5c[0] + 1),__s);
  if (local_5c[0] + 1 != (int)sVar6) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x259,"writeIVector:v",local_5c[0] + 1,sVar6 & 0xffffffff
                  );
  }
  local_5c[0] = iVar3;
  sVar6 = fwrite(local_5c,4,1,__s);
  if ((int)sVar6 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x25a,"writeIVector:n",1,sVar6 & 0xffffffff);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x50),4,(long)(local_5c[0] + 1),__s);
  if (local_5c[0] + 1 != (int)sVar6) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x25a,"writeIVector:v",local_5c[0] + 1,sVar6 & 0xffffffff
                  );
  }
  local_5c[0] = iVar3;
  sVar6 = fwrite(local_5c,4,1,__s);
  if ((int)sVar6 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x25b,"writeIVector:n",1,sVar6 & 0xffffffff);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x58),4,(long)(local_5c[0] + 1),__s);
  if (local_5c[0] + 1 != (int)sVar6) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x25b,"writeIVector:v",local_5c[0] + 1,sVar6 & 0xffffffff
                  );
  }
  local_5c[0] = iVar3;
  sVar6 = fwrite(local_5c,4,1,__s);
  if ((int)sVar6 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x25c,"writeIVector:n",1,sVar6 & 0xffffffff);
  }
  sVar6 = fwrite(*(void **)(param_1 + 0x60),4,(long)(local_5c[0] + 1),__s);
  if (local_5c[0] + 1 != (int)sVar6) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x25c,"writeIVector:v",local_5c[0] + 1,sVar6 & 0xffffffff
                  );
  }
  writeLIVector(*(undefined8 *)(param_1 + 0x68),iVar3,__s);
  casiFClose_vsdlss(6);
  return;
}


