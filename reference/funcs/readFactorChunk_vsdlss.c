/* VSDLSS function (decompiled by Ghidra) */
/* name: readFactorChunk_vsdlss  addr: 009f2ad0  size: 5d2 */
#include "vsdlss_ref.h"

/* signature: undefined4 * readFactorChunk_vsdlss(int param_1,int param_2,long param_3,long param_4); */

undefined4 * readFactorChunk_vsdlss(int param_1,int param_2,long param_3,long param_4)

{
  void *pvVar1;
  uint uVar2;
  long lVar3;
  long lVar4;
  int iVar5;
  int iVar6;
  FILE *__stream;
  size_t sVar7;
  long lVar8;
  undefined4 *puVar9;
  void *__ptr;
  long lVar10;
  long lVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  long lVar15;
  undefined4 uVar16;
  long lVar17;
  int iVar18;
  long lVar19;
  void *pvVar20;
  void *local_130;
  long local_e0;
  int local_d0;
  int local_cc;
  int local_8c;
  undefined1 local_88 [4];
  int local_84;
  
  __stream = (FILE *)openFactorChunkFileR_vsdlss(param_3,param_1);
  iVar12 = *(int *)(param_3 + 4) >> 10;
  if (*(int *)(param_3 + 4) < 0x400) {
    iVar12 = 1;
  }
  sVar7 = fread(&local_8c,4,1,__stream);
  if ((int)sVar7 != 1) {
    nrerror_vsdlss(0x10,"factorchunkutils.c",0x116,"readIVectorNoNewVec:n",1,sVar7 & 0xffffffff);
  }
  sVar7 = fread(local_88,4,(long)(local_8c + 1),__stream);
  if (local_8c + 1 != (int)sVar7) {
    nrerror_vsdlss(0x10,"factorchunkutils.c",0x116,"readIVectorNoNewVec:v",local_8c + 1,
                   sVar7 & 0xffffffff);
  }
  if (local_84 != param_1) {
    nrerror_vsdlss(0x24,"factorchunkutils.c",0x118);
  }
  lVar8 = (long)param_1;
  if (param_2 == 2) {
    uVar16 = *(undefined4 *)(*(long *)(param_3 + 0x58) + lVar8 * 4);
    iVar18 = *(int *)(*(long *)(param_3 + 0x60) + lVar8 * 4);
    local_cc = *(int *)(*(long *)(param_3 + 0x40) + lVar8 * 4);
    iVar13 = *(int *)(*(long *)(param_3 + 0x48) + lVar8 * 4);
    fseek(__stream,*(long *)(*(long *)(param_3 + 0x68) + lVar8 * 8),0);
  }
  else {
    uVar16 = *(undefined4 *)(*(long *)(param_3 + 0x50) + lVar8 * 4);
    local_cc = *(int *)(*(long *)(param_3 + 0x38) + lVar8 * 4);
    iVar18 = *(int *)(*(long *)(param_3 + 0x58) + lVar8 * 4) + -1;
    iVar13 = *(int *)(*(long *)(param_3 + 0x40) + lVar8 * 4) + -1;
  }
  puVar9 = (undefined4 *)newFactorChunk_vsdlss(param_1,uVar16,iVar18,local_cc,iVar13);
  lVar8 = *(long *)(puVar9 + 10);
  lVar3 = *(long *)(puVar9 + 0xc);
  lVar4 = *(long *)(puVar9 + 0xe);
  uVar16 = *puVar9;
  iVar18 = (iVar13 - local_cc) + 1;
  if (0 < iVar18) {
    lVar17 = 0;
    iVar13 = 0;
    local_d0 = 0;
    local_130 = (void *)0x0;
    pvVar20 = (void *)0x0;
    iVar6 = 1;
    local_e0 = (long)local_cc << 2;
    do {
      sVar7 = fread(&local_8c,4,1,__stream);
      if ((int)sVar7 != 1) {
        nrerror_vsdlss(0x10,"factorchunkutils.c",0x144,"readIVectorNoNewVec:n",1,sVar7 & 0xffffffff)
        ;
      }
      sVar7 = fread(local_88,4,(long)(local_8c + 1),__stream);
      if (local_8c + 1 != (int)sVar7) {
        nrerror_vsdlss(0x10,"factorchunkutils.c",0x144,"readIVectorNoNewVec:v",local_8c + 1,
                       sVar7 & 0xffffffff);
      }
      if (local_cc + (int)lVar17 != local_84) {
        nrerror_vsdlss(0x24,"factorchunkutils.c",0x146);
      }
      lVar17 = lVar17 + 1;
      *(undefined4 *)(*(long *)(puVar9 + 8) + lVar17 * 4) =
           *(undefined4 *)(*(long *)(param_4 + 0x10) + local_e0);
      uVar2 = *(uint *)(*(long *)(param_4 + 0x18) + local_e0);
      __ptr = pvVar20;
      if (local_d0 <= (int)uVar2) {
        local_d0 = uVar2 + 1;
        if ((int)uVar2 < iVar12) {
          local_d0 = iVar12;
        }
        __ptr = (void *)newBlock_vsdlss(uVar16,(long)local_d0 << 2);
      }
      local_d0 = local_d0 + ~uVar2;
      pvVar20 = (void *)((long)__ptr + ((long)(int)uVar2 + 1) * 4);
      sVar7 = fread(&local_8c,4,1,__stream);
      if ((int)sVar7 != 1) {
        nrerror_vsdlss(0x10,"factorchunkutils.c",0x158,"readIVectorNoNewVec:n",1,sVar7 & 0xffffffff)
        ;
      }
      sVar7 = fread(__ptr,4,(long)(local_8c + 1),__stream);
      if (local_8c + 1 != (int)sVar7) {
        nrerror_vsdlss(0x10,"factorchunkutils.c",0x158,"readIVectorNoNewVec:v",local_8c + 1,
                       sVar7 & 0xffffffff);
      }
      if (0 < *(int *)(*(long *)(puVar9 + 8) + lVar17 * 4)) {
        lVar10 = (long)iVar6;
        lVar19 = ((long)(int)uVar2 + 1) * 8;
        lVar11 = lVar10 * 8;
        lVar15 = 0;
        iVar5 = iVar6 + 1;
        do {
          iVar14 = (int)lVar15;
          iVar6 = uVar2 - iVar14;
          *(int *)(lVar8 + lVar10 * 4 + lVar15 * 4) = iVar6;
          *(void **)(lVar3 + lVar11 + lVar15 * 8) = __ptr;
          if (iVar13 <= iVar6) {
            iVar13 = (uVar2 + 1) - iVar14;
            if (iVar6 < iVar12) {
              iVar13 = iVar12;
            }
            local_130 = (void *)newBlock_vsdlss(uVar16,(long)iVar13 << 3);
          }
          pvVar1 = (void *)((long)local_130 + lVar19);
          iVar13 = iVar13 + ~uVar2 + iVar14;
          sVar7 = fread(&local_8c,4,1,__stream);
          if ((int)sVar7 != 1) {
            nrerror_vsdlss(0x10,"factorchunkutils.c",0x167,"readFVectorNoNewVec:n",1,
                           sVar7 & 0xffffffff);
          }
          sVar7 = fread(local_130,8,(long)(local_8c + 1),__stream);
          if (local_8c + 1 != (int)sVar7) {
            nrerror_vsdlss(0x10,"factorchunkutils.c",0x167,"readFVectorNoNewVec:v",local_8c + 1,
                           sVar7 & 0xffffffff);
          }
          __ptr = (void *)((long)__ptr + 4);
          lVar19 = lVar19 + -8;
          *(void **)(lVar11 + lVar4 + lVar15 * 8) = local_130;
          lVar15 = lVar15 + 1;
          iVar6 = iVar5 + iVar14;
          local_130 = pvVar1;
        } while (iVar14 + 2 <= *(int *)(*(long *)(puVar9 + 8) + lVar17 * 4));
      }
      local_e0 = local_e0 + 4;
    } while ((int)lVar17 + 1 <= iVar18);
  }
  closeFactorChunkFile_vsdlss(param_3,param_1);
  return puVar9;
}


