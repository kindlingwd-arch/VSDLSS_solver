/* driver: TOFQBdy_QLOmKBaERKHXSPAIPP addr=009686f0 size=895 */
#include "vsdlss_ref.h"


void TOFQBdy_QLOmKBaERKHXSPAIPP(long param_1)

{
  undefined8 uVar1;
  long lVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  void *__ptr;
  void *__ptr_00;
  undefined8 uVar5;
  undefined8 uVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  FILE *__s;
  size_t sVar10;
  long lVar11;
  ulong uVar12;
  size_t sVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  ulong uVar19;
  size_t local_d8;
  long local_d0;
  undefined1 local_c8 [8];
  undefined8 local_c0;
  long local_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  long local_a0;
  undefined8 local_98;
  undefined8 local_90;
  ulong local_88;
  
  uVar1 = *(undefined8 *)(param_1 + 8);
  lVar2 = *(long *)(param_1 + 0x10);
  uVar3 = *(undefined8 *)(param_1 + 0x18);
  uVar4 = *(undefined8 *)(param_1 + 0x20);
  __ptr = *(void **)(param_1 + 0x40);
  lVar14 = *(long *)(param_1 + 0x28);
  __ptr_00 = *(void **)(param_1 + 0x48);
  uVar5 = *(undefined8 *)(param_1 + 0x30);
  uVar6 = *(undefined8 *)(param_1 + 0x38);
  lVar9 = DBQkyUgtB_QLOXSPAIPP(__ptr_00,lVar2);
  lVar16 = *(long *)(param_1 + 0x50);
  lVar11 = lVar2 + 0x3ff;
  if (-1 < lVar2) {
    lVar11 = lVar2;
  }
  lVar7 = *(long *)(param_1 + 0x58);
  lVar15 = lVar11 >> 10;
  if (lVar11 >> 10 <= lVar9) {
    lVar15 = lVar9;
  }
  uVar19 = (ulong)(*(long *)(param_1 + 0x68) != 0);
  __s = (FILE *)LMBKdFIBuXSPAIPP(0x11);
  local_d0 = 8;
  local_c0 = uVar1;
  local_b8 = lVar2;
  local_b0 = uVar3;
  local_a8 = uVar4;
  local_a0 = lVar14;
  local_98 = uVar5;
  local_90 = uVar6;
  local_88 = uVar19;
  sVar10 = fwrite(&local_d0,8,1,__s);
  if (sVar10 != 1) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xb9,"writeIVector:n",1,sVar10);
  }
  sVar10 = fwrite(local_c8,8,local_d0 + 1,__s);
  if (local_d0 + 1U != sVar10) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xb9,"writeIVector:v",local_d0 + 1U,sVar10);
  }
  local_d0 = lVar14;
  sVar10 = fwrite(&local_d0,8,1,__s);
  if (sVar10 != 1) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xbb,"writeIVector:n",1,sVar10);
  }
  sVar10 = fwrite(__ptr,8,local_d0 + 1,__s);
  if (local_d0 + 1U != sVar10) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xbb,"writeIVector:v",local_d0 + 1U,sVar10);
  }
  local_d0 = lVar2;
  sVar10 = fwrite(&local_d0,8,1,__s);
  if (sVar10 != 1) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xbc,"writeIVector:n",1,sVar10);
  }
  sVar10 = fwrite(__ptr_00,8,local_d0 + 1,__s);
  if (local_d0 + 1U != sVar10) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xbc,"writeIVector:v",local_d0 + 1U,sVar10);
  }
  if (uVar19 != 0) {
    local_d0 = lVar2;
    sVar10 = fwrite(&local_d0,8,1,__s);
    if (sVar10 != 1) {
      KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xbe,"writeIVector:n",1,sVar10);
    }
    sVar10 = fwrite(*(void **)(param_1 + 0x68),8,local_d0 + 1,__s);
    if (local_d0 + 1U != sVar10) {
      KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xbe,"writeIVector:v",local_d0 + 1U,sVar10);
    }
  }
  lVar11 = KBTgtB_QLOXSPAIPP(lVar15,0,0);
  local_d8 = 0;
  local_d0 = 0;
  if (0 < lVar14) {
    lVar17 = 1;
    lVar18 = 1;
    lVar9 = lVar11;
    do {
      uVar19 = *(ulong *)((long)__ptr_00 + lVar17 * 8);
      if (lVar15 < (long)(local_d8 + uVar19)) {
        sVar10 = fwrite(&local_d8,8,1,__s);
        if (sVar10 != 1) {
          KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xcb,"writeFactorOneChunk:currSize",1,sVar10);
        }
        sVar10 = fwrite(&local_d0,8,1,__s);
        if (sVar10 != 1) {
          KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xce,"writeFactorOneChunk:nVecsPacked",1,sVar10);
        }
        sVar10 = local_d8;
        sVar13 = fwrite((void *)(lVar11 + 8),8,local_d8,__s);
        if (sVar13 != sVar10) {
          KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xd1,"writeFactorOneChunk:indBuff",sVar10,sVar13);
        }
        local_d8 = 0;
        local_d0 = 0;
        lVar9 = lVar11;
      }
      lVar8 = *(long *)(lVar16 + lVar17 * 8);
      uVar12 = 1;
      if (uVar19 != 0) {
        do {
          *(undefined8 *)(lVar9 + uVar12 * 8) = *(undefined8 *)(lVar8 + uVar12 * 8);
          uVar12 = uVar12 + 1;
        } while (uVar12 <= uVar19);
      }
      lVar9 = lVar9 + uVar19 * 8;
      local_d8 = uVar19 + local_d8;
      local_d0 = local_d0 + 1;
      lVar17 = lVar17 + *(long *)((long)__ptr + lVar18 * 8);
      lVar18 = lVar18 + 1;
    } while (lVar18 <= lVar14);
    if (0 < (long)local_d8) {
      sVar10 = fwrite(&local_d8,8,1,__s);
      if (sVar10 != 1) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xe1,"writeFactorOneChunk:currSize",1,sVar10);
      }
      sVar10 = fwrite(&local_d0,8,1,__s);
      if (sVar10 != 1) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xe4,"writeFactorOneChunk:nVecsPacked",1,sVar10);
      }
      sVar10 = local_d8;
      sVar13 = fwrite((void *)(lVar11 + 8),8,local_d8,__s);
      if (sVar13 != sVar10) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xe7,"writeFactorOneChunk:indBuff",sVar10,sVar13);
      }
    }
  }
  if (lVar11 != 0) {
    _yPFdOBBXSPAIPP();
  }
  lVar14 = KBTdtB_QLOXSPAIPP(0,lVar15);
  local_d8 = 0;
  local_d0 = 0;
  if (0 < lVar2) {
    lVar11 = 1;
    lVar16 = lVar14;
    do {
      uVar19 = *(ulong *)((long)__ptr_00 + lVar11 * 8);
      if (lVar15 < (long)(local_d8 + uVar19)) {
        sVar10 = fwrite(&local_d8,8,1,__s);
        if (sVar10 != 1) {
          KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xf4,"writeFactorOneChunk:currSize",1,sVar10);
        }
        sVar10 = fwrite(&local_d0,8,1,__s);
        if (sVar10 != 1) {
          KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xf7,"writeFactorOneChunk:nVecsPacked",1,sVar10);
        }
        sVar10 = local_d8;
        sVar13 = fwrite((void *)(lVar14 + 8),8,local_d8,__s);
        if (sVar13 != sVar10) {
          KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xfa,"writeFactorOneChunk:valBuff",sVar10,sVar13);
        }
        local_d0 = 1;
        lVar16 = lVar14;
        local_d8 = uVar19;
      }
      else {
        local_d0 = local_d0 + 1;
        local_d8 = local_d8 + uVar19;
      }
      lVar9 = *(long *)(lVar7 + lVar11 * 8);
      uVar12 = 1;
      if (uVar19 != 0) {
        do {
          *(undefined8 *)(lVar16 + uVar12 * 8) = *(undefined8 *)(lVar9 + uVar12 * 8);
          uVar12 = uVar12 + 1;
        } while (uVar12 <= uVar19);
      }
      lVar11 = lVar11 + 1;
      lVar16 = lVar16 + uVar19 * 8;
    } while (lVar11 <= lVar2);
    if (0 < (long)local_d8) {
      sVar10 = fwrite(&local_d8,8,1,__s);
      if (sVar10 != 1) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x108,"writeFactorOneChunk:currSize",1,sVar10);
      }
      sVar10 = fwrite(&local_d0,8,1,__s);
      if (sVar10 != 1) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x10b,"writeFactorOneChunk:nVecsPacked",1,sVar10);
      }
      sVar10 = local_d8;
      sVar13 = fwrite((void *)(lVar14 + 8),8,local_d8,__s);
      if (sVar13 != sVar10) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x10e,"writeFactorOneChunk:valBuff",sVar10,sVar13);
      }
    }
  }
  if (lVar14 != 0) {
    _yPFdOBBXSPAIPP(lVar14);
  }
  _yPFdaILPBXSPAIPP(0x11);
  return;
}


