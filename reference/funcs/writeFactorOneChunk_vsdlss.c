/* VSDLSS function (decompiled by Ghidra) */
/* name: writeFactorOneChunk_vsdlss  addr: 00a14c20  size: 877 */
#include "vsdlss_ref.h"

/* signature: void writeFactorOneChunk_vsdlss(long param_1); */

void writeFactorOneChunk_vsdlss(long param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 uVar7;
  void *__ptr;
  int *__ptr_00;
  long lVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  FILE *__s;
  size_t sVar12;
  long lVar13;
  ulong uVar14;
  long lVar15;
  ulong uVar16;
  int iVar17;
  long lVar18;
  long lVar19;
  uint uVar20;
  int iVar21;
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
  uint local_68;
  
  iVar1 = *(int *)(param_1 + 8);
  __ptr = *(void **)(param_1 + 0x28);
  uVar2 = *(undefined4 *)(param_1 + 4);
  uVar3 = *(undefined4 *)(param_1 + 0xc);
  uVar4 = *(undefined4 *)(param_1 + 0x10);
  uVar5 = *(undefined4 *)(param_1 + 0x18);
  iVar6 = *(int *)(param_1 + 0x14);
  uVar7 = *(undefined4 *)(param_1 + 0x1c);
  __ptr_00 = *(int **)(param_1 + 0x20);
  iVar10 = getMaxIVector_vsdlss(__ptr,iVar1);
  lVar15 = *(long *)(param_1 + 0x30);
  iVar17 = iVar1 + 0x3ff;
  if (-1 < iVar1) {
    iVar17 = iVar1;
  }
  lVar8 = *(long *)(param_1 + 0x38);
  iVar9 = iVar17 >> 10;
  if (iVar17 >> 10 <= iVar10) {
    iVar9 = iVar10;
  }
  lVar13 = *(long *)(param_1 + 0x48);
  __s = (FILE *)openFileW_vsdlss(0x11);
  uVar20 = (uint)(lVar13 != 0);
  local_8c = 8;
  local_84 = uVar2;
  local_80 = iVar1;
  local_7c = uVar3;
  local_78 = uVar4;
  local_74 = iVar6;
  local_70 = uVar5;
  local_6c = uVar7;
  local_68 = uVar20;
  sVar12 = fwrite(&local_8c,4,1,__s);
  if ((int)sVar12 != 1) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xb9,"writeIVector:n",1,sVar12 & 0xffffffff);
  }
  sVar12 = fwrite(local_88,4,(long)(local_8c + 1),__s);
  if (local_8c + 1 != (int)sVar12) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xb9,"writeIVector:v",local_8c + 1,sVar12 & 0xffffffff);
  }
  local_8c = iVar6;
  sVar12 = fwrite(&local_8c,4,1,__s);
  if ((int)sVar12 != 1) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xbb,"writeIVector:n",1,sVar12 & 0xffffffff);
  }
  sVar12 = fwrite(__ptr_00,4,(long)(local_8c + 1),__s);
  if (local_8c + 1 != (int)sVar12) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xbb,"writeIVector:v",local_8c + 1,sVar12 & 0xffffffff);
  }
  local_8c = iVar1;
  sVar12 = fwrite(&local_8c,4,1,__s);
  if ((int)sVar12 != 1) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xbc,"writeIVector:n",1,sVar12 & 0xffffffff);
  }
  sVar12 = fwrite(__ptr,4,(long)(local_8c + 1),__s);
  if (local_8c + 1 != (int)sVar12) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xbc,"writeIVector:v",local_8c + 1,sVar12 & 0xffffffff);
  }
  if (uVar20 != 0) {
    local_8c = iVar1;
    sVar12 = fwrite(&local_8c,4,1,__s);
    if ((int)sVar12 != 1) {
      nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xbe,"writeIVector:n",1,sVar12 & 0xffffffff);
    }
    sVar12 = fwrite(*(void **)(param_1 + 0x48),4,(long)(local_8c + 1),__s);
    if (local_8c + 1 != (int)sVar12) {
      nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xbe,"writeIVector:v",local_8c + 1,sVar12 & 0xffffffff
                    );
    }
  }
  lVar13 = newIVector_vsdlss((long)iVar9,0,0);
  local_90 = 0;
  local_8c = 0;
  if (0 < iVar6) {
    iVar21 = 1;
    lVar18 = 1;
    iVar17 = *(int *)((long)__ptr + 4);
    iVar10 = 1;
    lVar19 = lVar13;
    iVar11 = iVar17;
    while( true ) {
      __ptr_00 = __ptr_00 + 1;
      if (iVar9 < iVar11) {
        sVar12 = fwrite(&local_90,4,1,__s);
        if ((int)sVar12 != 1) {
          nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xcb,"writeFactorOneChunk:currSize",1,
                         sVar12 & 0xffffffff);
        }
        sVar12 = fwrite(&local_8c,4,1,__s);
        if ((int)sVar12 != 1) {
          nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xce,"writeFactorOneChunk:nVecsPacked",1,
                         sVar12 & 0xffffffff);
        }
        iVar11 = local_90;
        sVar12 = fwrite((void *)(lVar13 + 4),4,(long)local_90,__s);
        if ((int)sVar12 != iVar11) {
          nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xd1,"writeFactorOneChunk:indBuff",iVar11,
                         sVar12 & 0xffffffff);
        }
        local_90 = 0;
        local_8c = 0;
        lVar19 = lVar13;
      }
      uVar16 = (ulong)iVar17;
      lVar18 = *(long *)(lVar15 + lVar18 * 8);
      uVar14 = 1;
      if (uVar16 != 0) {
        do {
          *(undefined4 *)(lVar19 + uVar14 * 4) = *(undefined4 *)(lVar18 + uVar14 * 4);
          uVar14 = uVar14 + 1;
        } while (uVar14 <= uVar16);
      }
      local_90 = iVar17 + local_90;
      local_8c = local_8c + 1;
      iVar10 = iVar10 + 1;
      lVar19 = lVar19 + uVar16 * 4;
      iVar21 = iVar21 + *__ptr_00;
      if (iVar6 < iVar10) break;
      lVar18 = (long)iVar21;
      iVar17 = *(int *)((long)__ptr + lVar18 * 4);
      iVar11 = local_90 + iVar17;
    }
    if (0 < local_90) {
      sVar12 = fwrite(&local_90,4,1,__s);
      if ((int)sVar12 != 1) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xe1,"writeFactorOneChunk:currSize",1,
                       sVar12 & 0xffffffff);
      }
      sVar12 = fwrite(&local_8c,4,1,__s);
      if ((int)sVar12 != 1) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xe4,"writeFactorOneChunk:nVecsPacked",1,
                       sVar12 & 0xffffffff);
      }
      iVar6 = local_90;
      sVar12 = fwrite((void *)(lVar13 + 4),4,(long)local_90,__s);
      if ((int)sVar12 != iVar6) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xe7,"writeFactorOneChunk:indBuff",iVar6,
                       sVar12 & 0xffffffff);
      }
    }
  }
  if (lVar13 != 0) {
    casiFree_vsdlss();
  }
  lVar15 = newFVector_vsdlss(0,(long)iVar9,0);
  local_90 = 0;
  local_8c = 0;
  if (0 < iVar1) {
    lVar13 = lVar15;
    lVar19 = 0;
    do {
      iVar6 = *(int *)((long)__ptr + lVar19 * 4 + 4);
      if (iVar9 < local_90 + iVar6) {
        sVar12 = fwrite(&local_90,4,1,__s);
        if ((int)sVar12 != 1) {
          nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xf4,"writeFactorOneChunk:currSize",1,
                         sVar12 & 0xffffffff);
        }
        sVar12 = fwrite(&local_8c,4,1,__s);
        if ((int)sVar12 != 1) {
          nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xf7,"writeFactorOneChunk:nVecsPacked",1,
                         sVar12 & 0xffffffff);
        }
        iVar17 = local_90;
        sVar12 = fwrite((void *)(lVar15 + 8),8,(long)local_90,__s);
        if ((int)sVar12 != iVar17) {
          nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xfa,"writeFactorOneChunk:valBuff",iVar17,
                         sVar12 & 0xffffffff);
        }
        local_8c = 1;
        lVar13 = lVar15;
        local_90 = iVar6;
      }
      else {
        local_8c = local_8c + 1;
        local_90 = local_90 + iVar6;
      }
      uVar16 = (ulong)iVar6;
      lVar18 = *(long *)(lVar8 + 8 + lVar19 * 8);
      uVar14 = 1;
      if (uVar16 != 0) {
        do {
          *(undefined8 *)(lVar13 + uVar14 * 8) = *(undefined8 *)(lVar18 + uVar14 * 8);
          uVar14 = uVar14 + 1;
        } while (uVar14 <= uVar16);
      }
      lVar13 = lVar13 + uVar16 * 8;
      iVar6 = (int)lVar19;
      lVar19 = lVar19 + 1;
    } while (iVar6 + 2 <= iVar1);
    if (0 < local_90) {
      sVar12 = fwrite(&local_90,4,1,__s);
      if ((int)sVar12 != 1) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x108,"writeFactorOneChunk:currSize",1,
                       sVar12 & 0xffffffff);
      }
      sVar12 = fwrite(&local_8c,4,1,__s);
      if ((int)sVar12 != 1) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x10b,"writeFactorOneChunk:nVecsPacked",1,
                       sVar12 & 0xffffffff);
      }
      iVar1 = local_90;
      sVar12 = fwrite((void *)(lVar15 + 8),8,(long)local_90,__s);
      if ((int)sVar12 != iVar1) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0x10e,"writeFactorOneChunk:valBuff",iVar1,
                       sVar12 & 0xffffffff);
      }
    }
  }
  if (lVar15 != 0) {
    casiFree_vsdlss(lVar15);
  }
  casiFClose_vsdlss(0x11);
  return;
}


