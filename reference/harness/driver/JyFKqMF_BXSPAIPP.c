/* driver: JyFKqMF_BXSPAIPP addr=0095eff0 size=dd6 */
#include "vsdlss_ref.h"


undefined8 JyFKqMF_BXSPAIPP(char *param_1,long param_2,long *param_3)

{
  char cVar1;
  uint uVar2;
  uint uVar3;
  size_t sVar4;
  void *pvVar5;
  FILE *pFVar6;
  size_t __n;
  char *__filename;
  uint *puVar7;
  uint *__dest;
  uint *__dest_00;
  uint *__dest_01;
  uint *__dest_02;
  uint *__dest_03;
  uint *__dest_04;
  uint *__dest_05;
  uint *__dest_06;
  uint *__dest_07;
  uint *__dest_08;
  uint *__dest_09;
  uint *__dest_10;
  uint *__dest_11;
  undefined8 uVar8;
  long lVar9;
  uint *puVar10;
  uint *puVar11;
  undefined4 *puVar12;
  long lVar13;
  size_t local_470;
  undefined8 local_468;
  undefined8 local_460;
  undefined8 local_458;
  undefined8 local_450;
  long local_448;
  long local_440;
  undefined1 local_438 [1032];
  
  lVar13 = *param_3;
  local_460 = 0;
  local_458 = 0;
  local_450 = 0;
  local_468 = DAT_00abc248;
  local_448 = 0;
  local_440 = 0;
  sVar4 = strlen(param_1);
  pvVar5 = tc_malloc(sVar4 + 5);
  memcpy(pvVar5,param_1,sVar4);
  *(undefined4 *)(sVar4 + (long)pvVar5) = 0x7364682e;
  *(undefined1 *)((undefined4 *)(sVar4 + (long)pvVar5) + 1) = 0;
  pFVar6 = (FILE *)LMBKdFIBkyFKprXSPAIPP(pvVar5);
  while (cVar1 = DBQYjFKBuFQEdFOPQaEyOgKqBQXSPAIPP(pFVar6,"jnrivtckxgJNRIVTCKXG",local_438),
        cVar1 == '\0') {
    switch(local_438[0]) {
    case 0x43:
    case 99:
      __isoc99_sscanf(local_438,"%*s %ld",&local_460);
      break;
    default:
                    /* WARNING: Subroutine does not return */
      CyQyIcOOLOXSPAIPP(9999,"main.c",0xa13,"mainSpice:1:This case is erroneous");
    case 0x47:
    case 0x67:
      __isoc99_sscanf(local_438,"%*s %ld",&local_440);
      break;
    case 0x49:
    case 0x69:
      __isoc99_sscanf(local_438,"%*s %ld",&DAT_00e39ff0);
      break;
    case 0x4a:
    case 0x6a:
      __isoc99_sscanf(local_438,"%*s %*s");
      break;
    case 0x4b:
    case 0x6b:
      __isoc99_sscanf(local_438,"%*s %ld",&local_458);
      break;
    case 0x4e:
    case 0x6e:
      __isoc99_sscanf(local_438,"%*s %ld",&DAT_00e3a000);
      break;
    case 0x52:
    case 0x72:
      __isoc99_sscanf(local_438,"%*s %ld",&DAT_00e39ff8);
      break;
    case 0x54:
    case 0x74:
      __isoc99_sscanf(local_438,"%*s %lg",&local_468);
      break;
    case 0x56:
    case 0x76:
      __isoc99_sscanf(local_438,"%*s %ld",&DAT_00e39fe8);
      break;
    case 0x58:
    case 0x78:
      __isoc99_sscanf(local_438,"%*s %ld",&local_450);
    }
  }
  fclose(pFVar6);
  tc_cfree(pvVar5);
  if (DAT_00e3a000 < 1) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0xa1f,"Null circuit. Number of nodes is 0 or not provided\n");
  }
  if (DAT_00e39ff8 < 1) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0xa24,"Circuit does not have any resistors\n");
  }
  if (DAT_00e39ff0 < 1) {
    if (DAT_00e39fe8 < 1) {
                    /* WARNING: Subroutine does not return */
      CyQyIcOOLOXSPAIPP(9999,"main.c",0xa29,"Circuit does not have a forcing function\n");
    }
    if (DAT_00e39ff0 == 0) goto LAB_0095f2bc;
    fwrite("Assuming total number of current sources to be 0\n",1,0x31,stderr);
    DAT_00e39ff0 = 0;
  }
  if (DAT_00e39fe8 < 0) {
    fwrite("Assuming total number of voltage sources to be 0\n",1,0x31,stderr);
    DAT_00e39fe8 = 0;
  }
LAB_0095f2bc:
  if (local_440 < 0) {
    fwrite("Assuming total number of voltage controlled current sources to be 0\n",1,0x44,stderr);
    local_440 = 0;
  }
  if (100000 < DAT_00e3a000) {
    lVar13 = 0;
  }
  strcpy((char *)&DAT_00e56360,param_1);
  puVar11 = &DAT_00e56360;
  do {
    puVar7 = puVar11;
    uVar2 = *puVar7 + 0xfefefeff & ~*puVar7;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar7 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar7 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar12 = 0x6370732e;
  *(undefined1 *)(puVar12 + 1) = 0;
  LMBKlBQIFPQdFIBXSPAIPP();
  fclose(DAT_00e56348);
  __n = strlen(param_1);
  sVar4 = __n + 0x11;
  __filename = tc_malloc(sVar4);
  puVar7 = tc_malloc(sVar4);
  __dest = tc_malloc(sVar4);
  __dest_00 = tc_malloc(sVar4);
  __dest_01 = tc_malloc(sVar4);
  __dest_02 = tc_malloc(sVar4);
  __dest_03 = tc_malloc(sVar4);
  __dest_04 = tc_malloc(sVar4);
  __dest_05 = tc_malloc(sVar4);
  __dest_06 = tc_malloc(sVar4);
  __dest_07 = tc_malloc(sVar4);
  __dest_08 = tc_malloc(sVar4);
  __dest_09 = tc_malloc(sVar4);
  __dest_10 = tc_malloc(sVar4);
  __dest_11 = tc_malloc(sVar4);
  memcpy(__filename,param_1,__n);
  builtin_strncpy(__filename + __n,".hdr",5);
  strcpy((char *)puVar7,param_1);
  puVar11 = puVar7;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar12 = 0x74616d2e;
  *(undefined1 *)(puVar12 + 1) = 0;
  strcpy((char *)__dest,param_1);
  puVar11 = __dest;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar12 = 0x74616d2e;
  *(undefined2 *)(puVar12 + 1) = 100;
  strcpy((char *)__dest_00,param_1);
  puVar11 = __dest_00;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar12 = 0x74616d2e;
  *(undefined2 *)(puVar12 + 1) = 0x66;
  strcpy((char *)__dest_01,param_1);
  puVar11 = __dest_01;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar12 = 0x74616d2e;
  *(undefined2 *)(puVar12 + 1) = 0x74;
  strcpy((char *)__dest_02,param_1);
  puVar11 = __dest_02;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar12 = 0x74616d2e;
  *(undefined2 *)(puVar12 + 1) = 0x6f;
  strcpy((char *)__dest_03,param_1);
  puVar11 = __dest_03;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar12 = 0x7368722e;
  *(undefined1 *)(puVar12 + 1) = 0;
  strcpy((char *)__dest_04,param_1);
  puVar11 = __dest_04;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar12 = 0x6c73722e;
  *(undefined1 *)(puVar12 + 1) = 0;
  strcpy((char *)__dest_05,param_1);
  puVar11 = __dest_05;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar12 = 0x6e65722e;
  *(undefined1 *)(puVar12 + 1) = 0;
  strcpy((char *)__dest_06,param_1);
  puVar11 = __dest_06;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar12 = 0x3231612e;
  *(undefined1 *)(puVar12 + 1) = 0;
  strcpy((char *)__dest_07,param_1);
  puVar11 = __dest_07;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar12 = 0x3232612e;
  *(undefined1 *)(puVar12 + 1) = 0;
  strcpy((char *)__dest_08,param_1);
  puVar11 = __dest_08;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *(undefined2 *)(puVar12 + 1) = 0x31;
  *puVar12 = 0x7368722e;
  strcpy((char *)__dest_09,param_1);
  puVar11 = __dest_09;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *(undefined2 *)(puVar12 + 1) = 0x32;
  *puVar12 = 0x7368722e;
  strcpy((char *)__dest_10,param_1);
  puVar11 = __dest_10;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar12 = 0x6c73722e;
  *(undefined2 *)(puVar12 + 1) = 0x31;
  strcpy((char *)__dest_11,param_1);
  puVar11 = __dest_11;
  do {
    puVar10 = puVar11;
    uVar2 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar3 = uVar2 & 0x80808080;
    puVar11 = puVar10 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar11 = (uint *)((long)puVar10 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar12 = (undefined4 *)((long)puVar11 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *(undefined2 *)(puVar12 + 1) = 0x32;
  *puVar12 = 0x6c73722e;
  uVar8 = OByAqMF_BYKAuOFQBkyQtB_PXSPAIPP
                    (lVar13,puVar7,__dest_03,__dest_05,__dest_06,__dest_07,__dest_08,__dest_09,
                     DAT_00e3a000,DAT_00e39ff8,DAT_00e39fe8,DAT_00e39ff0,local_440,&local_470,
                     &local_448);
  local_470 = (DAT_00e3a000 - DAT_00e39fe8) - local_448;
  pFVar6 = (FILE *)LMBKdFIBkyFKurXSPAIPP(__filename);
  fprintf(pFVar6,"%c %ld\n",0x6e,local_470);
  fprintf(pFVar6,"%c %ld\n",0x6f,uVar8);
  fprintf(pFVar6,"%c %ld\n",0x6d,0);
  fprintf(pFVar6,"%c %g\n",DAT_00b18ef8,0x61);
  if ((lVar13 == 0) && (local_440 == 0)) {
    fprintf(pFVar6,"%c %ld\n",0x72,1);
    fprintf(pFVar6,"%c %s\n",100,&DAT_00af460a);
    fprintf(pFVar6,"%c %ld\n",99,local_460);
    fprintf(pFVar6,"%c %ld\n",0x6b,local_458);
    fprintf(pFVar6,"%c %ld\n",0x78,local_450);
  }
  else {
    fprintf(pFVar6,"%c %ld\n",0x75,local_448);
  }
  fclose(pFVar6);
  lVar9 = CFIBcUFPQPkyFKXSPAIPP(puVar7);
  if (lVar9 != 0) {
    PMIFQkyQdFIBkyFKXSPAIPP(puVar7,__dest,__dest_00,__dest_01,__dest_02);
    remove((char *)puVar7);
  }
  uVar8 = 0;
  if ((param_2 != 4) && (local_448 == 0)) {
    uVar8 = JyFKkyQXSPAIPP(param_1,param_2,0);
    if (param_2 != 1) {
      pFVar6 = (FILE *)LMBKdFIBkyFKpXSPAIPP(__dest_04);
      sVar4 = local_470;
      pvVar5 = tc_malloc(local_470 * 8 + 0x10);
      sVar4 = fread((void *)((long)pvVar5 + 8),8,sVar4,pFVar6);
      if (sVar4 != local_470) {
                    /* WARNING: Subroutine does not return */
        CyQyIcOOLOXSPAIPP(9999,"main.c",0xaad,"Reading of result failed\n");
      }
      TOFQBlLAyItLIQyDBPXSPAIPP(param_1,__dest_05,DAT_00e3a000,pvVar5);
      tc_cfree(pvVar5);
    }
    remove(__filename);
    remove((char *)puVar7);
    remove((char *)__dest);
    remove((char *)__dest_00);
    remove((char *)__dest_01);
    remove((char *)__dest_02);
    if (local_440 == 0) {
      remove((char *)__dest_03);
      remove((char *)__dest_04);
    }
  }
  remove((char *)__dest_05);
  tc_cfree(__filename);
  tc_cfree(puVar7);
  tc_cfree(__dest);
  tc_cfree(__dest_00);
  tc_cfree(__dest_01);
  tc_cfree(__dest_02);
  tc_cfree(__dest_03);
  tc_cfree(__dest_04);
  tc_cfree(__dest_05);
  tc_cfree(__dest_06);
  tc_cfree(__dest_07);
  tc_cfree(__dest_08);
  tc_cfree(__dest_09);
  tc_cfree(__dest_10);
  tc_cfree(__dest_11);
  *param_3 = lVar13;
  return uVar8;
}


