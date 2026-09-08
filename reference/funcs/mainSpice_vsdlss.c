/* VSDLSS function (decompiled by Ghidra) */
/* name: mainSpice_vsdlss  addr: 009d6820  size: d9b */
#include "vsdlss_ref.h"

/* signature: undefined4 mainSpice_vsdlss(char *param_1,int param_2,int *param_3); */

undefined4 mainSpice_vsdlss(char *param_1,int param_2,int *param_3)

{
  int iVar1;
  char cVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  size_t sVar7;
  void *pvVar8;
  FILE *pFVar9;
  char *__filename;
  uint *puVar10;
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
  uint *puVar11;
  uint *puVar12;
  undefined4 *puVar13;
  size_t __size;
  uint local_458;
  uint local_454;
  uint local_450;
  uint local_44c;
  uint local_448;
  int local_444;
  undefined8 local_440;
  undefined1 local_438 [1032];
  
  iVar1 = *param_3;
  local_454 = 0;
  local_450 = 0;
  local_44c = 0;
  local_440 = DAT_00abc248;
  local_448 = 0;
  local_444 = 0;
  sVar7 = strlen(param_1);
  pvVar8 = tc_malloc(sVar7 + 5);
  memcpy(pvVar8,param_1,sVar7);
  *(undefined4 *)(sVar7 + (long)pvVar8) = 0x7364682e;
  *(undefined1 *)((undefined4 *)(sVar7 + (long)pvVar8) + 1) = 0;
  pFVar9 = (FILE *)openFileMainRT_vsdlss(pvVar8);
  while (cVar2 = getALineWithFirstCharInSet_vsdlss(pFVar9,"jnrivtckxgJNRIVTCKXG",local_438),
        cVar2 == '\0') {
    switch(local_438[0]) {
    case 0x43:
    case 99:
      __isoc99_sscanf(local_438,"%*s %d",&local_454);
      break;
    default:
                    /* WARNING: Subroutine does not return */
      fatalError_vsdlss(9999,"main.c",0xa13,"mainSpice:1:This case is erroneous");
    case 0x47:
    case 0x67:
      __isoc99_sscanf(local_438,"%*s %d",&local_444);
      break;
    case 0x49:
    case 0x69:
      __isoc99_sscanf(local_438,"%*s %d",&DAT_00e3bd38);
      break;
    case 0x4a:
    case 0x6a:
      __isoc99_sscanf(local_438,"%*s %*s");
      break;
    case 0x4b:
    case 0x6b:
      __isoc99_sscanf(local_438,"%*s %d",&local_450);
      break;
    case 0x4e:
    case 0x6e:
      __isoc99_sscanf(local_438,"%*s %d",&DAT_00e3bd40);
      break;
    case 0x52:
    case 0x72:
      __isoc99_sscanf(local_438,"%*s %d",&DAT_00e3bd3c);
      break;
    case 0x54:
    case 0x74:
      __isoc99_sscanf(local_438,"%*s %lg",&local_440);
      break;
    case 0x56:
    case 0x76:
      __isoc99_sscanf(local_438,"%*s %d",&DAT_00e3bd34);
      break;
    case 0x58:
    case 0x78:
      __isoc99_sscanf(local_438,"%*s %d",&local_44c);
    }
  }
  fclose(pFVar9);
  tc_cfree(pvVar8);
  if (DAT_00e3bd40 < 1) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0xa1f,"Null circuit. Number of nodes is 0 or not provided\n");
  }
  if (DAT_00e3bd3c < 1) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0xa24,"Circuit does not have any resistors\n");
  }
  if (DAT_00e3bd38 < 1) {
    if (DAT_00e3bd34 < 1) {
                    /* WARNING: Subroutine does not return */
      fatalError_vsdlss(9999,"main.c",0xa29,"Circuit does not have a forcing function\n");
    }
    if (DAT_00e3bd38 == 0) goto LAB_009d6ad8;
    fwrite("Assuming total number of current sources to be 0\n",1,0x31,stderr);
    DAT_00e3bd38 = 0;
  }
  if (DAT_00e3bd34 < 0) {
    fwrite("Assuming total number of voltage sources to be 0\n",1,0x31,stderr);
    DAT_00e3bd34 = 0;
  }
LAB_009d6ad8:
  if (local_444 < 0) {
    fwrite("Assuming total number of voltage controlled current sources to be 0\n",1,0x44,stderr);
    local_444 = 0;
  }
  if (100000 < DAT_00e3bd40) {
    iVar1 = 0;
  }
  strcpy((char *)&DAT_00e96460,param_1);
  puVar12 = &DAT_00e96460;
  do {
    puVar10 = puVar12;
    uVar3 = *puVar10 + 0xfefefeff & ~*puVar10;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar10 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar10 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *puVar13 = 0x6370732e;
  *(undefined1 *)(puVar13 + 1) = 0;
  openNetlistFile_vsdlss();
  fclose(DAT_00e96448);
  sVar7 = strlen(param_1);
  __size = (size_t)((int)sVar7 + 0x11);
  __filename = tc_malloc(__size);
  puVar10 = tc_malloc(__size);
  __dest = tc_malloc(__size);
  __dest_00 = tc_malloc(__size);
  __dest_01 = tc_malloc(__size);
  __dest_02 = tc_malloc(__size);
  __dest_03 = tc_malloc(__size);
  __dest_04 = tc_malloc(__size);
  __dest_05 = tc_malloc(__size);
  __dest_06 = tc_malloc(__size);
  __dest_07 = tc_malloc(__size);
  __dest_08 = tc_malloc(__size);
  __dest_09 = tc_malloc(__size);
  __dest_10 = tc_malloc(__size);
  __dest_11 = tc_malloc(__size);
  memcpy(__filename,param_1,sVar7);
  builtin_strncpy(__filename + sVar7,".hdr",5);
  strcpy((char *)puVar10,param_1);
  puVar12 = puVar10;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *puVar13 = 0x74616d2e;
  *(undefined1 *)(puVar13 + 1) = 0;
  strcpy((char *)__dest,param_1);
  puVar12 = __dest;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *(undefined2 *)(puVar13 + 1) = 100;
  *puVar13 = 0x74616d2e;
  strcpy((char *)__dest_00,param_1);
  puVar12 = __dest_00;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *puVar13 = 0x74616d2e;
  *(undefined2 *)(puVar13 + 1) = 0x66;
  strcpy((char *)__dest_01,param_1);
  puVar12 = __dest_01;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *puVar13 = 0x74616d2e;
  *(undefined2 *)(puVar13 + 1) = 0x74;
  strcpy((char *)__dest_02,param_1);
  puVar12 = __dest_02;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *puVar13 = 0x74616d2e;
  *(undefined2 *)(puVar13 + 1) = 0x6f;
  strcpy((char *)__dest_03,param_1);
  puVar12 = __dest_03;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *puVar13 = 0x7368722e;
  *(undefined1 *)(puVar13 + 1) = 0;
  strcpy((char *)__dest_04,param_1);
  puVar12 = __dest_04;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *puVar13 = 0x6c73722e;
  *(undefined1 *)(puVar13 + 1) = 0;
  strcpy((char *)__dest_05,param_1);
  puVar12 = __dest_05;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *puVar13 = 0x6e65722e;
  *(undefined1 *)(puVar13 + 1) = 0;
  strcpy((char *)__dest_06,param_1);
  puVar12 = __dest_06;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *puVar13 = 0x3231612e;
  *(undefined1 *)(puVar13 + 1) = 0;
  strcpy((char *)__dest_07,param_1);
  puVar12 = __dest_07;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *puVar13 = 0x3232612e;
  *(undefined1 *)(puVar13 + 1) = 0;
  strcpy((char *)__dest_08,param_1);
  puVar12 = __dest_08;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *puVar13 = 0x7368722e;
  *(undefined2 *)(puVar13 + 1) = 0x31;
  strcpy((char *)__dest_09,param_1);
  puVar12 = __dest_09;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *puVar13 = 0x7368722e;
  *(undefined2 *)(puVar13 + 1) = 0x32;
  strcpy((char *)__dest_10,param_1);
  puVar12 = __dest_10;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *(undefined2 *)(puVar13 + 1) = 0x31;
  *puVar13 = 0x6c73722e;
  strcpy((char *)__dest_11,param_1);
  puVar12 = __dest_11;
  do {
    puVar11 = puVar12;
    uVar3 = *puVar11 + 0xfefefeff & ~*puVar11;
    uVar4 = uVar3 & 0x80808080;
    puVar12 = puVar11 + 1;
  } while (uVar4 == 0);
  if ((uVar3 & 0x8080) == 0) {
    puVar12 = (uint *)((long)puVar11 + 6);
    uVar4 = uVar4 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar12 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
  *(undefined2 *)(puVar13 + 1) = 0x32;
  *puVar13 = 0x6c73722e;
  uVar4 = readSpiceAndWriteMatVecs_vsdlss
                    (iVar1,puVar10,__dest_03,__dest_05,__dest_06,__dest_07,__dest_08,__dest_09,
                     DAT_00e3bd40,DAT_00e3bd3c,DAT_00e3bd34,DAT_00e3bd38,local_444,&local_458,
                     &local_448);
  local_458 = (DAT_00e3bd40 - DAT_00e3bd34) - local_448;
  pFVar9 = (FILE *)openFileMainWT_vsdlss(__filename);
  fprintf(pFVar9,"%c %d\n",0x6e,(ulong)local_458);
  fprintf(pFVar9,"%c %d\n",0x6f,(ulong)uVar4);
  fprintf(pFVar9,"%c %d\n",0x6d,0);
  fprintf(pFVar9,"%c %g\n",DAT_00b18ef8,0x61);
  if ((iVar1 == 0) && (local_444 == 0)) {
    fprintf(pFVar9,"%c %d\n",0x72,1);
    fprintf(pFVar9,"%c %s\n",100,&DAT_00af460a);
    fprintf(pFVar9,"%c %d\n",99,(ulong)local_454);
    fprintf(pFVar9,"%c %d\n",0x6b,(ulong)local_450);
    fprintf(pFVar9,"%c %d\n",0x78,(ulong)local_44c);
  }
  else {
    fprintf(pFVar9,"%c %d\n",0x75,(ulong)local_448);
  }
  fclose(pFVar9);
  iVar5 = fileExistsMain_vsdlss(puVar10);
  if (iVar5 != 0) {
    splitMatFileMain_vsdlss(puVar10,__dest,__dest_00,__dest_01,__dest_02);
    remove((char *)puVar10);
  }
  uVar6 = 0;
  if ((param_2 != 4) && (local_448 == 0)) {
    uVar6 = mainMat_vsdlss(param_1,param_2,0);
    if (param_2 != 1) {
      pFVar9 = (FILE *)openFileMainR_vsdlss(__dest_04);
      pvVar8 = tc_malloc((long)(int)(local_458 + 2) << 3);
      sVar7 = fread((void *)((long)pvVar8 + 8),8,(long)(int)local_458,pFVar9);
      if (local_458 != (uint)sVar7) {
                    /* WARNING: Subroutine does not return */
        fatalError_vsdlss(9999,"main.c",0xaad,"Reading of result failed\n");
      }
      writeNodalVoltages_vsdlss(param_1,__dest_05,DAT_00e3bd40,pvVar8);
      tc_cfree(pvVar8);
    }
    remove(__filename);
    remove((char *)puVar10);
    remove((char *)__dest);
    remove((char *)__dest_00);
    remove((char *)__dest_01);
    remove((char *)__dest_02);
    if (local_444 == 0) {
      remove((char *)__dest_03);
      remove((char *)__dest_04);
    }
  }
  remove((char *)__dest_05);
  tc_cfree(__filename);
  tc_cfree(puVar10);
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
  *param_3 = iVar1;
  return uVar6;
}


