/* VSDLSS function (decompiled by Ghidra) */
/* name: mainMat_vsdlss  addr: 009d4da0  size: 1a3e */
#include "vsdlss_ref.h"

/* signature: undefined8 mainMat_vsdlss(char *param_1,int param_2,int param_3); */

undefined8 mainMat_vsdlss(char *param_1,int param_2,int param_3)

{
  bool bVar1;
  byte bVar2;
  bool bVar3;
  bool bVar4;
  byte bVar5;
  char cVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  undefined2 **ppuVar10;
  FILE *pFVar11;
  undefined2 *__dest;
  void *__dest_00;
  FILE *pFVar12;
  FILE *__stream;
  FILE *__stream_00;
  uint *puVar13;
  uint *puVar14;
  long lVar15;
  uint *puVar16;
  undefined4 *puVar17;
  int iVar18;
  ulong uVar19;
  size_t sVar20;
  int iVar21;
  double dVar22;
  int local_5a80;
  undefined1 *local_5a70;
  byte local_5a51;
  uint local_5a50;
  int local_5a4c;
  int local_5a48;
  uint local_5a44;
  int local_5a40;
  uint local_5a3c;
  int local_5a38;
  uint local_5a34;
  uint local_5a30;
  int local_5a2c;
  int local_5a28;
  int local_5a24;
  double local_5a20;
  undefined2 local_5a18;
  undefined1 local_5a16;
  undefined1 local_59e8 [1024];
  uint local_55e8 [255];
  char cStack_51e9;
  undefined4 local_51e8;
  uint local_4de8 [255];
  undefined1 auStack_49eb [1027];
  uint local_45e8 [256];
  uint local_41e8 [256];
  undefined8 local_3de8 [128];
  undefined2 *local_39e8 [129];
  undefined2 *local_35e0;
  uint local_35d8 [264];
  uint local_31b8 [264];
  uint local_2d98 [264];
  uint local_2978 [264];
  uint local_2558 [264];
  uint local_2138 [264];
  uint local_1d18 [264];
  uint local_18f8 [264];
  uint local_14d8 [264];
  uint local_10b8 [264];
  uint local_c98 [264];
  uint local_878 [264];
  uint local_458 [266];
  
  local_5a48 = 0;
  local_5a20 = DAT_00b18ef8;
  dVar22 = (double)enqMemForVsdlss_vsdlss();
  ppuVar10 = local_39e8;
  local_5a44 = 0;
  local_5a40 = 0;
  local_5a3c = 0;
  local_5a38 = 1;
  local_5a34 = 0;
  local_5a30 = 0;
  local_5a2c = 0;
  local_5a28 = 1;
  local_5a24 = 0;
  do {
    *ppuVar10 = (undefined2 *)0x0;
    ppuVar10 = ppuVar10 + 1;
  } while (ppuVar10 != &local_35e0);
  strcpy((char *)local_35d8,param_1);
  puVar16 = local_35d8;
  do {
    puVar14 = puVar16;
    uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar8 = uVar7 & 0x80808080;
    puVar16 = puVar14 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar16 = (uint *)((long)puVar14 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar16 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x7264682e;
  *(undefined1 *)(puVar17 + 1) = 0;
  pFVar11 = (FILE *)openFileMainRT_vsdlss(local_35d8);
  local_5a51 = 0x46;
  cVar6 = getALineWithFirstCharInSet_vsdlss(pFVar11,"nomardcpslkxuzhNOMARDCPSLKXUZH",local_59e8);
  if (cVar6 == '\0') {
    local_5a80 = 0;
    bVar3 = false;
    puVar16 = (uint *)0x0;
    bVar5 = 0;
    bVar4 = false;
    iVar21 = 0;
    bVar1 = false;
    do {
      switch(local_59e8[0]) {
      case 0x41:
      case 0x61:
        __isoc99_sscanf(local_59e8,"%*s %lg",&local_5a20);
        if (local_5a20 <= 0.0) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x774,
                            "Available Memory %g(record a in .hdr file) should be positive\n");
        }
        break;
      default:
                    /* WARNING: Subroutine does not return */
        fatalError_vsdlss(9999,"main.c",0x806,"mainMat:1:This case is erroneous\n");
      case 0x43:
      case 99:
        __isoc99_sscanf(local_59e8,"%*s %d",&local_5a44);
        if (1 < local_5a44) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x798,
                            "Out-of-core Flag %d(record c in .hdr file) is out of range\n");
        }
        break;
      case 0x44:
      case 100:
        iVar21 = iVar21 + 1;
        if (0x80 < iVar21) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x789,"Number of directories exceeds %d allowed\n",0x80);
        }
        __isoc99_sscanf(local_59e8,"%*s %s",local_55e8);
        puVar14 = local_55e8;
        do {
          puVar13 = puVar14;
          uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
          uVar8 = uVar7 & 0x80808080;
          puVar14 = puVar13 + 1;
        } while (uVar8 == 0);
        if ((uVar7 & 0x8080) == 0) {
          puVar14 = (uint *)((long)puVar13 + 6);
          uVar8 = uVar8 >> 0x10;
        }
        lVar15 = (long)puVar14 + ((-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)) - (long)local_55e8);
        __dest = tc_malloc((long)((int)lVar15 + 1));
        local_39e8[iVar21] = __dest;
        memcpy(__dest,local_55e8,lVar15 + 1);
        bVar3 = true;
        break;
      case 0x48:
      case 0x68:
        __isoc99_sscanf(local_59e8,"%*s %d",&local_5a24);
        if ((local_5a28 < 0) || (1 < local_5a24)) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x7fa,
                            "hmslssReorderOnlyF %d(record h in .hdr file) is out of range\n",
                            local_5a24);
        }
        local_5a80 = 1;
        break;
      case 0x4b:
      case 0x6b:
        __isoc99_sscanf(local_59e8,"%*s %d",&local_5a34);
        if (4 < local_5a34) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x7cd,
                            "Solve-In-Memory Flag %d(record k in .hdr file) is out of range\n");
        }
        break;
      case 0x4c:
      case 0x6c:
        puVar16 = &local_51e8;
        __isoc99_sscanf(local_59e8,"%*s %s",puVar16);
        puVar14 = puVar16;
        do {
          puVar13 = puVar14;
          uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
          uVar8 = uVar7 & 0x80808080;
          puVar14 = puVar13 + 1;
        } while (uVar8 == 0);
        if ((uVar7 & 0x8080) == 0) {
          puVar14 = (uint *)((long)puVar13 + 6);
          uVar8 = uVar8 >> 0x10;
        }
        lVar15 = (long)(int)((((int)puVar14 + -3) - (uint)CARRY1((byte)uVar8,(byte)uVar8)) -
                            (int)puVar16);
        if ((&cStack_51e9)[lVar15] != '/') {
          *(undefined1 *)((long)&local_51e8 + lVar15 + 1) = *(undefined1 *)((long)puVar16 + lVar15);
          *(undefined1 *)((long)puVar16 + lVar15) = 0x2f;
        }
        break;
      case 0x4d:
      case 0x6d:
        __isoc99_sscanf(local_59e8,"%*s %d",&local_5a48);
        if (4 < local_5a48) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x767,
                            "Multiple Load Case Flag %d(record m in .hdr file) is out of range\n");
        }
        if (local_5a48 < 0) {
          local_5a48 = -1;
        }
        break;
      case 0x4e:
      case 0x6e:
        __isoc99_sscanf(local_59e8,"%*s %d",&local_5a50);
        if ((int)local_5a50 < 1) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x753,
                            "Number of nodes %d(record n in .hdr file) must be positive\n");
        }
        break;
      case 0x4f:
      case 0x6f:
        __isoc99_sscanf(local_59e8,"%*s %d",&local_5a4c);
        if (local_5a4c < 0) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x75d,
                            "Number of offdiagonals %d(record o in .hdr file) cannot be negative\n")
          ;
        }
        break;
      case 0x50:
      case 0x70:
        iVar9 = __isoc99_sscanf(local_59e8,"%*s %d %d",&local_5a40,&local_5a3c);
        bVar1 = true;
        if (iVar9 != 2) {
          local_5a3c = 0;
        }
        break;
      case 0x52:
      case 0x72:
        __isoc99_sscanf(local_59e8,"%*s %d",&local_5a38);
        if (local_5a38 < 0) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x77e,
                            "Reduce Problem Size Flag %d(record r in .hdr file) is out of range\n");
        }
        break;
      case 0x53:
      case 0x73:
        __isoc99_sscanf(local_59e8,"%*s %c",&local_5a51);
        if ((local_5a51 & 0xdf) == 0x4c) {
          local_5a51 = 0x4c;
          bVar4 = true;
        }
        else {
          if ((local_5a51 & 0xdf) != 0x55) {
                    /* WARNING: Subroutine does not return */
            fatalError_vsdlss(9999,"main.c",0x7b5,
                              "Record s value %c should be \'l\' or \'L\' (Lower Solve), \'u\' or \'U\' (Upper Solve)\n"
                             );
          }
          local_5a51 = 0x55;
          bVar4 = true;
        }
        break;
      case 0x55:
      case 0x75:
        __isoc99_sscanf(local_59e8,"%*s %d",&local_5a2c);
        if (param_3 != 0) {
          if (local_5a2c < 1) {
                    /* WARNING: Subroutine does not return */
            fatalError_vsdlss(9999,"main.c",0x7e5,
                              "nUnsym %d(record u in .hdr file) is out of range\n");
          }
          bVar5 = 1;
        }
        break;
      case 0x58:
      case 0x78:
        __isoc99_sscanf(local_59e8,"%*s %d",&local_5a30);
        if (1 < local_5a30) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x7d7,
                            "Order Selector %d(record x in .hdr file) is out of range\n");
        }
        break;
      case 0x5a:
      case 0x7a:
        __isoc99_sscanf(local_59e8,"%*s %d",&local_5a28);
        if (0x3f < local_5a28 - 1U) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x7f0,
                            "numThreads Selector %d(record z in .hdr file) is out of range\n");
        }
      }
      cVar6 = getALineWithFirstCharInSet_vsdlss(pFVar11,"nomardcpslkxuzhNOMARDCPSLKXUZH",local_59e8)
      ;
    } while (cVar6 == '\0');
    fclose(pFVar11);
    if (local_5a48 == -1) {
LAB_009d5da6:
      local_5a3c = 0;
      local_5a44 = 0;
      bVar2 = 0;
    }
    else {
      if (bVar4) {
        local_5a3c = 0;
        local_5a44 = 0;
        bVar1 = false;
      }
      else {
        bVar2 = 0;
        if (!bVar1) goto LAB_009d506c;
      }
      if ((local_5a40 < 1) || ((int)local_5a50 <= local_5a40)) {
                    /* WARNING: Subroutine does not return */
        fatalError_vsdlss(9999,"main.c",0x820,
                          "WSize %d(record p in .hdr file) is not in range [%d,%d]\n",local_5a40,1,
                          local_5a50 - 1);
      }
      bVar2 = 0;
      if (bVar1) {
        if (1 < local_5a3c) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x82a,
                            "reducedSystemOnlyF %d(record p in .hdr file) is out of range\n");
        }
        if (local_5a48 != 4) {
          local_5a48 = 0;
        }
        local_5a44 = 0;
        local_5a38 = 0;
        bVar2 = 1;
      }
    }
  }
  else {
    puVar16 = (uint *)0x0;
    fclose(pFVar11);
    bVar5 = 0;
    bVar3 = false;
    local_5a80 = 0;
    if (local_5a48 == -1) {
      iVar21 = 0;
      goto LAB_009d5da6;
    }
    bVar2 = 0;
    iVar21 = 0;
  }
LAB_009d506c:
  if (local_5a20 < dVar22) {
    local_5a20 = dVar22;
  }
  if (!bVar3) {
    local_39e8[1] = &local_5a18;
    local_5a16 = 0;
    local_5a18 = 0x2f2e;
    iVar21 = 1;
  }
  getBaseJobName_vsdlss(param_1,local_4de8);
  puVar14 = puVar16;
  if (puVar16 == (uint *)0x0) {
    local_5a70 = (undefined1 *)0x0;
  }
  else {
    do {
      puVar13 = puVar14;
      uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
      uVar8 = uVar7 & 0x80808080;
      puVar14 = puVar13 + 1;
    } while (uVar8 == 0);
    if ((uVar7 & 0x8080) == 0) {
      puVar14 = (uint *)((long)puVar13 + 6);
      uVar8 = uVar8 >> 0x10;
    }
    sVar20 = (long)puVar14 + ((-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)) - (long)puVar16);
    puVar14 = local_4de8;
    do {
      puVar13 = puVar14;
      uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
      uVar8 = uVar7 & 0x80808080;
      puVar14 = puVar13 + 1;
    } while (uVar8 == 0);
    if ((uVar7 & 0x8080) == 0) {
      puVar14 = (uint *)((long)puVar13 + 6);
      uVar8 = uVar8 >> 0x10;
    }
    lVar15 = (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)) - (long)local_4de8;
    if (0x100 < (long)puVar14 + sVar20 + 6 + lVar15) {
                    /* WARNING: Subroutine does not return */
      fatalError_vsdlss(9999,"main.c",0x847,
                        "mainMat:Increase MAX_FILE_NAME (currently %d), recompile, and rerun...",
                        0x100);
    }
    memcpy(auStack_49eb + 3,puVar16,sVar20);
    memcpy(auStack_49eb + sVar20 + 3,local_4de8,(long)puVar14 + lVar15 + 1);
    local_5a70 = auStack_49eb + 3;
  }
  strcpy((char *)local_31b8,param_1);
  puVar14 = local_31b8;
  do {
    puVar13 = puVar14;
    uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
    uVar8 = uVar7 & 0x80808080;
    puVar14 = puVar13 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar14 = (uint *)((long)puVar13 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *(undefined2 *)(puVar17 + 1) = 0x74;
  *puVar17 = 0x6173722e;
  strcpy((char *)local_2d98,param_1);
  puVar14 = local_2d98;
  do {
    puVar13 = puVar14;
    uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
    uVar8 = uVar7 & 0x80808080;
    puVar14 = puVar13 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar14 = (uint *)((long)puVar13 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  builtin_strncpy((char *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8))),".rset",6)
  ;
  strcpy((char *)local_2978,param_1);
  puVar14 = local_2978;
  do {
    puVar13 = puVar14;
    uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
    uVar8 = uVar7 & 0x80808080;
    puVar14 = puVar13 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar14 = (uint *)((long)puVar13 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x74616d2e;
  *(undefined1 *)(puVar17 + 1) = 0;
  strcpy((char *)local_2558,param_1);
  puVar14 = local_2558;
  do {
    puVar13 = puVar14;
    uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
    uVar8 = uVar7 & 0x80808080;
    puVar14 = puVar13 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar14 = (uint *)((long)puVar13 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x74616d2e;
  *(undefined2 *)(puVar17 + 1) = 100;
  strcpy((char *)local_2138,param_1);
  puVar14 = local_2138;
  do {
    puVar13 = puVar14;
    uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
    uVar8 = uVar7 & 0x80808080;
    puVar14 = puVar13 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar14 = (uint *)((long)puVar13 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x74616d2e;
  *(undefined2 *)(puVar17 + 1) = 0x66;
  strcpy((char *)local_1d18,param_1);
  puVar14 = local_1d18;
  do {
    puVar13 = puVar14;
    uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
    uVar8 = uVar7 & 0x80808080;
    puVar14 = puVar13 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar14 = (uint *)((long)puVar13 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x74616d2e;
  *(undefined2 *)(puVar17 + 1) = 0x74;
  strcpy((char *)local_18f8,param_1);
  puVar14 = local_18f8;
  do {
    puVar13 = puVar14;
    uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
    uVar8 = uVar7 & 0x80808080;
    puVar14 = puVar13 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar14 = (uint *)((long)puVar13 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x74616d2e;
  *(undefined2 *)(puVar17 + 1) = 0x6f;
  strcpy((char *)local_14d8,param_1);
  puVar14 = local_14d8;
  do {
    puVar13 = puVar14;
    uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
    uVar8 = uVar7 & 0x80808080;
    puVar14 = puVar13 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar14 = (uint *)((long)puVar13 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x7368722e;
  *(undefined1 *)(puVar17 + 1) = 0;
  strcpy((char *)local_10b8,param_1);
  puVar14 = local_10b8;
  do {
    puVar13 = puVar14;
    uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
    uVar8 = uVar7 & 0x80808080;
    puVar14 = puVar13 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar14 = (uint *)((long)puVar13 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x6c73722e;
  *(undefined1 *)(puVar17 + 1) = 0;
  strcpy((char *)local_c98,param_1);
  puVar14 = local_c98;
  do {
    puVar13 = puVar14;
    uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
    uVar8 = uVar7 & 0x80808080;
    puVar14 = puVar13 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar14 = (uint *)((long)puVar13 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x3231612e;
  *(undefined1 *)(puVar17 + 1) = 0;
  strcpy((char *)local_878,param_1);
  puVar14 = local_878;
  do {
    puVar13 = puVar14;
    uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
    uVar8 = uVar7 & 0x80808080;
    puVar14 = puVar13 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar14 = (uint *)((long)puVar13 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x646e652e;
  *(undefined2 *)(puVar17 + 1) = 0x6c;
  strcpy((char *)local_458,param_1);
  puVar14 = local_458;
  do {
    puVar13 = puVar14;
    uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
    uVar8 = uVar7 & 0x80808080;
    puVar14 = puVar13 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar14 = (uint *)((long)puVar13 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x6e66752e;
  *(undefined2 *)(puVar17 + 1) = 0x73;
  fclose(DAT_00e96440);
  if (local_5a48 != -1) {
    if ((((bVar5 == 0 && local_5a80 == 0) && bVar2 == 0) || (local_5a3c != 0)) &&
       (iVar9 = fileExistsMain_vsdlss(local_14d8), iVar9 == 0)) {
                    /* WARNING: Subroutine does not return */
      fatalError_vsdlss(9999,"main.c",0x887,"An RHS is required but RHS file %s does not exist\n",
                        local_14d8);
    }
    if ((local_5a51 == 0x55) || (local_5a51 == 0x4c)) {
      strcpy((char *)local_45e8,param_1);
      puVar14 = local_45e8;
      do {
        puVar13 = puVar14;
        uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
        uVar8 = uVar7 & 0x80808080;
        puVar14 = puVar13 + 1;
      } while (uVar8 == 0);
      if ((uVar7 & 0x8080) == 0) {
        puVar14 = (uint *)((long)puVar13 + 6);
        uVar8 = uVar8 >> 0x10;
      }
      builtin_strncpy((char *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8))),
                      ".parf",6);
      strcpy((char *)local_41e8,param_1);
      puVar14 = local_41e8;
      do {
        puVar13 = puVar14;
        uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
        uVar8 = uVar7 & 0x80808080;
        puVar14 = puVar13 + 1;
      } while (uVar8 == 0);
      if ((uVar7 & 0x8080) == 0) {
        puVar14 = (uint *)((long)puVar13 + 6);
        uVar8 = uVar8 >> 0x10;
      }
      puVar17 = (undefined4 *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
      *puVar17 = 0x6f73692e;
      *(undefined2 *)(puVar17 + 1) = 0x6c;
      strcpy((char *)local_3de8,param_1);
      puVar14 = (uint *)local_3de8;
      do {
        puVar13 = puVar14;
        uVar7 = *puVar13 + 0xfefefeff & ~*puVar13;
        uVar8 = uVar7 & 0x80808080;
        puVar14 = puVar13 + 1;
      } while (uVar8 == 0);
      if ((uVar7 & 0x8080) == 0) {
        puVar14 = (uint *)((long)puVar13 + 6);
        uVar8 = uVar8 >> 0x10;
      }
      builtin_strncpy((char *)((long)puVar14 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8))),
                      ".portv",7);
      iVar9 = fileExistsMain_vsdlss(local_45e8);
      if (iVar9 == 0) {
                    /* WARNING: Subroutine does not return */
        fatalError_vsdlss(9999,"main.c",0x89c,
                          "Partial Factor File %s is required but does not exist\n",local_45e8);
      }
      if ((local_5a51 == 0x4c) && (iVar9 = fileExistsMain_vsdlss(local_14d8), iVar9 == 0)) {
                    /* WARNING: Subroutine does not return */
        fatalError_vsdlss(9999,"main.c",0x8a4,"An RHS is required but RHS file %s does not exist\n",
                          local_14d8);
      }
      if (local_5a51 == 0x55) {
        iVar9 = fileExistsMain_vsdlss(local_41e8);
        if (iVar9 == 0) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x8ad,"Required intermediate file %s does not exist\n",
                            local_41e8);
        }
        iVar9 = fileExistsMain_vsdlss(local_3de8);
        if (iVar9 == 0) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x8b3,"Required input file %s does not exist\n",local_3de8
                           );
        }
      }
    }
    if (((bVar2 & (bVar5 ^ 1)) != 0) && (iVar9 = fileExistsMain_vsdlss(local_2d98), iVar9 == 0)) {
      iVar9 = fileExistsMain_vsdlss(local_31b8);
      if (iVar9 == 0) {
                    /* WARNING: Subroutine does not return */
        fatalError_vsdlss(9999,"main.c",0x8be,"Required file <jobname>.rset does not exist\n",
                          local_2d98);
      }
      convertRsatToRset(local_5a40,local_31b8,local_2d98);
    }
    if ((((-1 < local_5a48) && (local_5a48 != 2)) && (local_5a80 == 0)) &&
       (((iVar9 = fileExistsMain_vsdlss(local_2558), iVar9 == 0 ||
         (iVar9 = fileExistsMain_vsdlss(local_2138), iVar9 == 0)) ||
        ((iVar9 = fileExistsMain_vsdlss(local_1d18), iVar9 == 0 ||
         (iVar9 = fileExistsMain_vsdlss(local_18f8), iVar9 == 0)))))) {
      splitMatFileMain_vsdlss(local_2978,local_2558,local_2138,local_1d18,local_18f8);
    }
  }
  if (bVar5 == 0) {
LAB_009d563d:
    if (param_2 == 5) {
      uVar19 = 1;
      sVar20 = strlen(param_1);
      __dest_00 = tc_malloc(sVar20 + 5);
      memcpy(__dest_00,param_1,sVar20);
      *(undefined4 *)(sVar20 + (long)__dest_00) = 0x7073622e;
      *(undefined1 *)((undefined4 *)(sVar20 + (long)__dest_00) + 1) = 0;
      pFVar11 = (FILE *)openFileMainWT_vsdlss(__dest_00);
      fprintf(pFVar11,"%s\n",param_1);
      fprintf(pFVar11,"%d real\n",(ulong)local_5a50);
      pFVar12 = (FILE *)openFileMainR_vsdlss(local_2558);
      if (0 < (int)local_5a50) {
        do {
          sVar20 = fread(local_3de8,8,1,pFVar12);
          if ((int)sVar20 != 1) {
                    /* WARNING: Subroutine does not return */
            fatalError_vsdlss(9999,"main.c",0x909,"fread matdFp failed: Reading %d but read %d\n",1,
                              sVar20 & 0xffffffff);
          }
          fprintf(pFVar11,"%d %d %g\n",local_3de8[0],uVar19,uVar19);
          uVar8 = (int)uVar19 + 1;
          uVar19 = (ulong)uVar8;
        } while ((int)uVar8 <= (int)local_5a50);
      }
      iVar9 = 1;
      fclose(pFVar12);
      pFVar12 = (FILE *)openFileMainR_vsdlss(local_2138);
      __stream = (FILE *)openFileMainR_vsdlss(local_1d18);
      __stream_00 = (FILE *)openFileMainR_vsdlss(local_18f8);
      if (0 < local_5a4c) {
        do {
          sVar20 = fread(local_45e8,4,1,pFVar12);
          if ((int)sVar20 != 1) {
                    /* WARNING: Subroutine does not return */
            fatalError_vsdlss(9999,"main.c",0x91a,"fread matfFp failed: Reading %d but read %d\n",1,
                              sVar20 & 0xffffffff);
          }
          sVar20 = fread(local_41e8,4,1,__stream);
          if ((int)sVar20 != 1) {
                    /* WARNING: Subroutine does not return */
            fatalError_vsdlss(9999,"main.c",0x922,"fread mattFp failed: Reading %d but read %d\n",1,
                              sVar20 & 0xffffffff);
          }
          sVar20 = fread(local_3de8,8,1,__stream_00);
          if ((int)sVar20 != 1) {
                    /* WARNING: Subroutine does not return */
            fatalError_vsdlss(9999,"main.c",0x92a,"fread matoFp failed: Reading %d but read %d\n",1,
                              sVar20 & 0xffffffff);
          }
          iVar9 = iVar9 + 1;
          fprintf(pFVar11,"%d %d %g\n",local_3de8[0],(ulong)local_45e8[0],(ulong)local_41e8[0]);
          fprintf(pFVar11,"%d %d %g\n",local_3de8[0],(ulong)local_41e8[0],(ulong)local_45e8[0]);
        } while (iVar9 <= local_5a4c);
      }
      iVar9 = 1;
      fwrite(&DAT_00b185a6,1,8,pFVar11);
      fclose(pFVar12);
      fclose(__stream);
      fclose(__stream_00);
      pFVar12 = (FILE *)openFileMainR_vsdlss(local_14d8);
      if (0 < (int)local_5a50) {
        do {
          sVar20 = fread(local_3de8,8,1,pFVar12);
          if ((int)sVar20 != 1) {
                    /* WARNING: Subroutine does not return */
            fatalError_vsdlss(9999,"main.c",0x93d,"fread rhsFp failed: Reading %d but read %d\n",1,
                              sVar20 & 0xffffffff);
          }
          iVar9 = iVar9 + 1;
          fprintf(pFVar11,"%g\n",local_3de8[0]);
        } while (iVar9 <= (int)local_5a50);
      }
      iVar9 = 0;
      fclose(pFVar11);
      tc_cfree(__dest_00);
    }
    else {
      iVar9 = vsdlss(local_5a20,param_1,local_5a50,local_5a4c,local_5a48,local_5a44,iVar21,
                     local_39e8,local_5a38,bVar2,local_5a40,(int)(char)local_5a51,puVar16,local_5a34
                     ,local_5a30,local_4de8,local_5a70,local_5a3c,local_2d98,anilMalloc_vsdlss,
                     anilFree_vsdlss,local_2558,local_2138,local_1d18,local_18f8,local_14d8,
                     local_10b8,local_c98,1,param_2,bVar5,local_5a2c,local_878,local_458,local_5a28,
                     local_5a80,local_5a24);
    }
  }
  else {
    if ((local_5a48 != 4) && (local_5a48 != -1)) {
      local_5a48 = 0;
    }
    local_5a44 = 0;
    local_5a38 = 1;
    local_5a51 = 0x46;
    local_5a30 = 0;
    local_5a3c = 0;
    if (param_2 != 4) goto LAB_009d563d;
    iVar9 = 0;
  }
  if ((bVar3) && (iVar21 != 0)) {
    ppuVar10 = local_39e8;
    iVar18 = 1;
    do {
      ppuVar10 = ppuVar10 + 1;
      iVar18 = iVar18 + 1;
      tc_cfree(*ppuVar10);
    } while (iVar18 <= iVar21);
  }
  DAT_00e96440 = (FILE *)openFileMainAT_vsdlss(&DAT_00e96340);
  if (iVar9 == 5) {
    fwrite("SQRT_OF_NEGATIVE_NUMBER\n",1,0x18,stderr);
    goto LAB_009d588d;
  }
  if (iVar9 < 6) {
    if (iVar9 == 2) {
      fwrite("NEGATIVE_OR_ZERO_PIVOT\n",1,0x17,stderr);
      goto LAB_009d588d;
    }
    if (iVar9 < 3) {
      if (iVar9 == 0) goto LAB_009d641f;
    }
    else {
      if (iVar9 == 3) {
        fwrite("NEGATIVE_DIAGONAL_OF_A\n",1,0x17,stderr);
        goto LAB_009d588d;
      }
      if (iVar9 == 4) {
        fwrite("DIVISION_BY_ZERO\n",1,0x11,stderr);
        goto LAB_009d588d;
      }
    }
  }
  else {
    if (iVar9 == 7) {
      fwrite("INPUT_DATA_INCONSISTENT\n",1,0x18,stderr);
      goto LAB_009d588d;
    }
    if (iVar9 < 7) {
      fwrite("INSUFFICIENT_MEMORY\n",1,0x14,stderr);
      goto LAB_009d588d;
    }
    if (iVar9 == 8) {
      fwrite("FILE_ERROR\n",1,0xb,stderr);
      goto LAB_009d588d;
    }
    if (iVar9 == 9999) {
      fwrite("vsdlss_FATAL\n",1,0xd,stderr);
      goto LAB_009d588d;
    }
  }
  fwrite("Unknown status.\n",1,0x10,stderr);
  if (iVar9 == 0) {
LAB_009d641f:
    if (DAT_00e96440 != (FILE *)0x0) {
      fwrite("Solver vsdlss returned successfully.\n",1,0x25,DAT_00e96440);
    }
    return 0;
  }
LAB_009d588d:
                    /* WARNING: Subroutine does not return */
  fatalError_vsdlss(iVar9,"main.c",0x9a7,"Solver vsdlss failed with status %d.\n",iVar9);
}


