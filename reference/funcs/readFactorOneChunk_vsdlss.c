/* VSDLSS function (decompiled by Ghidra) */
/* name: readFactorOneChunk_vsdlss  addr: 00a154a0  size: 6c2 */
#include "vsdlss_ref.h"

/* signature: undefined4 * readFactorOneChunk_vsdlss(void); */

undefined4 * readFactorOneChunk_vsdlss(void)

{
  long *plVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  FILE *__stream;
  size_t sVar5;
  undefined4 *puVar6;
  void *__ptr;
  void *__ptr_00;
  long lVar7;
  long lVar8;
  long lVar9;
  undefined8 uVar10;
  int *piVar11;
  int iVar12;
  long lVar13;
  int iVar14;
  int local_dc;
  int local_90;
  int local_8c;
  undefined1 local_88 [4];
  undefined4 local_84;
  int local_80;
  undefined4 local_7c;
  undefined4 local_78;
  int local_74;
  undefined4 local_70;
  undefined4 local_6c;
  int local_68;
  
  __stream = (FILE *)openFileR_vsdlss(0x11);
  sVar5 = fread(&local_8c,4,1,__stream);
  if ((int)sVar5 != 1) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x143,"readIVectorNoNewVec:n",1,sVar5 & 0xffffffff);
  }
  sVar5 = fread(local_88,4,(long)(local_8c + 1),__stream);
  if (local_8c + 1 != (int)sVar5) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x143,"readIVectorNoNewVec:v",local_8c + 1,
                   sVar5 & 0xffffffff);
  }
  puVar6 = (undefined4 *)newOneFactorChunkHeader_vsdlss(local_80);
  uVar2 = *puVar6;
  puVar6[3] = local_7c;
  puVar6[4] = local_78;
  puVar6[1] = local_84;
  puVar6[6] = local_70;
  puVar6[2] = local_80;
  puVar6[5] = local_74;
  puVar6[7] = local_6c;
  __ptr = (void *)newIVector_vsdlss((long)local_74,0,0);
  lVar13 = (long)local_80;
  *(void **)(puVar6 + 8) = __ptr;
  __ptr_00 = (void *)tagNewIVector_vsdlss(uVar2,lVar13,0,0);
  *(void **)(puVar6 + 10) = __ptr_00;
  lVar7 = tagNewIPVector_vsdlss(uVar2,lVar13,1,0);
  *(long *)(puVar6 + 0xc) = lVar7;
  lVar8 = tagNewFPVector_vsdlss(uVar2,lVar13,1,0);
  *(long *)(puVar6 + 0xe) = lVar8;
  sVar5 = fread(&local_8c,4,1,__stream);
  if ((int)sVar5 != 1) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x160,"readIVectorNoNewVec:n",1,sVar5 & 0xffffffff);
  }
  sVar5 = fread(__ptr,4,(long)(local_8c + 1),__stream);
  if (local_8c + 1 != (int)sVar5) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x160,"readIVectorNoNewVec:v",local_8c + 1,
                   sVar5 & 0xffffffff);
  }
  sVar5 = fread(&local_8c,4,1,__stream);
  if ((int)sVar5 != 1) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x161,"readIVectorNoNewVec:n",1,sVar5 & 0xffffffff);
  }
  sVar5 = fread(__ptr_00,4,(long)(local_8c + 1),__stream);
  if (local_8c + 1 != (int)sVar5) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x161,"readIVectorNoNewVec:v",local_8c + 1,
                   sVar5 & 0xffffffff);
  }
  if (local_68 != 0) {
    uVar10 = tagNewIVector_vsdlss(uVar2,lVar13,0,0);
    *(undefined8 *)(puVar6 + 0x12) = uVar10;
    sVar5 = fread(&local_8c,4,1,__stream);
    if ((int)sVar5 != 1) {
      nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x164,"readIVectorNoNewVec:n",1,sVar5 & 0xffffffff);
    }
    sVar5 = fread(*(void **)(puVar6 + 0x12),4,(long)(local_8c + 1),__stream);
    if (local_8c + 1 != (int)sVar5) {
      nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x164,"readIVectorNoNewVec:v",local_8c + 1,
                     sVar5 & 0xffffffff);
    }
  }
  if (0 < local_74) {
    local_dc = 1;
    iVar14 = 1;
    do {
      sVar5 = fread(&local_90,4,1,__stream);
      if ((int)sVar5 != 1) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x16c,"readFactorOneChunk:currSize",1,
                       sVar5 & 0xffffffff);
      }
      lVar13 = tagNewIVector_vsdlss(uVar2,(long)local_90,0,0);
      sVar5 = fread(&local_8c,4,1,__stream);
      if ((int)sVar5 != 1) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x170,"readFactorOneChunk:nVecsPacked",1,
                       sVar5 & 0xffffffff);
      }
      iVar12 = local_90;
      sVar5 = fread((void *)(lVar13 + 4),4,(long)local_90,__stream);
      if ((int)sVar5 != iVar12) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x173,"readFactorOneChunk:indBuff",iVar12,
                       sVar5 & 0xffffffff);
      }
      if (local_8c < 1) {
        iVar12 = 0;
      }
      else {
        iVar12 = 0;
        piVar11 = (int *)((long)__ptr + (long)local_dc * 4);
        do {
          iVar3 = *piVar11;
          plVar1 = (long *)(lVar7 + (long)iVar14 * 8);
          iVar4 = *(int *)((long)__ptr_00 + (long)iVar14 * 4);
          *plVar1 = lVar13;
          if (1 < iVar3) {
            lVar9 = 0;
            do {
              *(long *)((long)plVar1 + lVar9 * 2 + 8) = lVar13 + 4 + lVar9;
              lVar9 = lVar9 + 4;
            } while (lVar9 != (ulong)(iVar3 - 2) * 4 + 4);
          }
          piVar11 = piVar11 + 1;
          iVar12 = iVar12 + iVar4;
          iVar14 = iVar14 + iVar3;
          lVar13 = lVar13 + (long)iVar4 * 4;
        } while (piVar11 != (int *)((long)__ptr + ((long)local_dc + (ulong)(local_8c - 1)) * 4 + 4))
        ;
        local_dc = local_dc + local_8c;
      }
      if ((local_90 != iVar12) || (local_74 + 1 < local_dc)) {
        nrerror_vsdlss(1,"vsdlss1memutils.c",0x183);
      }
    } while (local_dc <= local_74);
  }
  iVar14 = 1;
  if (0 < local_80) {
    do {
      sVar5 = fread(&local_90,4,1,__stream);
      if ((int)sVar5 != 1) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x18c,"readFactorOneChunk:currSize",1,
                       sVar5 & 0xffffffff);
      }
      lVar7 = tagNewFVector_vsdlss(0,uVar2,(long)local_90,0);
      sVar5 = fread(&local_8c,4,1,__stream);
      if ((int)sVar5 != 1) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",400,"readFactorOneChunk:nVecsPacked",1,
                       sVar5 & 0xffffffff);
      }
      iVar12 = local_90;
      sVar5 = fread((void *)(lVar7 + 8),8,(long)local_90,__stream);
      if ((int)sVar5 != iVar12) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x193,"readFactorOneChunk:valBuff",iVar12,
                       sVar5 & 0xffffffff);
      }
      if (local_8c < 1) {
        iVar12 = 0;
      }
      else {
        lVar13 = 0;
        iVar12 = 0;
        do {
          iVar3 = *(int *)((long)__ptr_00 + lVar13 + (long)iVar14 * 4);
          *(long *)(lVar8 + (long)iVar14 * 8 + lVar13 * 2) = lVar7;
          lVar13 = lVar13 + 4;
          iVar12 = iVar12 + iVar3;
          lVar7 = lVar7 + (long)iVar3 * 8;
        } while (lVar13 != (ulong)(local_8c - 1) * 4 + 4);
        iVar14 = iVar14 + local_8c;
      }
      if ((local_90 != iVar12) || (local_80 + 1 < iVar14)) {
        nrerror_vsdlss(1,"vsdlss1memutils.c",0x19e);
      }
    } while (iVar14 <= local_80);
  }
  casiFClose_vsdlss(0x11);
  return puVar6;
}


