/* driver: KOBOOLOXSPAIPP addr=0097a3b0 size=386 */
#include "vsdlss_ref.h"


void KOBOOLOXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                   undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                   long param_9,undefined8 param_10,undefined8 param_11,undefined8 param_12,
                   undefined8 param_13,undefined8 param_14)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  char in_AL;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  FILE *__stream;
  FILE *__stream_00;
  char *pcVar8;
  pthread_t pVar9;
  long lVar10;
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
  pthread_mutex_lock((pthread_mutex_t *)&DAT_00e74f80);
  uVar4 = local_d8;
  uVar3 = local_e0;
  local_5f8 = &stack0x00000008;
  local_5fc = 0x30;
  local_600 = 0x18;
  local_5f0 = local_e8;
  param_9 = param_9 * 0x18;
  uVar1 = *(undefined8 *)(BJQqQyQF_XSPAIPP + param_9);
  uVar2 = *(undefined8 *)(BJQqQyQF_XSPAIPP + param_9 + 8);
  pcVar8 = *(char **)(BJQqQyQF_XSPAIPP + param_9 + 0x10);
  __stream = (FILE *)DBQdFIBnQOXSPAIPP(10);
  if (__stream == (FILE *)0x0) {
    __stream = (FILE *)LMBKdFIBYpBQROKXSPAIPP(10);
  }
  __stream_00 = (FILE *)DBQdFIBnQOXSPAIPP(9);
  if (__stream_00 == (FILE *)0x0) {
    __stream_00 = (FILE *)LMBKdFIBYpBQROKXSPAIPP(9);
  }
  sprintf((char *)local_618,"SPAIPP%s:",uVar1);
  vsprintf((char *)local_4e8,pcVar8,&local_600);
  puVar13 = local_618;
  do {
    puVar12 = puVar13;
    uVar5 = *puVar12 + 0xfefefeff & ~*puVar12;
    uVar6 = uVar5 & 0x80808080;
    puVar13 = puVar12 + 1;
  } while (uVar6 == 0);
  if ((uVar5 & 0x8080) == 0) {
    puVar13 = (uint *)((long)puVar12 + 6);
    uVar6 = uVar6 >> 0x10;
  }
  puVar12 = local_4e8;
  do {
    puVar11 = puVar12;
    uVar7 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar5 = uVar7 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar5 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar5 = uVar5 >> 0x10;
  }
  if ((long)((long)puVar13 +
            (long)((long)puVar12 +
                  (-(long)local_618 - (ulong)CARRY1((byte)uVar6,(byte)uVar6)) +
                  ((-6 - (ulong)CARRY1((byte)uVar5,(byte)uVar5)) - (long)local_4e8))) < 0x101) {
    sprintf(SPAIPPkBPPyDBXSPAIPP,"%s%s",local_618,local_4e8);
  }
  else {
    pcVar8 = "Formatted message is too long. Printing a truncated message.";
    pcVar14 = local_5e8;
    for (lVar10 = 0x3d; lVar10 != 0; lVar10 = lVar10 + -1) {
      *pcVar14 = *pcVar8;
      pcVar8 = pcVar8 + (ulong)bVar15 * -2 + 1;
      pcVar14 = pcVar14 + (ulong)bVar15 * -2 + 1;
    }
    _TayPFayPJPDXSPAIPP(local_5e8);
    strcpy(SPAIPPkBPPyDBXSPAIPP,(char *)local_618);
    lVar10 = -1;
    puVar13 = local_618;
    do {
      if (lVar10 == 0) break;
      lVar10 = lVar10 + -1;
      uVar6 = *puVar13;
      puVar13 = (uint *)((long)puVar13 + (ulong)bVar15 * -2 + 1);
    } while ((char)uVar6 != '\0');
    strncat(SPAIPPkBPPyDBXSPAIPP,(char *)local_4e8,lVar10 + 0x101);
  }
  MOFKQcOOLOXSPAIPP(uVar2,uVar1,SPAIPPkBPPyDBXSPAIPP,uVar3,uVar4);
  if (__stream == (FILE *)0x0) {
    __stream = stderr;
  }
  local_620 = time((time_t *)0x0);
  pcVar8 = ctime(&local_620);
  fprintf(__stream,"Job:%s\tat:%s\n",GLzlyJBXSPAIPP,pcVar8);
  MOFKQcOOLOrLdFIBXSPAIPP(__stream,uVar2,uVar1,SPAIPPkBPPyDBXSPAIPP,uVar3,uVar4);
  if (__stream_00 != (FILE *)0x0) {
    local_620 = time((time_t *)0x0);
    pcVar8 = ctime(&local_620);
    fprintf(__stream_00,"Job:%s\tat:%s\n",GLzlyJBXSPAIPP,pcVar8);
    MOFKQcOOLOrLdFIBXSPAIPP(__stream_00,uVar2,uVar1,SPAIPPkBPPyDBXSPAIPP,uVar3,uVar4);
  }
  _yPFdaILPBXSPAIPP(10);
  _yPFdaILPBXSPAIPP(9);
  pVar9 = pthread_self();
  lVar10 = DBQkVrEOByAgKABUXSPAIPP(pVar9);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_00e74f80);
  if ((_yPFnyOqLISBOd != 0) && (0 < lVar10)) {
    *(undefined8 *)(SPAIPPrEOByAqQyQRP + lVar10 * 8) = uVar2;
                    /* WARNING: Subroutine does not return */
    longjmp((__jmp_buf_tag *)(SPAIPPrEOByAcKS + lVar10 * 200),(int)uVar2);
  }
  SPAIPPqQyQRPXSPAIPP = uVar2;
                    /* WARNING: Subroutine does not return */
  longjmp((__jmp_buf_tag *)SPAIPPcKSXSPAIPP,(int)uVar2);
}


