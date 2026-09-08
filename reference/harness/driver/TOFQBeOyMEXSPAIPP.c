/* driver: TOFQBeOyMEXSPAIPP addr=00988740 size=438 */
#include "vsdlss_ref.h"


void TOFQBeOyMEXSPAIPP(long *param_1)

{
  long lVar1;
  long lVar2;
  FILE *__s;
  ulong uVar3;
  size_t sVar4;
  void *__ptr;
  long lVar5;
  ulong uVar6;
  bool bVar7;
  long local_d0;
  undefined1 local_c8 [8];
  long local_c0;
  long local_b8;
  ulong local_b0;
  ulong local_a8;
  ulong local_a0;
  ulong local_98;
  
  __s = (FILE *)LMBKdFIBuXSPAIPP(0xb);
  lVar1 = param_1[4];
  lVar2 = *param_1;
  lVar5 = param_1[3];
  uVar6 = (ulong)(param_1[2] != 0);
  uVar3 = (ulong)(lVar1 != 0);
  bVar7 = param_1[6] != 0;
  if ((param_1[2] == 0 && bVar7) || (local_c0 = lVar2, bVar7 != (param_1[5] != 0))) {
    KOBOOLOXSPAIPP(1,"graphutils.c",0x22);
    local_c0 = *param_1;
  }
  local_b8 = param_1[1];
  local_d0 = 6;
  local_b0 = uVar6;
  local_a8 = (ulong)(lVar5 != 0);
  local_a0 = uVar3;
  local_98 = (ulong)bVar7;
  sVar4 = fwrite(&local_d0,8,1,__s);
  if (sVar4 != 1) {
    KOBOOLOXSPAIPP(0x10,"graphutils.c",0x2e,"writeIVector:n",1,sVar4);
  }
  sVar4 = fwrite(local_c8,8,local_d0 + 1,__s);
  if (local_d0 + 1U != sVar4) {
    KOBOOLOXSPAIPP(0x10,"graphutils.c",0x2e,"writeIVector:v",local_d0 + 1U,sVar4);
  }
  if (uVar6 != 0) {
    local_d0 = lVar2;
    sVar4 = fwrite(&local_d0,8,1,__s);
    if (sVar4 != 1) {
      KOBOOLOXSPAIPP(0x10,"graphutils.c",0x31,"writeIVector:n",1,sVar4);
    }
    sVar4 = fwrite((void *)param_1[2],8,local_d0 + 1,__s);
    if (local_d0 + 1U != sVar4) {
      KOBOOLOXSPAIPP(0x10,"graphutils.c",0x31,"writeIVector:v",local_d0 + 1U,sVar4);
    }
  }
  if ((ulong)(lVar5 != 0) != 0) {
    local_d0 = lVar2;
    sVar4 = fwrite(&local_d0,8,1,__s);
    if (sVar4 != 1) {
      KOBOOLOXSPAIPP(0x10,"graphutils.c",0x35,"writeIVector:n",1,sVar4);
    }
    sVar4 = fwrite((void *)param_1[3],8,local_d0 + 1,__s);
    if (local_d0 + 1U != sVar4) {
      KOBOOLOXSPAIPP(0x10,"graphutils.c",0x35,"writeIVector:v",local_d0 + 1U,sVar4);
    }
  }
  if (uVar3 != 0) {
    __ptr = (void *)KBTgtB_QLOXSPAIPP(lVar2,0,0);
    lVar5 = 1;
    if (0 < lVar2) {
      do {
        local_d0 = _LMVacADBIFPQrLYOOyVXSPAIPP(*(undefined8 *)(lVar1 + lVar5 * 8),__ptr);
        sVar4 = fwrite(&local_d0,8,1,__s);
        if (sVar4 != 1) {
          KOBOOLOXSPAIPP(0x10,"graphutils.c",0x3c,"writeIVector:n",1,sVar4);
        }
        sVar4 = fwrite(__ptr,8,local_d0 + 1,__s);
        if (local_d0 + 1U != sVar4) {
          KOBOOLOXSPAIPP(0x10,"graphutils.c",0x3c,"writeIVector:v",local_d0 + 1U,sVar4);
        }
        lVar5 = lVar5 + 1;
      } while (lVar5 <= lVar2);
    }
    if (__ptr != (void *)0x0) {
      _yPFdOBBXSPAIPP(__ptr);
    }
  }
  if ((ulong)bVar7 != 0) {
    local_d0 = param_1[1] * 2;
    sVar4 = fwrite(&local_d0,8,1,__s);
    if (sVar4 != 1) {
      KOBOOLOXSPAIPP(0x10,"graphutils.c",0x42,"writeIVector:n",1,sVar4);
    }
    sVar4 = fwrite((void *)param_1[5],8,local_d0 + 1,__s);
    if (local_d0 + 1U != sVar4) {
      KOBOOLOXSPAIPP(0x10,"graphutils.c",0x42,"writeIVector:v",local_d0 + 1U,sVar4);
    }
  }
  _yPFdaILPBXSPAIPP(0xb);
  return;
}


