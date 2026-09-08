/* driver: JyFKkyQXSPAIPP addr=0095d520 size=1aa5 */
#include "vsdlss_ref.h"


undefined8 JyFKkyQXSPAIPP(char *param_1,long param_2,long param_3)

{
  bool bVar1;
  bool bVar2;
  byte bVar3;
  byte bVar4;
  bool bVar5;
  char cVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  undefined2 **ppuVar10;
  FILE *pFVar11;
  long lVar12;
  undefined2 *__dest;
  void *__dest_00;
  FILE *pFVar13;
  FILE *__stream;
  FILE *__stream_00;
  uint *puVar14;
  uint *puVar15;
  uint *puVar16;
  undefined4 *puVar17;
  long lVar18;
  size_t sVar19;
  long lVar20;
  double dVar21;
  long local_5ab8;
  undefined1 *local_5a98;
  byte local_5a81;
  long local_5a80;
  long local_5a78;
  long local_5a70;
  double local_5a68;
  ulong local_5a60;
  long local_5a58;
  ulong local_5a50;
  long local_5a48;
  ulong local_5a40;
  ulong local_5a38;
  long local_5a30;
  long local_5a28;
  long local_5a20;
  undefined2 local_5a18;
  undefined1 local_5a16;
  undefined1 local_59e8 [1024];
  uint local_55e8 [255];
  char acStack_51ec [4];
  uint local_51e8 [256];
  uint local_4de8 [255];
  undefined1 auStack_49eb [1027];
  undefined8 local_45e8 [128];
  undefined8 local_41e8 [128];
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
  
  local_5a70 = 0;
  local_5a68 = DAT_00b18ef8;
  dVar21 = (double)BKNkBJdLOtPAIPPXSPAIPP();
  ppuVar10 = local_39e8;
  local_5a60 = 0;
  local_5a58 = 0;
  local_5a50 = 0;
  local_5a48 = 1;
  local_5a40 = 0;
  local_5a38 = 0;
  local_5a30 = 0;
  local_5a28 = 1;
  local_5a20 = 0;
  do {
    *ppuVar10 = (undefined2 *)0x0;
    ppuVar10 = ppuVar10 + 1;
  } while (ppuVar10 != &local_35e0);
  strcpy((char *)local_35d8,param_1);
  puVar16 = local_35d8;
  do {
    puVar15 = puVar16;
    uVar7 = *puVar15 + 0xfefefeff & ~*puVar15;
    uVar8 = uVar7 & 0x80808080;
    puVar16 = puVar15 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar16 = (uint *)((long)puVar15 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar16 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x7264682e;
  *(undefined1 *)(puVar17 + 1) = 0;
  pFVar11 = (FILE *)LMBKdFIBkyFKprXSPAIPP(local_35d8);
  local_5a81 = 0x46;
  cVar6 = DBQYjFKBuFQEdFOPQaEyOgKqBQXSPAIPP(pFVar11,"nomardcpslkxuzhNOMARDCPSLKXUZH",local_59e8);
  if (cVar6 == '\0') {
    local_5ab8 = 0;
    bVar2 = false;
    puVar16 = (uint *)0x0;
    bVar3 = 0;
    bVar1 = false;
    lVar20 = 0;
    bVar5 = false;
    do {
      switch(local_59e8[0]) {
      case 0x41:
      case 0x61:
        __isoc99_sscanf(local_59e8,"%*s %lg",&local_5a68);
        if (local_5a68 <= 0.0) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x774,
                            "Available Memory %g(record a in .hdr file) should be positive\n");
        }
        break;
      default:
                    /* WARNING: Subroutine does not return */
        CyQyIcOOLOXSPAIPP(9999,"main.c",0x806,"mainMat:1:This case is erroneous\n");
      case 0x43:
      case 99:
        __isoc99_sscanf(local_59e8,"%*s %ld",&local_5a60);
        if (1 < local_5a60) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x798,
                            "Out-of-core Flag %ld(record c in .hdr file) is out of range\n");
        }
        break;
      case 0x44:
      case 100:
        lVar20 = lVar20 + 1;
        if (0x80 < lVar20) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x789,"Number of directories exceeds %ld allowed\n",0x80);
        }
        __isoc99_sscanf(local_59e8,"%*s %s",local_55e8);
        puVar15 = local_55e8;
        do {
          puVar14 = puVar15;
          uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
          uVar8 = uVar7 & 0x80808080;
          puVar15 = puVar14 + 1;
        } while (uVar8 == 0);
        if ((uVar7 & 0x8080) == 0) {
          puVar15 = (uint *)((long)puVar14 + 6);
          uVar8 = uVar8 >> 0x10;
        }
        sVar19 = (long)puVar15 + (-(long)local_55e8 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)) + -2;
        __dest = tc_malloc(sVar19);
        local_39e8[lVar20] = __dest;
        memcpy(__dest,local_55e8,sVar19);
        bVar2 = true;
        break;
      case 0x48:
      case 0x68:
        __isoc99_sscanf(local_59e8,"%*s %ld",&local_5a20);
        if ((local_5a28 < 0) || (1 < local_5a20)) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x7fa,
                            "hmslssReorderOnlyF %ld(record h in .hdr file) is out of range\n",
                            local_5a20);
        }
        local_5ab8 = 1;
        break;
      case 0x4b:
      case 0x6b:
        __isoc99_sscanf(local_59e8,"%*s %ld",&local_5a40);
        if (4 < local_5a40) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x7cd,
                            "Solve-In-Memory Flag %ld(record k in .hdr file) is out of range\n");
        }
        break;
      case 0x4c:
      case 0x6c:
        puVar16 = local_51e8;
        __isoc99_sscanf(local_59e8,"%*s %s",puVar16);
        puVar15 = puVar16;
        do {
          puVar14 = puVar15;
          uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
          uVar8 = uVar7 & 0x80808080;
          puVar15 = puVar14 + 1;
        } while (uVar8 == 0);
        if ((uVar7 & 0x8080) == 0) {
          puVar15 = (uint *)((long)puVar14 + 6);
          uVar8 = uVar8 >> 0x10;
        }
        lVar12 = (long)puVar15 + ((-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)) - (long)puVar16);
        if (acStack_51ec[lVar12 + 3] != '/') {
          acStack_51ec[lVar12 + 5] = *(char *)((long)puVar16 + lVar12);
          *(char *)((long)puVar16 + lVar12) = '/';
        }
        break;
      case 0x4d:
      case 0x6d:
        __isoc99_sscanf(local_59e8,"%*s %ld",&local_5a70);
        if (4 < local_5a70) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x767,
                            "Multiple Load Case Flag %ld(record m in .hdr file) is out of range\n");
        }
        if (local_5a70 < 0) {
          local_5a70 = -1;
        }
        break;
      case 0x4e:
      case 0x6e:
        __isoc99_sscanf(local_59e8,"%*s %ld",&local_5a80);
        if (local_5a80 < 1) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x753,
                            "Number of nodes %ld(record n in .hdr file) must be positive\n");
        }
        break;
      case 0x4f:
      case 0x6f:
        __isoc99_sscanf(local_59e8,"%*s %ld",&local_5a78);
        if (local_5a78 < 0) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x75d,
                            "Number of offdiagonals %ld(record o in .hdr file) cannot be negative\n"
                           );
        }
        break;
      case 0x50:
      case 0x70:
        iVar9 = __isoc99_sscanf(local_59e8,"%*s %ld %ld",&local_5a58,&local_5a50);
        bVar5 = true;
        if (iVar9 != 2) {
          local_5a50 = 0;
        }
        break;
      case 0x52:
      case 0x72:
        __isoc99_sscanf(local_59e8,"%*s %ld",&local_5a48);
        if (local_5a48 < 0) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x77e,
                            "Reduce Problem Size Flag %ld(record r in .hdr file) is out of range\n")
          ;
        }
        break;
      case 0x53:
      case 0x73:
        __isoc99_sscanf(local_59e8,"%*s %c",&local_5a81);
        if ((local_5a81 & 0xdf) == 0x4c) {
          local_5a81 = 0x4c;
          bVar1 = true;
        }
        else {
          if ((local_5a81 & 0xdf) != 0x55) {
                    /* WARNING: Subroutine does not return */
            CyQyIcOOLOXSPAIPP(9999,"main.c",0x7b5,
                              "Record s value %c should be \'l\' or \'L\' (Lower Solve), \'u\' or \'U\' (Upper Solve)\n"
                             );
          }
          local_5a81 = 0x55;
          bVar1 = true;
        }
        break;
      case 0x55:
      case 0x75:
        __isoc99_sscanf(local_59e8,"%*s %ld",&local_5a30);
        if (param_3 != 0) {
          if (local_5a30 < 1) {
                    /* WARNING: Subroutine does not return */
            CyQyIcOOLOXSPAIPP(9999,"main.c",0x7e5,
                              "nUnsym %ld(record u in .hdr file) is out of range\n");
          }
          bVar3 = 1;
        }
        break;
      case 0x58:
      case 0x78:
        __isoc99_sscanf(local_59e8,"%*s %ld",&local_5a38);
        if (1 < local_5a38) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x7d7,
                            "Order Selector %ld(record x in .hdr file) is out of range\n");
        }
        break;
      case 0x5a:
      case 0x7a:
        __isoc99_sscanf(local_59e8,"%*s %ld",&local_5a28);
        if (0x3f < local_5a28 - 1U) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x7f0,
                            "KRJrEOByAP Selector %ld(record z in .hdr file) is out of range\n");
        }
      }
      cVar6 = DBQYjFKBuFQEdFOPQaEyOgKqBQXSPAIPP(pFVar11,"nomardcpslkxuzhNOMARDCPSLKXUZH",local_59e8)
      ;
    } while (cVar6 == '\0');
    fclose(pFVar11);
    if (local_5a70 == -1) {
LAB_0095e55f:
      local_5a50 = 0;
      local_5a60 = 0;
      bVar4 = 0;
    }
    else {
      if (bVar1) {
        local_5a50 = 0;
        local_5a60 = 0;
        bVar5 = false;
      }
      else {
        bVar4 = 0;
        if (!bVar5) goto LAB_0095d7fb;
      }
      if ((local_5a58 < 1) || (local_5a80 <= local_5a58)) {
                    /* WARNING: Subroutine does not return */
        CyQyIcOOLOXSPAIPP(9999,"main.c",0x820,
                          "WSize %ld(record p in .hdr file) is not in range [%ld,%ld]\n",local_5a58,
                          1,local_5a80 + -1);
      }
      bVar4 = 0;
      if (bVar5) {
        if (1 < local_5a50) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x82a,
                            "reducedSystemOnlyF %ld(record p in .hdr file) is out of range\n");
        }
        if (local_5a70 != 4) {
          local_5a70 = 0;
        }
        local_5a60 = 0;
        local_5a48 = 0;
        bVar4 = 1;
      }
    }
  }
  else {
    puVar16 = (uint *)0x0;
    fclose(pFVar11);
    bVar3 = 0;
    bVar2 = false;
    local_5ab8 = 0;
    if (local_5a70 == -1) {
      lVar20 = 0;
      goto LAB_0095e55f;
    }
    bVar4 = 0;
    lVar20 = 0;
  }
LAB_0095d7fb:
  if (local_5a68 < dVar21) {
    local_5a68 = dVar21;
  }
  if (!bVar2) {
    local_39e8[1] = &local_5a18;
    local_5a16 = 0;
    lVar20 = 1;
    local_5a18 = 0x2f2e;
  }
  DBQZyPBhLzlyJBXSPAIPP(param_1,local_4de8);
  puVar15 = puVar16;
  if (puVar16 == (uint *)0x0) {
    local_5a98 = (undefined1 *)0x0;
  }
  else {
    do {
      puVar14 = puVar15;
      uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
      uVar8 = uVar7 & 0x80808080;
      puVar15 = puVar14 + 1;
    } while (uVar8 == 0);
    if ((uVar7 & 0x8080) == 0) {
      puVar15 = (uint *)((long)puVar14 + 6);
      uVar8 = uVar8 >> 0x10;
    }
    sVar19 = (long)puVar15 + ((-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)) - (long)puVar16);
    puVar15 = local_4de8;
    do {
      puVar14 = puVar15;
      uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
      uVar8 = uVar7 & 0x80808080;
      puVar15 = puVar14 + 1;
    } while (uVar8 == 0);
    if ((uVar7 & 0x8080) == 0) {
      puVar15 = (uint *)((long)puVar14 + 6);
      uVar8 = uVar8 >> 0x10;
    }
    lVar12 = (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)) - (long)local_4de8;
    if (0x100 < (long)puVar15 + sVar19 + 6 + lVar12) {
                    /* WARNING: Subroutine does not return */
      CyQyIcOOLOXSPAIPP(9999,"main.c",0x847,
                        "mainMat:Increase MAX_FILE_NAME (currently %ld), recompile, and rerun...",
                        0x100);
    }
    memcpy(auStack_49eb + 3,puVar16,sVar19);
    memcpy(auStack_49eb + sVar19 + 3,local_4de8,(long)puVar15 + lVar12 + 1);
    local_5a98 = auStack_49eb + 3;
  }
  strcpy((char *)local_31b8,param_1);
  puVar15 = local_31b8;
  do {
    puVar14 = puVar15;
    uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar8 = uVar7 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x6173722e;
  *(undefined2 *)(puVar17 + 1) = 0x74;
  strcpy((char *)local_2d98,param_1);
  puVar15 = local_2d98;
  do {
    puVar14 = puVar15;
    uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar8 = uVar7 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  builtin_strncpy((char *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8))),".rset",6)
  ;
  strcpy((char *)local_2978,param_1);
  puVar15 = local_2978;
  do {
    puVar14 = puVar15;
    uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar8 = uVar7 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x74616d2e;
  *(undefined1 *)(puVar17 + 1) = 0;
  strcpy((char *)local_2558,param_1);
  puVar15 = local_2558;
  do {
    puVar14 = puVar15;
    uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar8 = uVar7 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x74616d2e;
  *(undefined2 *)(puVar17 + 1) = 100;
  strcpy((char *)local_2138,param_1);
  puVar15 = local_2138;
  do {
    puVar14 = puVar15;
    uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar8 = uVar7 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x74616d2e;
  *(undefined2 *)(puVar17 + 1) = 0x66;
  strcpy((char *)local_1d18,param_1);
  puVar15 = local_1d18;
  do {
    puVar14 = puVar15;
    uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar8 = uVar7 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x74616d2e;
  *(undefined2 *)(puVar17 + 1) = 0x74;
  strcpy((char *)local_18f8,param_1);
  puVar15 = local_18f8;
  do {
    puVar14 = puVar15;
    uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar8 = uVar7 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x74616d2e;
  *(undefined2 *)(puVar17 + 1) = 0x6f;
  strcpy((char *)local_14d8,param_1);
  puVar15 = local_14d8;
  do {
    puVar14 = puVar15;
    uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar8 = uVar7 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x7368722e;
  *(undefined1 *)(puVar17 + 1) = 0;
  strcpy((char *)local_10b8,param_1);
  puVar15 = local_10b8;
  do {
    puVar14 = puVar15;
    uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar8 = uVar7 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x6c73722e;
  *(undefined1 *)(puVar17 + 1) = 0;
  strcpy((char *)local_c98,param_1);
  puVar15 = local_c98;
  do {
    puVar14 = puVar15;
    uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar8 = uVar7 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x3231612e;
  *(undefined1 *)(puVar17 + 1) = 0;
  strcpy((char *)local_878,param_1);
  puVar15 = local_878;
  do {
    puVar14 = puVar15;
    uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar8 = uVar7 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *puVar17 = 0x646e652e;
  *(undefined2 *)(puVar17 + 1) = 0x6c;
  strcpy((char *)local_458,param_1);
  puVar15 = local_458;
  do {
    puVar14 = puVar15;
    uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar8 = uVar7 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar8 == 0);
  if ((uVar7 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar8 = uVar8 >> 0x10;
  }
  puVar17 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
  *(undefined2 *)(puVar17 + 1) = 0x73;
  *puVar17 = 0x6e66752e;
  fclose(DAT_00e56340);
  if (local_5a70 != -1) {
    if ((((bVar3 == 0 && local_5ab8 == 0) && bVar4 == 0) || (local_5a50 != 0)) &&
       (lVar12 = CFIBcUFPQPkyFKXSPAIPP(local_14d8), lVar12 == 0)) {
                    /* WARNING: Subroutine does not return */
      CyQyIcOOLOXSPAIPP(9999,"main.c",0x887,"An RHS is required but RHS file %s does not exist\n",
                        local_14d8);
    }
    if ((local_5a81 == 0x55) || (local_5a81 == 0x4c)) {
      strcpy((char *)local_45e8,param_1);
      puVar15 = (uint *)local_45e8;
      do {
        puVar14 = puVar15;
        uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
        uVar8 = uVar7 & 0x80808080;
        puVar15 = puVar14 + 1;
      } while (uVar8 == 0);
      if ((uVar7 & 0x8080) == 0) {
        puVar15 = (uint *)((long)puVar14 + 6);
        uVar8 = uVar8 >> 0x10;
      }
      builtin_strncpy((char *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8))),
                      ".parf",6);
      strcpy((char *)local_41e8,param_1);
      puVar15 = (uint *)local_41e8;
      do {
        puVar14 = puVar15;
        uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
        uVar8 = uVar7 & 0x80808080;
        puVar15 = puVar14 + 1;
      } while (uVar8 == 0);
      if ((uVar7 & 0x8080) == 0) {
        puVar15 = (uint *)((long)puVar14 + 6);
        uVar8 = uVar8 >> 0x10;
      }
      puVar17 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8)));
      *puVar17 = 0x6f73692e;
      *(undefined2 *)(puVar17 + 1) = 0x6c;
      strcpy((char *)local_3de8,param_1);
      puVar15 = (uint *)local_3de8;
      do {
        puVar14 = puVar15;
        uVar7 = *puVar14 + 0xfefefeff & ~*puVar14;
        uVar8 = uVar7 & 0x80808080;
        puVar15 = puVar14 + 1;
      } while (uVar8 == 0);
      if ((uVar7 & 0x8080) == 0) {
        puVar15 = (uint *)((long)puVar14 + 6);
        uVar8 = uVar8 >> 0x10;
      }
      builtin_strncpy((char *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar8,(byte)uVar8))),
                      ".portv",7);
      lVar12 = CFIBcUFPQPkyFKXSPAIPP(local_45e8);
      if (lVar12 == 0) {
                    /* WARNING: Subroutine does not return */
        CyQyIcOOLOXSPAIPP(9999,"main.c",0x89c,
                          "Partial Factor File %s is required but does not exist\n",local_45e8);
      }
      if ((local_5a81 == 0x4c) && (lVar12 = CFIBcUFPQPkyFKXSPAIPP(local_14d8), lVar12 == 0)) {
                    /* WARNING: Subroutine does not return */
        CyQyIcOOLOXSPAIPP(9999,"main.c",0x8a4,"An RHS is required but RHS file %s does not exist\n",
                          local_14d8);
      }
      if (local_5a81 == 0x55) {
        lVar12 = CFIBcUFPQPkyFKXSPAIPP(local_41e8);
        if (lVar12 == 0) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x8ad,"Required intermediate file %s does not exist\n",
                            local_41e8);
        }
        lVar12 = CFIBcUFPQPkyFKXSPAIPP(local_3de8);
        if (lVar12 == 0) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x8b3,"Required input file %s does not exist\n",local_3de8
                           );
        }
      }
    }
    if (((bVar4 & (bVar3 ^ 1)) != 0) && (lVar12 = CFIBcUFPQPkyFKXSPAIPP(local_2d98), lVar12 == 0)) {
      lVar12 = CFIBcUFPQPkyFKXSPAIPP(local_31b8);
      if (lVar12 == 0) {
                    /* WARNING: Subroutine does not return */
        CyQyIcOOLOXSPAIPP(9999,"main.c",0x8be,"Required file <jobname>.rset does not exist\n",
                          local_2d98);
      }
      _LKSBOQpPyQrLpPBQ(local_5a58,local_31b8,local_2d98);
    }
    if ((((-1 < local_5a70) && (local_5a70 != 2)) && (local_5ab8 == 0)) &&
       (((lVar12 = CFIBcUFPQPkyFKXSPAIPP(local_2558), lVar12 == 0 ||
         (lVar12 = CFIBcUFPQPkyFKXSPAIPP(local_2138), lVar12 == 0)) ||
        ((lVar12 = CFIBcUFPQPkyFKXSPAIPP(local_1d18), lVar12 == 0 ||
         (lVar12 = CFIBcUFPQPkyFKXSPAIPP(local_18f8), lVar12 == 0)))))) {
      PMIFQkyQdFIBkyFKXSPAIPP(local_2978,local_2558,local_2138,local_1d18,local_18f8);
    }
  }
  if (bVar3 == 0) {
LAB_0095ddda:
    if (param_2 == 5) {
      lVar12 = 1;
      sVar19 = strlen(param_1);
      __dest_00 = tc_malloc(sVar19 + 5);
      memcpy(__dest_00,param_1,sVar19);
      *(undefined4 *)(sVar19 + (long)__dest_00) = 0x7073622e;
      *(undefined1 *)((undefined4 *)(sVar19 + (long)__dest_00) + 1) = 0;
      pFVar11 = (FILE *)LMBKdFIBkyFKurXSPAIPP(__dest_00);
      fprintf(pFVar11,"%s\n",param_1);
      fprintf(pFVar11,"%ld real\n",local_5a80);
      pFVar13 = (FILE *)LMBKdFIBkyFKpXSPAIPP(local_2558);
      if (0 < local_5a80) {
        do {
          sVar19 = fread(local_3de8,8,1,pFVar13);
          if (sVar19 != 1) {
                    /* WARNING: Subroutine does not return */
            CyQyIcOOLOXSPAIPP(9999,"main.c",0x909,"fread matdFp failed: Reading %ld but read %ld\n",
                              1,sVar19);
          }
          fprintf(pFVar11,"%ld %ld %g\n",local_3de8[0],lVar12,lVar12);
          lVar12 = lVar12 + 1;
        } while (lVar12 <= local_5a80);
      }
      lVar12 = 1;
      fclose(pFVar13);
      pFVar13 = (FILE *)LMBKdFIBkyFKpXSPAIPP(local_2138);
      __stream = (FILE *)LMBKdFIBkyFKpXSPAIPP(local_1d18);
      __stream_00 = (FILE *)LMBKdFIBkyFKpXSPAIPP(local_18f8);
      if (0 < local_5a78) {
        do {
          sVar19 = fread(local_45e8,8,1,pFVar13);
          if (sVar19 != 1) {
                    /* WARNING: Subroutine does not return */
            CyQyIcOOLOXSPAIPP(9999,"main.c",0x91a,"fread matfFp failed: Reading %ld but read %ld\n",
                              1,sVar19);
          }
          sVar19 = fread(local_41e8,8,1,__stream);
          if (sVar19 != 1) {
                    /* WARNING: Subroutine does not return */
            CyQyIcOOLOXSPAIPP(9999,"main.c",0x922,"fread mattFp failed: Reading %ld but read %ld\n",
                              1,sVar19);
          }
          sVar19 = fread(local_3de8,8,1,__stream_00);
          if (sVar19 != 1) {
                    /* WARNING: Subroutine does not return */
            CyQyIcOOLOXSPAIPP(9999,"main.c",0x92a,"fread matoFp failed: Reading %ld but read %ld\n",
                              1,sVar19);
          }
          lVar12 = lVar12 + 1;
          fprintf(pFVar11,"%ld %ld %g\n",local_3de8[0],local_45e8[0],local_41e8[0]);
          fprintf(pFVar11,"%ld %ld %g\n",local_3de8[0],local_41e8[0],local_45e8[0]);
        } while (lVar12 <= local_5a78);
      }
      lVar12 = 1;
      fwrite(&DAT_00b185a6,1,8,pFVar11);
      fclose(pFVar13);
      fclose(__stream);
      fclose(__stream_00);
      pFVar13 = (FILE *)LMBKdFIBkyFKpXSPAIPP(local_14d8);
      if (0 < local_5a80) {
        do {
          sVar19 = fread(local_3de8,8,1,pFVar13);
          if (sVar19 != 1) {
                    /* WARNING: Subroutine does not return */
            CyQyIcOOLOXSPAIPP(9999,"main.c",0x93d,"fread rhsFp failed: Reading %ld but read %ld\n",1
                              ,sVar19);
          }
          lVar12 = lVar12 + 1;
          fprintf(pFVar11,"%g\n",local_3de8[0]);
        } while (lVar12 <= local_5a80);
      }
      lVar12 = 0;
      fclose(pFVar11);
      tc_cfree(__dest_00);
    }
    else {
      lVar12 = SPAIPP(local_5a68,param_1,local_5a80,local_5a78,local_5a70,local_5a60,lVar20,
                      local_39e8,local_5a48,bVar4,local_5a58,(int)(char)local_5a81,puVar16,
                      local_5a40,local_5a38,local_4de8,local_5a98,local_5a50,local_2d98,
                      yKFIkyIIL_XSPAIPP,yKFIdOBBXSPAIPP,local_2558,local_2138,local_1d18,local_18f8,
                      local_14d8,local_10b8,local_c98,1,param_2,bVar3,local_5a30,local_878,local_458
                      ,local_5a28,local_5ab8,local_5a20);
    }
  }
  else {
    if ((local_5a70 != 4) && (local_5a70 != -1)) {
      local_5a70 = 0;
    }
    local_5a60 = 0;
    local_5a48 = 1;
    local_5a81 = 0x46;
    local_5a38 = 0;
    local_5a50 = 0;
    if (param_2 != 4) goto LAB_0095ddda;
    lVar12 = 0;
  }
  if ((bVar2) && (lVar20 != 0)) {
    lVar18 = 1;
    do {
      ppuVar10 = local_39e8 + lVar18;
      lVar18 = lVar18 + 1;
      tc_cfree(*ppuVar10);
    } while (lVar18 <= lVar20);
  }
  DAT_00e56340 = (FILE *)LMBKdFIBkyFKYrXSPAIPP(&DAT_00e56240);
  if (lVar12 == 5) {
    fwrite("SQRT_OF_NEGATIVE_NUMBER\n",1,0x18,stderr);
    goto LAB_0095e045;
  }
  if (lVar12 < 6) {
    if (lVar12 == 2) {
      fwrite("NEGATIVE_OR_ZERO_PIVOT\n",1,0x17,stderr);
      goto LAB_0095e045;
    }
    if (lVar12 < 3) {
      if (lVar12 == 0) goto LAB_0095ebea;
    }
    else {
      if (lVar12 == 3) {
        fwrite("NEGATIVE_DIAGONAL_OF_A\n",1,0x17,stderr);
        goto LAB_0095e045;
      }
      if (lVar12 == 4) {
        fwrite("DIVISION_BY_ZERO\n",1,0x11,stderr);
        goto LAB_0095e045;
      }
    }
  }
  else {
    if (lVar12 == 7) {
      fwrite("INPUT_DATA_INCONSISTENT\n",1,0x18,stderr);
      goto LAB_0095e045;
    }
    if (lVar12 < 7) {
      fwrite("INSUFFICIENT_MEMORY\n",1,0x14,stderr);
      goto LAB_0095e045;
    }
    if (lVar12 == 8) {
      fwrite("FILE_ERROR\n",1,0xb,stderr);
      goto LAB_0095e045;
    }
    if (lVar12 == 9999) {
      fwrite("vsdlss_FATAL\n",1,0xd,stderr);
      goto LAB_0095e045;
    }
  }
  fwrite("Unknown status.\n",1,0x10,stderr);
  if (lVar12 == 0) {
LAB_0095ebea:
    if (DAT_00e56340 != (FILE *)0x0) {
      fwrite("Solver SPAIPP returned successfully.\n",1,0x25,DAT_00e56340);
    }
    return 0;
  }
LAB_0095e045:
                    /* WARNING: Subroutine does not return */
  CyQyIcOOLOXSPAIPP(lVar12,"main.c",0x9a7,"Solver SPAIPP failed with status %ld.\n",lVar12);
}


