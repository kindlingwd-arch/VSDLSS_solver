/* driver: PBQkIAaLKQOLInyOJPXSPAIPP addr=00996000 size=5c1 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void PBQkIAaLKQOLInyOJPXSPAIPP(void)

{
  char cVar1;
  ulong uVar2;
  FILE *__stream;
  long lVar3;
  uint *puVar4;
  uint *puVar5;
  char *pcVar6;
  char *pcVar7;
  uint uVar8;
  uint uVar9;
  undefined1 uVar10;
  bool bVar11;
  byte bVar12;
  undefined4 local_818;
  char local_418 [1024];
  
  bVar12 = 0;
  DAT_00e8e600 = 200;
  DAT_00e8e610 = 0x14;
  DAT_00e8e630 = 3;
  DAT_00e8e638 = 8;
  DAT_00e8e640 = 0x28;
  DAT_00e8e648 = 0;
  _DAT_00e8e650 = 100;
  DAT_00e8e658 = 8;
  DAT_00e8e660 = 8;
  DAT_00e8e608 = DAT_00abc218;
  DAT_00e8e670 = 6;
  DAT_00e8e688 = 1;
  DAT_00e8e618 = DAT_00ab71f8;
  DAT_00e8e690 = 0;
  _DAT_00e8e620 = DAT_00acec78;
  _DAT_00e8e628 = DAT_00ab7240;
  DAT_00e8e668 = DAT_00b0eec8;
  DAT_00e8e678 = DAT_00b1d068;
  _DAT_00e8e680 = DAT_00afab98;
  __stream = fopen("mldparms.cas","r");
  if (__stream != (FILE *)0x0) {
    lVar3 = kjbeBQYjFKBXSPAIPP(&local_818,0x3ff,__stream);
    while (lVar3 == 0) {
      if ((char)local_818 == ';') {
LAB_00996370:
        lVar3 = kjbeBQYjFKBXSPAIPP(&local_818,0x3ff,__stream);
      }
      else {
        uVar2 = 0;
        cVar1 = (char)local_818;
        while ((puVar5 = &local_818, cVar1 != '\0' && (puVar5 = &local_818, cVar1 != '='))) {
          cVar1 = *(char *)((long)&local_818 + uVar2 + 1);
          uVar2 = uVar2 + 1;
        }
        do {
          puVar4 = puVar5;
          uVar8 = *puVar4 + 0xfefefeff & ~*puVar4;
          uVar9 = uVar8 & 0x80808080;
          puVar5 = puVar4 + 1;
        } while (uVar9 == 0);
        if ((uVar8 & 0x8080) == 0) {
          puVar5 = (uint *)((long)puVar4 + 6);
          uVar9 = uVar9 >> 0x10;
        }
        if ((long)puVar5 + ((-3 - (ulong)CARRY1((byte)uVar9,(byte)uVar9)) - (long)&local_818) <=
            uVar2) goto LAB_00996370;
        uVar10 = 1;
        __isoc99_sscanf(&local_818,"%s = ",local_418,uVar2,uVar9 & 0xffffff00);
        lVar3 = 0x14;
        pcVar6 = local_418;
        pcVar7 = "MLD_MMD_SIZE_THRESH";
        do {
          if (lVar3 == 0) break;
          lVar3 = lVar3 + -1;
          uVar10 = *pcVar6 == *pcVar7;
          pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
          pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
        } while ((bool)uVar10);
        if ((bool)uVar10) {
          __isoc99_sscanf(&local_818,"%*s = %ld\n",&DAT_00e8e600);
          lVar3 = kjbeBQYjFKBXSPAIPP(&local_818,0x3ff,__stream);
        }
        else {
          lVar3 = 0x1a;
          bVar11 = false;
          pcVar6 = local_418;
          pcVar7 = "MLD_MIN_COARSENING_FACTOR";
          do {
            if (lVar3 == 0) break;
            lVar3 = lVar3 + -1;
            bVar11 = *pcVar6 == *pcVar7;
            pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
            pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
          } while (bVar11);
          if (bVar11) {
            __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00e8e608);
            lVar3 = kjbeBQYjFKBXSPAIPP(&local_818,0x3ff,__stream);
          }
          else {
            lVar3 = 0x1b;
            bVar11 = false;
            pcVar6 = local_418;
            pcVar7 = "MLD_STOP_COARSENING_THRESH";
            do {
              if (lVar3 == 0) break;
              lVar3 = lVar3 + -1;
              bVar11 = *pcVar6 == *pcVar7;
              pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
              pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
            } while (bVar11);
            if (bVar11) {
              __isoc99_sscanf(&local_818,"%*s = %ld\n",&DAT_00e8e610);
              lVar3 = kjbeBQYjFKBXSPAIPP(&local_818,0x3ff,__stream);
            }
            else {
              lVar3 = 0x15;
              bVar11 = false;
              pcVar6 = local_418;
              pcVar7 = "MLD_IMBALANCE_FACTOR";
              do {
                if (lVar3 == 0) break;
                lVar3 = lVar3 + -1;
                bVar11 = *pcVar6 == *pcVar7;
                pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
              } while (bVar11);
              if (bVar11) {
                __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00e8e618);
                lVar3 = kjbeBQYjFKBXSPAIPP(&local_818,0x3ff,__stream);
              }
              else {
                lVar3 = 0x15;
                bVar11 = false;
                pcVar6 = local_418;
                pcVar7 = "MLD_SHEM_INDIFF_MULT";
                do {
                  if (lVar3 == 0) break;
                  lVar3 = lVar3 + -1;
                  bVar11 = *pcVar6 == *pcVar7;
                  pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                  pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                } while (bVar11);
                if (bVar11) {
                  __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00e8e620);
                  lVar3 = kjbeBQYjFKBXSPAIPP(&local_818,0x3ff,__stream);
                }
                else {
                  lVar3 = 0x10;
                  bVar11 = false;
                  pcVar6 = local_418;
                  pcVar7 = "MLD_MAXVWT_MULT";
                  do {
                    if (lVar3 == 0) break;
                    lVar3 = lVar3 + -1;
                    bVar11 = *pcVar6 == *pcVar7;
                    pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                    pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                  } while (bVar11);
                  if (bVar11) {
                    __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00e8e628);
                    lVar3 = kjbeBQYjFKBXSPAIPP(&local_818,0x3ff,__stream);
                  }
                  else {
                    lVar3 = 0x1a;
                    bVar11 = false;
                    pcVar6 = local_418;
                    pcVar7 = "MLD_INIT_GGGP_ROOTS_SMALL";
                    do {
                      if (lVar3 == 0) break;
                      lVar3 = lVar3 + -1;
                      bVar11 = *pcVar6 == *pcVar7;
                      pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                      pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                    } while (bVar11);
                    if (!bVar11) {
                      lVar3 = 0x1a;
                      pcVar6 = local_418;
                      pcVar7 = "MLD_INIT_GGGP_ROOTS_LARGE";
                      do {
                        if (lVar3 == 0) break;
                        lVar3 = lVar3 + -1;
                        bVar11 = *pcVar6 == *pcVar7;
                        pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                        pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                      } while (bVar11);
                      if (bVar11) {
                        __isoc99_sscanf(&local_818,"%*s = %ld\n",&DAT_00e8e638);
                      }
                      else {
                        lVar3 = 0x1e;
                        bVar11 = false;
                        pcVar6 = local_418;
                        pcVar7 = "MLD_BFS_FROM_ALL_NODES_THRESH";
                        do {
                          if (lVar3 == 0) break;
                          lVar3 = lVar3 + -1;
                          bVar11 = *pcVar6 == *pcVar7;
                          pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                          pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                        } while (bVar11);
                        if (bVar11) {
                          __isoc99_sscanf(&local_818,"%*s = %ld\n",&DAT_00e8e640);
                        }
                        else {
                          lVar3 = 0x1c;
                          bVar11 = false;
                          pcVar6 = local_418;
                          pcVar7 = "MLD_BFS_FROM_ALL_NODES_FLAG";
                          do {
                            if (lVar3 == 0) break;
                            lVar3 = lVar3 + -1;
                            bVar11 = *pcVar6 == *pcVar7;
                            pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                            pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                          } while (bVar11);
                          if (bVar11) {
                            __isoc99_sscanf(&local_818,"%*s = %ld\n",&DAT_00e8e648);
                          }
                          else {
                            lVar3 = 0x19;
                            bVar11 = false;
                            pcVar6 = local_418;
                            pcVar7 = "MLD_MAX_UPHILL_BKL_MOVES";
                            do {
                              if (lVar3 == 0) break;
                              lVar3 = lVar3 + -1;
                              bVar11 = *pcVar6 == *pcVar7;
                              pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                              pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                            } while (bVar11);
                            if (bVar11) {
                              __isoc99_sscanf(&local_818,"%*s = %ld\n",&DAT_00e8e650);
                            }
                            else {
                              lVar3 = 0x19;
                              bVar11 = false;
                              pcVar6 = local_418;
                              pcVar7 = "MLD_BKL_NUMPASSES_COARSE";
                              do {
                                if (lVar3 == 0) break;
                                lVar3 = lVar3 + -1;
                                bVar11 = *pcVar6 == *pcVar7;
                                pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                                pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                              } while (bVar11);
                              if (bVar11) {
                                __isoc99_sscanf(&local_818,"%*s = %ld\n",&DAT_00e8e658);
                              }
                              else {
                                lVar3 = 0x17;
                                bVar11 = false;
                                pcVar6 = local_418;
                                pcVar7 = "MLD_BKL_NUMPASSES_FINE";
                                do {
                                  if (lVar3 == 0) break;
                                  lVar3 = lVar3 + -1;
                                  bVar11 = *pcVar6 == *pcVar7;
                                  pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                                  pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                                } while (bVar11);
                                if (bVar11) {
                                  __isoc99_sscanf(&local_818,"%*s = %ld\n",&DAT_00e8e660);
                                }
                                else {
                                  lVar3 = 0x20;
                                  bVar11 = false;
                                  pcVar6 = local_418;
                                  pcVar7 = "MLD_BKL_NUMPASSES_SWITCH_THRESH";
                                  do {
                                    if (lVar3 == 0) break;
                                    lVar3 = lVar3 + -1;
                                    bVar11 = *pcVar6 == *pcVar7;
                                    pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                                    pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                                  } while (bVar11);
                                  if (bVar11) {
                                    __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00e8e668);
                                  }
                                  else {
                                    lVar3 = 0x17;
                                    bVar11 = false;
                                    pcVar6 = local_418;
                                    pcVar7 = "MLD_BKL_NUMPASSES_NODE";
                                    do {
                                      if (lVar3 == 0) break;
                                      lVar3 = lVar3 + -1;
                                      bVar11 = *pcVar6 == *pcVar7;
                                      pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                                      pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                                    } while (bVar11);
                                    if (bVar11) {
                                      __isoc99_sscanf(&local_818,"%*s = %ld\n",&DAT_00e8e670);
                                    }
                                    else {
                                      lVar3 = 0x11;
                                      bVar11 = false;
                                      pcVar6 = local_418;
                                      pcVar7 = "MLD_AVG_VWT_MULT";
                                      do {
                                        if (lVar3 == 0) break;
                                        lVar3 = lVar3 + -1;
                                        bVar11 = *pcVar6 == *pcVar7;
                                        pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                                        pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                                      } while (bVar11);
                                      if (bVar11) {
                                        __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00e8e678);
                                      }
                                      else {
                                        lVar3 = 0x1e;
                                        bVar11 = false;
                                        pcVar6 = local_418;
                                        pcVar7 = "MLD_EDGEREFINE_IMBALANCE_FRAC";
                                        do {
                                          if (lVar3 == 0) break;
                                          lVar3 = lVar3 + -1;
                                          bVar11 = *pcVar6 == *pcVar7;
                                          pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                                          pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                                        } while (bVar11);
                                        if (bVar11) {
                                          __isoc99_sscanf(&local_818,"%*s = %lg\n",&DAT_00e8e680);
                                        }
                                        else {
                                          lVar3 = 0x1d;
                                          bVar11 = false;
                                          pcVar6 = local_418;
                                          pcVar7 = "MLD_PARTDIFF_LENIENCE_FACTOR";
                                          do {
                                            if (lVar3 == 0) break;
                                            lVar3 = lVar3 + -1;
                                            bVar11 = *pcVar6 == *pcVar7;
                                            pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                                            pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                                          } while (bVar11);
                                          if (bVar11) {
                                            __isoc99_sscanf(&local_818,"%*s = %ld\n",&DAT_00e8e688);
                                          }
                                          else {
                                            lVar3 = 0xd;
                                            bVar11 = false;
                                            pcVar6 = local_418;
                                            pcVar7 = "MLD_PLUS_MMD";
                                            do {
                                              if (lVar3 == 0) break;
                                              lVar3 = lVar3 + -1;
                                              bVar11 = *pcVar6 == *pcVar7;
                                              pcVar6 = pcVar6 + (ulong)bVar12 * -2 + 1;
                                              pcVar7 = pcVar7 + (ulong)bVar12 * -2 + 1;
                                            } while (bVar11);
                                            if (bVar11) {
                                              __isoc99_sscanf(&local_818,"%*s = %ld\n",&DAT_00e8e690
                                                             );
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
                      goto LAB_00996370;
                    }
                    __isoc99_sscanf(&local_818,"%*s = %ld\n",&DAT_00e8e630);
                    lVar3 = kjbeBQYjFKBXSPAIPP(&local_818,0x3ff,__stream);
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


