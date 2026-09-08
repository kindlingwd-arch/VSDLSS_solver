/* VSDLSS function (decompiled by Ghidra) */
/* name: setMldControlParms_vsdlss  addr: 009fbdd0  size: 5c2 */
#include "vsdlss_ref.h"

/* signature: void setMldControlParms_vsdlss(void); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void setMldControlParms_vsdlss(void)

{
  char cVar1;
  int iVar2;
  FILE *__stream;
  uint *puVar3;
  uint *puVar4;
  ulong uVar5;
  ulong uVar6;
  long lVar7;
  char *pcVar8;
  char *pcVar9;
  uint uVar10;
  uint uVar11;
  undefined1 uVar12;
  bool bVar13;
  byte bVar14;
  undefined4 local_818;
  char local_418 [1024];
  
  bVar14 = 0;
  DAT_00ec5d80 = 200;
  DAT_00ec5d90 = 0x14;
  DAT_00ec5db0 = 3;
  DAT_00ec5db4 = 8;
  DAT_00ec5db8 = 0x28;
  DAT_00ec5dbc = 0;
  DAT_00ec5dc0 = 100;
  DAT_00ec5dc4 = 8;
  DAT_00ec5dc8 = 8;
  DAT_00ec5d88 = DAT_00abc218;
  DAT_00ec5dd8 = 6;
  DAT_00ec5df0 = 1;
  DAT_00ec5d98 = DAT_00ab71f8;
  DAT_00ec5df4 = 0;
  _DAT_00ec5da0 = DAT_00acec78;
  _DAT_00ec5da8 = DAT_00ab7240;
  DAT_00ec5dd0 = DAT_00b0eec8;
  DAT_00ec5de0 = DAT_00b1d068;
  _DAT_00ec5de8 = DAT_00afab98;
  __stream = fopen("mldparms.cas","r");
  if (__stream != (FILE *)0x0) {
    iVar2 = MLDGetALine_vsdlss(&local_818,0x3ff,__stream);
    while (iVar2 == 0) {
      if ((char)local_818 == ';') {
LAB_009fc140:
        iVar2 = MLDGetALine_vsdlss(&local_818,0x3ff,__stream);
      }
      else {
        uVar6 = 0;
        puVar4 = &local_818;
        if (((char)local_818 != '\0') && (puVar4 = &local_818, (char)local_818 != '=')) {
          do {
            uVar5 = uVar6 + 1;
            cVar1 = *(char *)((long)&local_818 + uVar6 + 1);
            if (cVar1 == '\0') break;
            uVar6 = uVar5;
          } while (cVar1 != '=');
          uVar6 = (ulong)(int)uVar5;
          puVar4 = &local_818;
        }
        do {
          puVar3 = puVar4;
          uVar10 = *puVar3 + 0xfefefeff & ~*puVar3;
          uVar11 = uVar10 & 0x80808080;
          puVar4 = puVar3 + 1;
        } while (uVar11 == 0);
        if ((uVar10 & 0x8080) == 0) {
          puVar4 = (uint *)((long)puVar3 + 6);
          uVar11 = uVar11 >> 0x10;
        }
        if ((long)puVar4 + ((-3 - (ulong)CARRY1((byte)uVar11,(byte)uVar11)) - (long)&local_818) <=
            uVar6) goto LAB_009fc140;
        uVar12 = 1;
        __isoc99_sscanf(&local_818,"%s = ",local_418,uVar6,uVar11 & 0xffffff00);
        lVar7 = 0x14;
        pcVar8 = local_418;
        pcVar9 = "MLD_MMD_SIZE_THRESH";
        do {
          if (lVar7 == 0) break;
          lVar7 = lVar7 + -1;
          uVar12 = *pcVar8 == *pcVar9;
          pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
          pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
        } while ((bool)uVar12);
        if ((bool)uVar12) {
          __isoc99_sscanf(&local_818,"%*s = %d\n",&DAT_00ec5d80);
          iVar2 = MLDGetALine_vsdlss(&local_818,0x3ff,__stream);
        }
        else {
          lVar7 = 0x1a;
          bVar13 = false;
          pcVar8 = local_418;
          pcVar9 = "MLD_MIN_COARSENING_FACTOR";
          do {
            if (lVar7 == 0) break;
            lVar7 = lVar7 + -1;
            bVar13 = *pcVar8 == *pcVar9;
            pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
            pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
          } while (bVar13);
          if (bVar13) {
            __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00ec5d88);
            iVar2 = MLDGetALine_vsdlss(&local_818,0x3ff,__stream);
          }
          else {
            lVar7 = 0x1b;
            bVar13 = false;
            pcVar8 = local_418;
            pcVar9 = "MLD_STOP_COARSENING_THRESH";
            do {
              if (lVar7 == 0) break;
              lVar7 = lVar7 + -1;
              bVar13 = *pcVar8 == *pcVar9;
              pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
              pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
            } while (bVar13);
            if (bVar13) {
              __isoc99_sscanf(&local_818,"%*s = %d\n",&DAT_00ec5d90);
              iVar2 = MLDGetALine_vsdlss(&local_818,0x3ff,__stream);
            }
            else {
              lVar7 = 0x15;
              bVar13 = false;
              pcVar8 = local_418;
              pcVar9 = "MLD_IMBALANCE_FACTOR";
              do {
                if (lVar7 == 0) break;
                lVar7 = lVar7 + -1;
                bVar13 = *pcVar8 == *pcVar9;
                pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
              } while (bVar13);
              if (bVar13) {
                __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00ec5d98);
                iVar2 = MLDGetALine_vsdlss(&local_818,0x3ff,__stream);
              }
              else {
                lVar7 = 0x15;
                bVar13 = false;
                pcVar8 = local_418;
                pcVar9 = "MLD_SHEM_INDIFF_MULT";
                do {
                  if (lVar7 == 0) break;
                  lVar7 = lVar7 + -1;
                  bVar13 = *pcVar8 == *pcVar9;
                  pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                  pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                } while (bVar13);
                if (bVar13) {
                  __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00ec5da0);
                  iVar2 = MLDGetALine_vsdlss(&local_818,0x3ff,__stream);
                }
                else {
                  lVar7 = 0x10;
                  bVar13 = false;
                  pcVar8 = local_418;
                  pcVar9 = "MLD_MAXVWT_MULT";
                  do {
                    if (lVar7 == 0) break;
                    lVar7 = lVar7 + -1;
                    bVar13 = *pcVar8 == *pcVar9;
                    pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                    pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                  } while (bVar13);
                  if (bVar13) {
                    __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00ec5da8);
                    iVar2 = MLDGetALine_vsdlss(&local_818,0x3ff,__stream);
                  }
                  else {
                    lVar7 = 0x1a;
                    bVar13 = false;
                    pcVar8 = local_418;
                    pcVar9 = "MLD_INIT_GGGP_ROOTS_SMALL";
                    do {
                      if (lVar7 == 0) break;
                      lVar7 = lVar7 + -1;
                      bVar13 = *pcVar8 == *pcVar9;
                      pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                      pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                    } while (bVar13);
                    if (!bVar13) {
                      lVar7 = 0x1a;
                      pcVar8 = local_418;
                      pcVar9 = "MLD_INIT_GGGP_ROOTS_LARGE";
                      do {
                        if (lVar7 == 0) break;
                        lVar7 = lVar7 + -1;
                        bVar13 = *pcVar8 == *pcVar9;
                        pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                        pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                      } while (bVar13);
                      if (bVar13) {
                        __isoc99_sscanf(&local_818,"%*s = %d\n",&DAT_00ec5db4);
                      }
                      else {
                        lVar7 = 0x1e;
                        bVar13 = false;
                        pcVar8 = local_418;
                        pcVar9 = "MLD_BFS_FROM_ALL_NODES_THRESH";
                        do {
                          if (lVar7 == 0) break;
                          lVar7 = lVar7 + -1;
                          bVar13 = *pcVar8 == *pcVar9;
                          pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                          pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                        } while (bVar13);
                        if (bVar13) {
                          __isoc99_sscanf(&local_818,"%*s = %d\n",&DAT_00ec5db8);
                        }
                        else {
                          lVar7 = 0x1c;
                          bVar13 = false;
                          pcVar8 = local_418;
                          pcVar9 = "MLD_BFS_FROM_ALL_NODES_FLAG";
                          do {
                            if (lVar7 == 0) break;
                            lVar7 = lVar7 + -1;
                            bVar13 = *pcVar8 == *pcVar9;
                            pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                            pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                          } while (bVar13);
                          if (bVar13) {
                            __isoc99_sscanf(&local_818,"%*s = %d\n",&DAT_00ec5dbc);
                          }
                          else {
                            lVar7 = 0x19;
                            bVar13 = false;
                            pcVar8 = local_418;
                            pcVar9 = "MLD_MAX_UPHILL_BKL_MOVES";
                            do {
                              if (lVar7 == 0) break;
                              lVar7 = lVar7 + -1;
                              bVar13 = *pcVar8 == *pcVar9;
                              pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                              pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                            } while (bVar13);
                            if (bVar13) {
                              __isoc99_sscanf(&local_818,"%*s = %d\n",&DAT_00ec5dc0);
                            }
                            else {
                              lVar7 = 0x19;
                              bVar13 = false;
                              pcVar8 = local_418;
                              pcVar9 = "MLD_BKL_NUMPASSES_COARSE";
                              do {
                                if (lVar7 == 0) break;
                                lVar7 = lVar7 + -1;
                                bVar13 = *pcVar8 == *pcVar9;
                                pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                                pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                              } while (bVar13);
                              if (bVar13) {
                                __isoc99_sscanf(&local_818,"%*s = %d\n",&DAT_00ec5dc4);
                              }
                              else {
                                lVar7 = 0x17;
                                bVar13 = false;
                                pcVar8 = local_418;
                                pcVar9 = "MLD_BKL_NUMPASSES_FINE";
                                do {
                                  if (lVar7 == 0) break;
                                  lVar7 = lVar7 + -1;
                                  bVar13 = *pcVar8 == *pcVar9;
                                  pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                                  pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                                } while (bVar13);
                                if (bVar13) {
                                  __isoc99_sscanf(&local_818,"%*s = %d\n",&DAT_00ec5dc8);
                                }
                                else {
                                  lVar7 = 0x20;
                                  bVar13 = false;
                                  pcVar8 = local_418;
                                  pcVar9 = "MLD_BKL_NUMPASSES_SWITCH_THRESH";
                                  do {
                                    if (lVar7 == 0) break;
                                    lVar7 = lVar7 + -1;
                                    bVar13 = *pcVar8 == *pcVar9;
                                    pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                                    pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                                  } while (bVar13);
                                  if (bVar13) {
                                    __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00ec5dd0);
                                  }
                                  else {
                                    lVar7 = 0x17;
                                    bVar13 = false;
                                    pcVar8 = local_418;
                                    pcVar9 = "MLD_BKL_NUMPASSES_NODE";
                                    do {
                                      if (lVar7 == 0) break;
                                      lVar7 = lVar7 + -1;
                                      bVar13 = *pcVar8 == *pcVar9;
                                      pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                                      pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                                    } while (bVar13);
                                    if (bVar13) {
                                      __isoc99_sscanf(&local_818,"%*s = %d\n",&DAT_00ec5dd8);
                                    }
                                    else {
                                      lVar7 = 0x11;
                                      bVar13 = false;
                                      pcVar8 = local_418;
                                      pcVar9 = "MLD_AVG_VWT_MULT";
                                      do {
                                        if (lVar7 == 0) break;
                                        lVar7 = lVar7 + -1;
                                        bVar13 = *pcVar8 == *pcVar9;
                                        pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                                        pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                                      } while (bVar13);
                                      if (bVar13) {
                                        __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00ec5de0);
                                      }
                                      else {
                                        lVar7 = 0x1e;
                                        bVar13 = false;
                                        pcVar8 = local_418;
                                        pcVar9 = "MLD_EDGEREFINE_IMBALANCE_FRAC";
                                        do {
                                          if (lVar7 == 0) break;
                                          lVar7 = lVar7 + -1;
                                          bVar13 = *pcVar8 == *pcVar9;
                                          pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                                          pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                                        } while (bVar13);
                                        if (bVar13) {
                                          __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00ec5de8);
                                        }
                                        else {
                                          lVar7 = 0x1d;
                                          bVar13 = false;
                                          pcVar8 = local_418;
                                          pcVar9 = "MLD_PARTDIFF_LENIENCE_FACTOR";
                                          do {
                                            if (lVar7 == 0) break;
                                            lVar7 = lVar7 + -1;
                                            bVar13 = *pcVar8 == *pcVar9;
                                            pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                                            pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                                          } while (bVar13);
                                          if (bVar13) {
                                            __isoc99_sscanf(&local_818,"%*s = %d\n",&DAT_00ec5df0);
                                          }
                                          else {
                                            lVar7 = 0xd;
                                            bVar13 = false;
                                            pcVar8 = local_418;
                                            pcVar9 = "MLD_PLUS_MMD";
                                            do {
                                              if (lVar7 == 0) break;
                                              lVar7 = lVar7 + -1;
                                              bVar13 = *pcVar8 == *pcVar9;
                                              pcVar8 = pcVar8 + (ulong)bVar14 * -2 + 1;
                                              pcVar9 = pcVar9 + (ulong)bVar14 * -2 + 1;
                                            } while (bVar13);
                                            if (bVar13) {
                                              __isoc99_sscanf(&local_818,"%*s = %d\n",&DAT_00ec5df4)
                                              ;
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                      goto LAB_009fc140;
                    }
                    __isoc99_sscanf(&local_818,"%*s = %d\n",&DAT_00ec5db0);
                    iVar2 = MLDGetALine_vsdlss(&local_818,0x3ff,__stream);
                  }
                }
              }
            }
          }
        }
      }
    }
    fclose(__stream);
  }
  return;
}


