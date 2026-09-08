/* driver: OByAdy_QLOmKBaERKHXSPAIPP addr=00968f90 size=6c5 */
#include "vsdlss_ref.h"


undefined8 * OByAdy_QLOmKBaERKHXSPAIPP(void)

{
  long *plVar1;
  undefined8 uVar2;
  long lVar3;
  long lVar4;
  FILE *__stream;
  size_t sVar5;
  undefined8 *puVar6;
  void *__ptr;
  void *__ptr_00;
  long lVar7;
  long lVar8;
  long lVar9;
  size_t sVar10;
  long lVar11;
  undefined8 uVar12;
  long lVar13;
  long lVar14;
  long local_110;
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
  long local_88;
  
  __stream = (FILE *)LMBKdFIBpXSPAIPP(0x11);
  sVar5 = fread(&local_d0,8,1,__stream);
  if (sVar5 != 1) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x143,"readIVectorNoNewVec:n",1,sVar5);
  }
  sVar5 = fread(local_c8,8,local_d0 + 1,__stream);
  if (local_d0 + 1U != sVar5) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x143,"readIVectorNoNewVec:v");
  }
  puVar6 = (undefined8 *)KBTmKBdy_QLOaERKHfByABOXSPAIPP(local_b8);
  puVar6[1] = local_c0;
  uVar2 = *puVar6;
  puVar6[3] = local_b0;
  puVar6[4] = local_a8;
  puVar6[5] = local_a0;
  puVar6[6] = local_98;
  puVar6[2] = local_b8;
  puVar6[7] = local_90;
  __ptr = (void *)KBTgtB_QLOXSPAIPP(local_a0,0,0);
  puVar6[8] = __ptr;
  __ptr_00 = (void *)QyDlBTgtB_QLOXSPAIPP(uVar2,local_b8,0,0);
  puVar6[9] = __ptr_00;
  lVar7 = QyDlBTgntB_QLOXSPAIPP(uVar2,local_b8,1,0);
  puVar6[10] = lVar7;
  lVar8 = QyDlBTdntB_QLOXSPAIPP(uVar2,local_b8,1,0);
  puVar6[0xb] = lVar8;
  sVar5 = fread(&local_d0,8,1,__stream);
  if (sVar5 != 1) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x160,"readIVectorNoNewVec:n",1,sVar5);
  }
  sVar5 = fread(__ptr,8,local_d0 + 1,__stream);
  if (local_d0 + 1U != sVar5) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x160,"readIVectorNoNewVec:v");
  }
  sVar5 = fread(&local_d0,8,1,__stream);
  if (sVar5 != 1) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x161,"readIVectorNoNewVec:n",1,sVar5);
  }
  sVar5 = fread(__ptr_00,8,local_d0 + 1,__stream);
  if (local_d0 + 1U != sVar5) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x161,"readIVectorNoNewVec:v");
  }
  if (local_88 != 0) {
    uVar12 = QyDlBTgtB_QLOXSPAIPP(uVar2,local_b8,0,0);
    puVar6[0xd] = uVar12;
    sVar5 = fread(&local_d0,8,1,__stream);
    if (sVar5 != 1) {
      KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x164,"readIVectorNoNewVec:n",1,sVar5);
    }
    sVar5 = fread((void *)puVar6[0xd],8,local_d0 + 1,__stream);
    if (local_d0 + 1U != sVar5) {
      KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x164,"readIVectorNoNewVec:v");
    }
  }
  if (0 < local_a0) {
    local_110 = 1;
    lVar14 = 1;
    do {
      sVar5 = fread(&local_d8,8,1,__stream);
      if (sVar5 != 1) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x16c,"readFactorOneChunk:currSize",1,sVar5);
      }
      lVar9 = QyDlBTgtB_QLOXSPAIPP(uVar2,local_d8,0,0);
      sVar5 = fread(&local_d0,8,1,__stream);
      if (sVar5 != 1) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x170,"readFactorOneChunk:nVecsPacked",1,sVar5);
      }
      sVar5 = local_d8;
      sVar10 = fread((void *)(lVar9 + 8),8,local_d8,__stream);
      if (sVar10 != sVar5) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x173,"readFactorOneChunk:indBuff",sVar5,sVar10);
      }
      if (local_d0 < 1) {
        sVar5 = 0;
        lVar13 = local_110;
      }
      else {
        lVar13 = local_d0 + local_110;
        sVar5 = 0;
        do {
          lVar3 = *(long *)((long)__ptr + local_110 * 8);
          plVar1 = (long *)(lVar7 + lVar14 * 8);
          lVar4 = *(long *)((long)__ptr_00 + lVar14 * 8);
          *plVar1 = lVar9;
          if (1 < lVar3) {
            lVar11 = 0;
            do {
              *(long *)((long)plVar1 + lVar11 + 8) = lVar9 + 8 + lVar11;
              lVar11 = lVar11 + 8;
            } while (lVar11 != lVar3 * 8 + -8);
          }
          local_110 = local_110 + 1;
          sVar5 = sVar5 + lVar4;
          lVar14 = lVar14 + lVar3;
          lVar9 = lVar9 + lVar4 * 8;
        } while (local_110 != lVar13);
      }
      local_110 = lVar13;
      if ((local_d8 != sVar5) || (local_a0 + 1 < local_110)) {
        KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x183);
      }
    } while (local_110 <= local_a0);
  }
  lVar7 = 1;
  if (0 < local_b8) {
    do {
      sVar5 = fread(&local_d8,8,1,__stream);
      if (sVar5 != 1) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x18c,"readFactorOneChunk:currSize",1,sVar5);
      }
      lVar14 = QyDlBTdtB_QLOXSPAIPP(0,uVar2,local_d8,0);
      sVar5 = fread(&local_d0,8,1,__stream);
      if (sVar5 != 1) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",400,"readFactorOneChunk:nVecsPacked",1,sVar5);
      }
      sVar5 = local_d8;
      sVar10 = fread((void *)(lVar14 + 8),8,local_d8,__stream);
      if (sVar10 != sVar5) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0x193,"readFactorOneChunk:valBuff",sVar5,sVar10);
      }
      if (local_d0 < 1) {
        sVar5 = 0;
        lVar9 = lVar7;
      }
      else {
        lVar9 = local_d0 + lVar7;
        sVar5 = 0;
        do {
          lVar13 = *(long *)((long)__ptr_00 + lVar7 * 8);
          *(long *)(lVar8 + lVar7 * 8) = lVar14;
          lVar7 = lVar7 + 1;
          sVar5 = sVar5 + lVar13;
          lVar14 = lVar14 + lVar13 * 8;
        } while (lVar7 != lVar9);
      }
      lVar7 = lVar9;
      if ((local_d8 != sVar5) || (local_b8 + 1 < lVar7)) {
        KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x19e);
      }
    } while (lVar7 <= local_b8);
  }
  _yPFdaILPBXSPAIPP(0x11);
  return puVar6;
}


