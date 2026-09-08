/* driver: OByAdy_QLOaERKHXSPAIPP addr=009862b0 size=5c9 */
#include "vsdlss_ref.h"


undefined8 * OByAdy_QLOaERKHXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  void *pvVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  FILE *__stream;
  size_t sVar7;
  undefined8 *puVar8;
  void *__ptr;
  long lVar9;
  long lVar10;
  ulong uVar11;
  undefined8 uVar12;
  ulong uVar13;
  long lVar14;
  long lVar15;
  void *pvVar16;
  void *local_170;
  long local_120;
  long local_118;
  long local_110;
  long local_108;
  long local_d0;
  undefined1 local_c8 [8];
  long local_c0;
  
  __stream = (FILE *)LMBKdy_QLOaERKHdFIBpXSPAIPP(param_3,param_1);
  lVar9 = *(long *)(param_3 + 8) >> 10;
  if (*(long *)(param_3 + 8) < 0x400) {
    lVar9 = 1;
  }
  sVar7 = fread(&local_d0,8,1,__stream);
  if (sVar7 != 1) {
    KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x116,"readIVectorNoNewVec:n",1,sVar7);
  }
  sVar7 = fread(local_c8,8,local_d0 + 1,__stream);
  if (local_d0 + 1U != sVar7) {
    KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x116,"readIVectorNoNewVec:v",local_d0 + 1U,sVar7);
  }
  if (local_c0 != param_1) {
    KOBOOLOXSPAIPP(0x24,"factorchunkutils.c",0x118);
  }
  if (param_2 == 2) {
    uVar12 = *(undefined8 *)(*(long *)(param_3 + 0x68) + param_1 * 8);
    lVar14 = *(long *)(*(long *)(param_3 + 0x70) + param_1 * 8);
    local_110 = *(long *)(*(long *)(param_3 + 0x50) + param_1 * 8);
    lVar10 = *(long *)(*(long *)(param_3 + 0x58) + param_1 * 8);
    fseek(__stream,*(long *)(*(long *)(param_3 + 0x78) + param_1 * 8),0);
  }
  else {
    uVar12 = *(undefined8 *)(*(long *)(param_3 + 0x60) + param_1 * 8);
    local_110 = *(long *)(*(long *)(param_3 + 0x48) + param_1 * 8);
    lVar14 = *(long *)(*(long *)(param_3 + 0x68) + param_1 * 8) + -1;
    lVar10 = *(long *)(*(long *)(param_3 + 0x50) + param_1 * 8) + -1;
  }
  puVar8 = (undefined8 *)KBTdy_QLOaERKHXSPAIPP(param_1,uVar12,lVar14,local_110,lVar10);
  lVar4 = puVar8[9];
  lVar5 = puVar8[10];
  lVar6 = puVar8[0xb];
  uVar12 = *puVar8;
  lVar14 = (lVar10 - local_110) + 1;
  if (0 < lVar14) {
    lVar10 = 0;
    local_120 = 0;
    local_170 = (void *)0x0;
    pvVar16 = (void *)0x0;
    uVar11 = 1;
    local_118 = 1;
    local_108 = local_110 << 3;
    do {
      sVar7 = fread(&local_d0,8,1,__stream);
      if (sVar7 != 1) {
        KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x144,"readIVectorNoNewVec:n",1,sVar7);
      }
      sVar7 = fread(local_c8,8,local_d0 + 1,__stream);
      if (local_d0 + 1U != sVar7) {
        KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x144,"readIVectorNoNewVec:v",local_d0 + 1U,sVar7);
      }
      if (local_c0 != local_110) {
        KOBOOLOXSPAIPP(0x24,"factorchunkutils.c",0x146);
      }
      *(undefined8 *)(puVar8[8] + local_118 * 8) =
           *(undefined8 *)(*(long *)(param_4 + 0x18) + local_108);
      uVar13 = *(ulong *)(*(long *)(param_4 + 0x20) + local_108);
      __ptr = pvVar16;
      if (local_120 <= (long)uVar13) {
        local_120 = lVar9;
        if (lVar9 <= (long)uVar13) {
          local_120 = uVar13 + 1;
        }
        __ptr = (void *)KBTZIL_HXSPAIPP(uVar12,local_120 * 8);
      }
      lVar15 = (uVar13 + 1) * 8;
      pvVar16 = (void *)((long)__ptr + lVar15);
      local_120 = local_120 + ~uVar13;
      sVar7 = fread(&local_d0,8,1,__stream);
      if (sVar7 != 1) {
        KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x158,"readIVectorNoNewVec:n",1,sVar7);
      }
      sVar7 = fread(__ptr,8,local_d0 + 1,__stream);
      if (local_d0 + 1U != sVar7) {
        KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x158,"readIVectorNoNewVec:v",local_d0 + 1U,sVar7);
      }
      if (0 < *(long *)(puVar8[8] + local_118 * 8)) {
        lVar2 = 1 - uVar11;
        lVar3 = ~uVar11 - uVar13;
        do {
          *(ulong *)(lVar4 + uVar11 * 8) = uVar13;
          *(void **)(lVar5 + uVar11 * 8) = __ptr;
          if (lVar10 <= (long)uVar13) {
            lVar10 = uVar13 + 1;
            if ((long)uVar13 < lVar9) {
              lVar10 = lVar9;
            }
            local_170 = (void *)KBTZIL_HXSPAIPP(uVar12,lVar10 * 8);
          }
          pvVar1 = (void *)((long)local_170 + lVar15);
          lVar10 = lVar10 + lVar3 + uVar11;
          sVar7 = fread(&local_d0,8,1,__stream);
          if (sVar7 != 1) {
            KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x167,"readFVectorNoNewVec:n",1,sVar7);
          }
          sVar7 = fread(local_170,8,local_d0 + 1,__stream);
          if (local_d0 + 1U != sVar7) {
            KOBOOLOXSPAIPP(0x10,"factorchunkutils.c",0x167,"readFVectorNoNewVec:v",local_d0 + 1U,
                           sVar7);
          }
          uVar13 = uVar13 - 1;
          __ptr = (void *)((long)__ptr + 8);
          lVar15 = lVar15 + -8;
          *(void **)(lVar6 + uVar11 * 8) = local_170;
          uVar11 = uVar11 + 1;
          local_170 = pvVar1;
        } while ((long)(lVar2 + uVar11) <= *(long *)(puVar8[8] + local_118 * 8));
      }
      local_118 = local_118 + 1;
      local_110 = local_110 + 1;
      local_108 = local_108 + 8;
    } while (local_118 <= lVar14);
  }
  _ILPBdy_QLOaERKHdFIBXSPAIPP(param_3,param_1);
  return puVar8;
}


