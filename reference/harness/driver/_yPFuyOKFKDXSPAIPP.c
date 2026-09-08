/* driver: _yPFuyOKFKDXSPAIPP addr=0097a740 size=3a9 */
#include "vsdlss_ref.h"


void _yPFuyOKFKDXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                       undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                       long param_9,undefined8 param_10,undefined8 param_11,undefined8 param_12,
                       undefined8 param_13,undefined8 param_14)

{
  undefined8 uVar1;
  undefined8 uVar2;
  char in_AL;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  FILE *__stream;
  FILE *__stream_00;
  char *pcVar6;
  uint *puVar7;
  uint *puVar8;
  uint *puVar9;
  time_t local_620;
  uint local_618 [6];
  undefined4 local_600;
  undefined4 local_5fc;
  undefined1 *local_5f8;
  undefined1 *local_5f0;
  undefined8 local_5e8;
  undefined8 local_5e0;
  undefined8 local_5d8;
  undefined8 local_5d0;
  undefined8 local_5c8;
  undefined8 local_5c0;
  undefined8 local_5b8;
  undefined4 local_5b0;
  undefined1 local_5ac;
  undefined1 auStack_519 [49];
  uint local_4e8 [256];
  undefined1 local_e8 [8];
  undefined8 local_e0;
  undefined8 local_d8;
  undefined8 local_d0;
  undefined8 local_c8;
  undefined8 local_c0;
  undefined8 local_b8;
  undefined8 local_a8;
  undefined8 local_98;
  undefined8 local_88;
  undefined8 local_78;
  undefined8 local_68;
  undefined8 local_58;
  undefined8 local_48;
  
  if (in_AL != '\0') {
    local_b8 = param_1;
    local_a8 = param_2;
    local_98 = param_3;
    local_88 = param_4;
    local_78 = param_5;
    local_68 = param_6;
    local_58 = param_7;
    local_48 = param_8;
  }
  local_5f8 = &stack0x00000008;
  local_5f0 = local_e8;
  local_5fc = 0x30;
  local_600 = 0x18;
  param_9 = param_9 * 0x18;
  uVar1 = *(undefined8 *)(BJQqQyQF_XSPAIPP + param_9 + 8);
  uVar2 = *(undefined8 *)(BJQqQyQF_XSPAIPP + param_9);
  pcVar6 = *(char **)(BJQqQyQF_XSPAIPP + param_9 + 0x10);
  local_e0 = param_10;
  local_d8 = param_11;
  local_d0 = param_12;
  local_c8 = param_13;
  local_c0 = param_14;
  __stream = (FILE *)DBQdFIBnQOXSPAIPP(10);
  if (__stream == (FILE *)0x0) {
    __stream = (FILE *)LMBKdFIBYXSPAIPP(10);
  }
  __stream_00 = (FILE *)DBQdFIBnQOXSPAIPP(9);
  if (__stream_00 == (FILE *)0x0) {
    __stream_00 = (FILE *)LMBKdFIBYXSPAIPP(9);
  }
  sprintf((char *)local_618,"SPAIPP%s:",uVar2);
  vsprintf((char *)local_4e8,pcVar6,&local_600);
  puVar9 = local_618;
  do {
    puVar8 = puVar9;
    uVar3 = *puVar8 + 0xfefefeff & ~*puVar8;
    uVar4 = uVar3 & 0x80808080;
    puVar9 = puVar8 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar9 = (uint *)((long)puVar8 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar8 = local_4e8;
  do {
    puVar7 = puVar8;
    uVar5 = *puVar7 + 0xfefefeff & ~*puVar7;
    uVar3 = uVar5 & 0x80808080;
    puVar8 = puVar7 + 1;
  } while (uVar3 == 0);
  if ((uVar5 & 0x8080) == 0) {
    puVar8 = (uint *)((long)puVar7 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  if ((long)((long)puVar9 +
            (long)((long)puVar8 +
                  (-(long)local_618 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)) +
                  ((-6 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)) - (long)local_4e8))) < 0x101) {
    sprintf(SPAIPPkBPPyDBXSPAIPP,"%s%s",local_618);
  }
  else {
    local_5e8 = 0x657474616d726f46;
    local_5b0 = 0x2e656761;
    local_5e0 = 0x67617373656d2064;
    local_5ac = 0;
    local_5d8 = 0x6f6f742073692065;
    local_5d0 = 0x50202e676e6f6c20;
    local_5c8 = 0x20676e69746e6972;
    local_5c0 = 0x61636e7572742061;
    local_5b8 = 0x7373656d20646574;
    _TayPFayPJPDXSPAIPP(&local_5e8);
    strcpy(SPAIPPkBPPyDBXSPAIPP,(char *)local_618);
    puVar9 = local_618;
    do {
      puVar8 = puVar9;
      uVar3 = *puVar8 + 0xfefefeff & ~*puVar8;
      uVar4 = uVar3 & 0x80808080;
      puVar9 = puVar8 + 1;
    } while (uVar4 == 0);
    if ((uVar3 & 0x8080) == 0) {
      puVar9 = (uint *)((long)puVar8 + 6);
      uVar4 = uVar4 >> 0x10;
    }
    strncat(SPAIPPkBPPyDBXSPAIPP,(char *)local_4e8,
            (size_t)(auStack_519 +
                    -(long)((long)puVar9 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)))));
  }
  MOFKQuyOKFKDXSPAIPP(uVar1,uVar2,SPAIPPkBPPyDBXSPAIPP,param_10,param_11);
  local_620 = time((time_t *)0x0);
  pcVar6 = ctime(&local_620);
  fprintf(__stream,"Job:%s\tat:%s\n",GLzlyJBXSPAIPP,pcVar6);
  MOFKQuyOKFKDrLdFIBXSPAIPP(__stream,uVar1,uVar2,SPAIPPkBPPyDBXSPAIPP,param_10,param_11);
  local_620 = time((time_t *)0x0);
  pcVar6 = ctime(&local_620);
  fprintf(__stream_00,"Job:%s\tat:%s\n",GLzlyJBXSPAIPP,pcVar6);
  MOFKQuyOKFKDrLdFIBXSPAIPP(__stream_00,uVar1,uVar2,SPAIPPkBPPyDBXSPAIPP,param_10,param_11);
  return;
}


