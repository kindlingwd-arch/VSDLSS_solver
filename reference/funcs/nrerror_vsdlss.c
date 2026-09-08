/* VSDLSS function (decompiled by Ghidra) */
/* name: nrerror_vsdlss  addr: 009e7c00  size: 382 */
#include "vsdlss_ref.h"

/* signature: void nrerror_vsdlss(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,int param_9,undefined8 param_10,ulong param_11,undefined8 param_12,undefined8 param_13,undefined8 param_14); */

void nrerror_vsdlss(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                   undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                   int param_9,undefined8 param_10,ulong param_11,undefined8 param_12,
                   undefined8 param_13,undefined8 param_14)

{
  int __val;
  undefined8 uVar1;
  undefined8 uVar2;
  ulong uVar3;
  char in_AL;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  long lVar8;
  FILE *__stream;
  FILE *__stream_00;
  char *pcVar9;
  pthread_t pVar10;
  uint *puVar11;
  uint *puVar12;
  uint *puVar13;
  char *pcVar14;
  byte bVar15;
  time_t local_620;
  uint local_618 [6];
  undefined4 local_600;
  undefined4 local_5fc;
  undefined1 *local_5f8;
  undefined1 *local_5f0;
  char local_5e8 [256];
  uint local_4e8 [256];
  undefined1 local_e8 [8];
  undefined8 local_e0;
  ulong local_d8;
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
  
  bVar15 = 0;
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
  local_e0 = param_10;
  local_d8 = param_11;
  local_d0 = param_12;
  local_c8 = param_13;
  local_c0 = param_14;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_00ead920);
  uVar3 = local_d8;
  uVar2 = local_e0;
  local_5f8 = &stack0x00000008;
  local_5fc = 0x30;
  local_600 = 0x18;
  local_5f0 = local_e8;
  lVar8 = (long)param_9 * 0x18;
  uVar1 = *(undefined8 *)(emtStatic_vsdlss + lVar8);
  __val = *(int *)(emtStatic_vsdlss + lVar8 + 8);
  pcVar9 = *(char **)(emtStatic_vsdlss + lVar8 + 0x10);
  __stream = (FILE *)getFilePtr_vsdlss(10);
  if (__stream == (FILE *)0x0) {
    __stream = (FILE *)openFileAReturn_vsdlss(10);
  }
  __stream_00 = (FILE *)getFilePtr_vsdlss(9);
  if (__stream_00 == (FILE *)0x0) {
    __stream_00 = (FILE *)openFileAReturn_vsdlss(9);
  }
  sprintf((char *)local_618,"vsdlss%s:",uVar1);
  vsprintf((char *)local_4e8,pcVar9,&local_600);
  puVar13 = local_618;
  do {
    puVar12 = puVar13;
    uVar4 = *puVar12 + 0xfefefeff & ~*puVar12;
    uVar5 = uVar4 & 0x80808080;
    puVar13 = puVar12 + 1;
  } while (uVar5 == 0);
  if ((uVar4 & 0x8080) == 0) {
    puVar13 = (uint *)((long)puVar12 + 6);
    uVar5 = uVar5 >> 0x10;
  }
  puVar12 = local_4e8;
  do {
    puVar11 = puVar12;
    uVar6 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar6 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar6 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  if ((int)((((int)puVar13 - (uint)CARRY1((byte)uVar5,(byte)uVar5)) - (int)local_618) +
           ((((int)puVar12 + -6) - (uint)CARRY1((byte)uVar4,(byte)uVar4)) - (int)local_4e8)) < 0x101
     ) {
    sprintf(vsdlssMessage_vsdlss,"%s%s",local_618,local_4e8);
  }
  else {
    pcVar9 = "Formatted message is too long. Printing a truncated message.";
    pcVar14 = local_5e8;
    for (lVar8 = 0x3d; lVar8 != 0; lVar8 = lVar8 + -1) {
      *pcVar14 = *pcVar9;
      pcVar9 = pcVar9 + (ulong)bVar15 * -2 + 1;
      pcVar14 = pcVar14 + (ulong)bVar15 * -2 + 1;
    }
    cwCasiCasmsg_vsdlss(local_5e8);
    strcpy(vsdlssMessage_vsdlss,(char *)local_618);
    lVar8 = -1;
    puVar13 = local_618;
    do {
      if (lVar8 == 0) break;
      lVar8 = lVar8 + -1;
      uVar5 = *puVar13;
      puVar13 = (uint *)((long)puVar13 + (ulong)bVar15 * -2 + 1);
    } while ((char)uVar5 != '\0');
    strncat(vsdlssMessage_vsdlss,(char *)local_4e8,lVar8 + 0x101);
  }
  printError_vsdlss(__val,uVar1,vsdlssMessage_vsdlss,uVar2,uVar3 & 0xffffffff);
  if (__stream == (FILE *)0x0) {
    __stream = stderr;
  }
  local_620 = time((time_t *)0x0);
  pcVar9 = ctime(&local_620);
  fprintf(__stream,"Job:%s\tat:%s\n",jobName_vsdlss,pcVar9);
  printErrorToFile_vsdlss(__stream,__val,uVar1,vsdlssMessage_vsdlss,uVar2,uVar3 & 0xffffffff);
  if (__stream_00 != (FILE *)0x0) {
    local_620 = time((time_t *)0x0);
    pcVar9 = ctime(&local_620);
    fprintf(__stream_00,"Job:%s\tat:%s\n",jobName_vsdlss,pcVar9);
    printErrorToFile_vsdlss(__stream_00,__val,uVar1,vsdlssMessage_vsdlss,uVar2,uVar3 & 0xffffffff);
  }
  casiFClose_vsdlss(10);
  casiFClose_vsdlss(9);
  pVar10 = pthread_self();
  iVar7 = getMyThreadIndex_vsdlss(pVar10);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_00ead920);
  if ((casiParSolverF != 0) && (0 < iVar7)) {
    *(int *)(vsdlssThreadStatus + (long)iVar7 * 4) = __val;
                    /* WARNING: Subroutine does not return */
    longjmp((__jmp_buf_tag *)(vsdlssThreadEnv + (long)iVar7 * 200),__val);
  }
  vsdlssStatus_vsdlss = __val;
                    /* WARNING: Subroutine does not return */
  longjmp((__jmp_buf_tag *)vsdlssEnv_vsdlss,__val);
}


