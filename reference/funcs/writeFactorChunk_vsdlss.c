/* VSDLSS function (decompiled by Ghidra) */
/* name: writeFactorChunk_vsdlss  addr: 009f26d0  size: 3f8 */
#include "vsdlss_ref.h"

/* signature: void writeFactorChunk_vsdlss(long param_1,long param_2,long param_3); */

/* WARNING: Type propagation algorithm not settling */

void writeFactorChunk_vsdlss(long param_1,long param_2,long param_3)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  FILE *__s;
  size_t sVar10;
  long lVar11;
  int iVar12;
  int iVar13;
  long lVar14;
  long lVar15;
  long local_f0;
  int local_8c [23];
  
  iVar2 = *(int *)(param_1 + 4);
  iVar3 = *(int *)(*(long *)(param_2 + 0x40) + (long)iVar2 * 4);
  __s = (FILE *)openFactorChunkFileW_vsdlss(param_2,iVar2);
  iVar4 = *(int *)(param_1 + 0x14);
  local_8c[1] = 0;
  local_8c[0] = 1;
  lVar6 = *(long *)(param_1 + 0x28);
  lVar7 = *(long *)(param_1 + 0x30);
  lVar8 = *(long *)(param_1 + 0x38);
  local_8c[2] = *(int *)(param_1 + 4);
  sVar10 = fwrite(local_8c,4,1,__s);
  if ((int)sVar10 != 1) {
    nrerror_vsdlss(0x10,"factorchunkutils.c",0xc9,"writeIVector:n",1,sVar10 & 0xffffffff);
  }
  sVar10 = fwrite(local_8c + 1,4,(long)(local_8c[0] + 1),__s);
  if (local_8c[0] + 1 != (int)sVar10) {
    nrerror_vsdlss(0x10,"factorchunkutils.c",0xc9,"writeIVector:v",local_8c[0] + 1,
                   sVar10 & 0xffffffff);
  }
  iVar5 = *(int *)(param_1 + 0x18);
  if (0 < iVar4) {
    lVar15 = 0;
    iVar12 = 1;
    local_f0 = (long)iVar5 << 3;
    do {
      iVar13 = iVar5 + (int)lVar15;
      lVar11 = ftell(__s);
      if (iVar13 == iVar3) {
        *(long *)(*(long *)(param_2 + 0x68) + (long)iVar2 * 8) = lVar11;
      }
      *(long *)(*(long *)(param_3 + 0x20) + local_f0) = lVar11;
      local_8c[0] = 1;
      local_8c[2] = iVar13;
      sVar10 = fwrite(local_8c,4,1,__s);
      if ((int)sVar10 != 1) {
        nrerror_vsdlss(0x10,"factorchunkutils.c",0xdc,"writeIVector:n",1,sVar10 & 0xffffffff);
      }
      sVar10 = fwrite(local_8c + 1,4,(long)(local_8c[0] + 1),__s);
      if (local_8c[0] + 1 != (int)sVar10) {
        nrerror_vsdlss(0x10,"factorchunkutils.c",0xdc,"writeIVector:v",local_8c[0] + 1,
                       sVar10 & 0xffffffff);
      }
      lVar11 = (long)iVar12;
      piVar1 = (int *)(lVar6 + lVar11 * 4);
      local_8c[0] = *piVar1;
      sVar10 = fwrite(local_8c,4,1,__s);
      if ((int)sVar10 != 1) {
        nrerror_vsdlss(0x10,"factorchunkutils.c",0xdf,"writeIVector:n",1,sVar10 & 0xffffffff);
      }
      sVar10 = fwrite(*(void **)(lVar7 + lVar11 * 8),4,(long)(local_8c[0] + 1),__s);
      if (local_8c[0] + 1 != (int)sVar10) {
        nrerror_vsdlss(0x10,"factorchunkutils.c",0xdf,"writeIVector:v",local_8c[0] + 1,
                       sVar10 & 0xffffffff);
      }
      lVar15 = lVar15 + 1;
      if (0 < *(int *)(*(long *)(param_1 + 0x20) + lVar15 * 4)) {
        lVar14 = 0;
        iVar13 = iVar12 + 1;
        do {
          local_8c[0] = piVar1[lVar14];
          sVar10 = fwrite(local_8c,4,1,__s);
          if ((int)sVar10 != 1) {
            nrerror_vsdlss(0x10,"factorchunkutils.c",0xe2,"writeFVector:n",1,sVar10 & 0xffffffff);
          }
          sVar10 = fwrite(*(void **)(lVar11 * 8 + lVar8 + lVar14 * 8),8,(long)(local_8c[0] + 1),__s)
          ;
          if (local_8c[0] + 1 != (int)sVar10) {
            nrerror_vsdlss(0x10,"factorchunkutils.c",0xe2,"writeFVector:v",local_8c[0] + 1,
                           sVar10 & 0xffffffff);
          }
          iVar9 = (int)lVar14;
          iVar12 = iVar13 + iVar9;
          lVar14 = lVar14 + 1;
        } while (iVar9 + 2 <= *(int *)(*(long *)(param_1 + 0x20) + lVar15 * 4));
      }
      local_f0 = local_f0 + 8;
    } while ((int)lVar15 + 1 <= iVar4);
  }
  closeFactorChunkFile_vsdlss(param_2,iVar2);
  return;
}


