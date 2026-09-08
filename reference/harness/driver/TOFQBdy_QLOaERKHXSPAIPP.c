/* driver: TOFQBdy_QLOaERKHXSPAIPP addr=00985ed0 size=3d7 */
#include "vsdlss_ref.h"


/* WARNING: Type propagation algorithm not settling */

void TOFQBdy_QLOaERKHXSPAIPP(long param_1,long param_2,long param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  FILE *__s;
  size_t sVar7;
  long lVar8;
  long lVar9;
  long local_128;
  long local_120;
  long local_118;
  long local_d0 [20];
  
  lVar1 = *(long *)(param_1 + 8);
  lVar2 = *(long *)(*(long *)(param_2 + 0x50) + lVar1 * 8);
  __s = (FILE *)LMBKdy_QLOaERKHdFIBuXSPAIPP(param_2,lVar1);
  lVar3 = *(long *)(param_1 + 0x28);
  lVar4 = *(long *)(param_1 + 0x48);
  lVar5 = *(long *)(param_1 + 0x58);
  lVar6 = *(long *)(param_1 + 0x50);
  local_d0[1] = 0;
  local_d0[0] = 1;
  local_d0[2] = *(long *)(param_1 + 8);
  sVar7 = fwrite(local_d0,8,1,__s);
  if (sVar7 != 1) {
    KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0xc9,"writeIVector:n",1,sVar7);
  }
  sVar7 = fwrite(local_d0 + 1,8,local_d0[0] + 1,__s);
  if (local_d0[0] + 1U != sVar7) {
    KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0xc9,"writeIVector:v",local_d0[0] + 1U,sVar7);
  }
  local_128 = *(long *)(param_1 + 0x30);
  lVar9 = 1;
  local_120 = 1;
  local_118 = local_128 << 3;
  if (0 < lVar3) {
    do {
      lVar8 = ftell(__s);
      if (lVar2 == local_128) {
        *(long *)(*(long *)(param_2 + 0x78) + lVar1 * 8) = lVar8;
      }
      *(long *)(*(long *)(param_3 + 0x28) + local_118) = lVar8;
      local_d0[0] = 1;
      local_d0[2] = local_128;
      sVar7 = fwrite(local_d0,8,1,__s);
      if (sVar7 != 1) {
        KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0xdc,"writeIVector:n",1,sVar7);
      }
      sVar7 = fwrite(local_d0 + 1,8,local_d0[0] + 1,__s);
      if (local_d0[0] + 1U != sVar7) {
        KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0xdc,"writeIVector:v",local_d0[0] + 1U,sVar7);
      }
      local_d0[0] = *(long *)(lVar4 + lVar9 * 8);
      sVar7 = fwrite(local_d0,8,1,__s);
      if (sVar7 != 1) {
        KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0xdf,"writeIVector:n",1,sVar7);
      }
      sVar7 = fwrite(*(void **)(lVar6 + lVar9 * 8),8,local_d0[0] + 1,__s);
      if (local_d0[0] + 1U != sVar7) {
        KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0xdf,"writeIVector:v",local_d0[0] + 1U,sVar7);
      }
      if (0 < *(long *)(*(long *)(param_1 + 0x40) + local_120 * 8)) {
        lVar8 = 1;
        do {
          local_d0[0] = *(long *)(lVar4 + lVar9 * 8);
          sVar7 = fwrite(local_d0,8,1,__s);
          if (sVar7 != 1) {
            KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0xe2,"writeFVector:n",1,sVar7);
          }
          sVar7 = fwrite(*(void **)(lVar5 + lVar9 * 8),8,local_d0[0] + 1,__s);
          if (local_d0[0] + 1U != sVar7) {
            KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0xe2,"writeFVector:v",local_d0[0] + 1U,sVar7);
          }
          lVar9 = lVar9 + 1;
          lVar8 = lVar8 + 1;
        } while (lVar8 <= *(long *)(*(long *)(param_1 + 0x40) + local_120 * 8));
      }
      local_120 = local_120 + 1;
      local_128 = local_128 + 1;
      local_118 = local_118 + 8;
    } while (local_120 <= lVar3);
  }
  _ILPBdy_QLOaERKHdFIBXSPAIPP(param_2,lVar1);
  return;
}


