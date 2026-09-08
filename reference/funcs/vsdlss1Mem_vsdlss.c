/* VSDLSS function (decompiled by Ghidra) */
/* name: vsdlss1Mem_vsdlss  addr: 00a13000  size: 18e9 */
#include "vsdlss_ref.h"

/* signature: undefined4 vsdlss1Mem_vsdlss(double param_1,int param_2,int param_3,int param_4,int param_5,undefined8 param_6,undefined8 param_7,int param_8,int param_9,int param_10,undefined8 param_11,long param_12,undefined4 param_13,long param_14,undefined8 param_15,int param_16,int param_17,undefined4 param_18,undefined8 param_19,undefined4 *param_20,undefined4 param_21,int param_22,int param_23); */

undefined4
vsdlss1Mem_vsdlss(double param_1,int param_2,int param_3,int param_4,int param_5,undefined8 param_6,
                 undefined8 param_7,int param_8,int param_9,int param_10,undefined8 param_11,
                 long param_12,undefined4 param_13,long param_14,undefined8 param_15,int param_16,
                 int param_17,undefined4 param_18,undefined8 param_19,undefined4 *param_20,
                 undefined4 param_21,int param_22,int param_23)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined4 *puVar13;
  FILE *pFVar14;
  char *pcVar15;
  long lVar16;
  long lVar17;
  undefined8 uVar18;
  undefined8 uVar19;
  undefined8 uVar20;
  undefined8 uVar21;
  undefined8 uVar22;
  undefined8 *puVar23;
  undefined8 uVar24;
  int iVar25;
  bool bVar26;
  bool bVar27;
  bool bVar28;
  undefined4 uVar29;
  long lVar30;
  long local_180;
  long local_130;
  long local_e0;
  undefined1 *local_d8;
  long local_d0;
  undefined8 local_c8;
  time_t local_c0;
  undefined1 local_b8 [16];
  undefined1 local_a8 [16];
  undefined1 local_98 [16];
  undefined1 local_88 [16];
  undefined1 local_78 [16];
  undefined1 local_68 [16];
  undefined1 local_58 [16];
  undefined1 local_48 [24];
  
  local_e0 = 0;
  local_d8 = (undefined1 *)0x0;
  iVar1 = enqSolveInMem_vsdlss();
  local_d0 = 0;
  iVar2 = enqThreadsInPool();
  iVar3 = enqGenMatStats_vsdlss();
  if (param_2 == 0) {
    vsdlssStatus_vsdlss = 0;
    return 0;
  }
  elemsInProb_vsdlss = 0;
  upperANZ_vsdlss = 0;
  varsInProb_vsdlss = param_2;
  nodesInProb_vsdlss = param_2;
  uVar7 = newTime_vsdlss();
  uVar8 = newTime_vsdlss();
  uVar9 = newTime_vsdlss();
  uVar10 = newTime_vsdlss();
  uVar11 = newTime_vsdlss();
  uVar12 = newTime_vsdlss();
  debugPrint_vsdlss("********** Starting vsdlss1mem **********\n");
  casiGetTimeOfDay(local_98);
  casiTimes_vsdlss(uVar9);
  if (MATRIX_CHANGED_vsdlss == '\0') {
    if (iVar1 != 0) {
      local_180 = DAT_00ecb050;
      puVar13 = DAT_00ecb058;
      if (param_8 != 0 || param_16 != 0) goto LAB_00a13997;
      goto LAB_00a1314d;
    }
    puVar13 = (undefined4 *)readFactorOneChunk_vsdlss();
    local_180 = readReorderVec_vsdlss();
    if (param_8 != 0 || param_16 != 0) goto LAB_00a13997;
LAB_00a1372e:
    local_d0 = 0;
LAB_00a1373a:
    local_130 = (long)param_2;
    lVar17 = newVector_vsdlss(0,local_130,0);
LAB_00a13750:
    iVar2 = getReduceProblemSizeF_vsdlss();
    if (((iVar2 == 0) && (iVar1 != 0)) && (param_14 != 0)) {
      if (casiParSolverF == 0) {
        permuteDVector_vsdlss(local_180,param_2,param_14,lVar17);
      }
      else {
        permuteDVectorPar();
      }
    }
    else {
      readDVectorFNumPermute_vsdlss(local_180,param_13,param_2,lVar17);
    }
  }
  else {
    bVar26 = iVar1 != 0;
    bVar27 = param_20 != (undefined4 *)0x0;
    if (((bVar26) && (bVar27)) && (iVar3 - 1U < 2)) {
      genMatStats(param_20[1],*param_20,*(undefined8 *)(param_20 + 0xe),
                  *(undefined8 *)(param_20 + 6),*(undefined8 *)(param_20 + 10),
                  *(undefined8 *)(param_20 + 0xc));
    }
    iVar4 = enqOrderOnly_vsdlss();
    if ((iVar4 != 0) && (bVar26)) {
      iVar4 = getPfTag_vsdlss();
      if (-1 < iVar4) {
        deleteTag_vsdlss(iVar4);
      }
      if (bVar27) {
        if (*(long *)(param_20 + 0xe) != 0) {
          casiFree_vsdlss();
        }
        *(undefined8 *)(param_20 + 0xe) = 0;
        if (*(long *)(param_20 + 0xc) != 0) {
          casiFree_vsdlss();
        }
        *(undefined8 *)(param_20 + 0xc) = 0;
      }
    }
    casiGetTimeOfDay();
    local_c0 = time((time_t *)0x0);
    pFVar14 = (FILE *)getFilePtr_vsdlss(9);
    pcVar15 = ctime(&local_c0);
    fprintf(pFVar14,"Time Stamp at %s : %s","Start of Reordering",pcVar15);
    fflush(pFVar14);
    casiTimes_vsdlss(uVar7);
    casiTimes_vsdlss();
    local_c0 = time((time_t *)0x0);
    pFVar14 = (FILE *)getFilePtr_vsdlss(9);
    pcVar15 = ctime(&local_c0);
    fprintf(pFVar14,"Time Stamp at %s : %s","Before Minimum Degree (Array based)",pcVar15);
    fflush(pFVar14);
    puVar13 = (undefined4 *)newOneFactorChunkHeader_vsdlss(param_2);
    if (bVar26) {
      setSemiPermF_vsdlss(*puVar13);
    }
    lVar17 = newIVector_vsdlss((long)param_2,0,0);
    if ((param_8 == 0) || (param_17 != 0)) {
      iVar4 = enqOrderSelector_vsdlss();
      if (iVar4 != 0) {
        local_d8 = (undefined1 *)newCVector_vsdlss((long)param_2,1,0);
        *local_d8 = 0;
      }
    }
    else {
      getRsetAndMarkVecs_vsdlss(param_9,param_2,&local_e0,&local_d8);
    }
    local_130 = (long)param_2;
    iVar4 = enqOrderSelector_vsdlss();
    if (iVar4 == 0) {
      MLDOrder_vsdlss(param_2,param_5,param_12,param_20,param_18,param_19,lVar17,puVar13,&local_d0);
      setFbspi();
      if (iVar3 == 3) {
        genFbspiOffdiagStats(local_d0,puVar13);
      }
    }
    else {
      if (param_16 != 0) {
        nrerror_vsdlss(1,"vsdlss1mem.c",0x214);
      }
      uVar24 = getGraphArrFromElemArr_vsdlss(param_2,param_5,param_12,param_20);
      debugPrint_vsdlss("Graph (Array based) has been obtained.\n");
      minDegreeOrderArr_vsdlss(param_8,param_9,local_e0,local_d8,uVar24,lVar17,puVar13);
      transferGraphToFactorChunkSort_vsdlss(lVar17,uVar24,puVar13);
      deleteGraph_vsdlss();
      setFbspi();
      longVecsInFC = 0;
    }
    local_c0 = time((time_t *)0x0);
    pFVar14 = (FILE *)getFilePtr_vsdlss(9);
    pcVar15 = ctime(&local_c0);
    fprintf(pFVar14,"Time Stamp at %s : %s","After  Minimum Degree (Array based)",pcVar15);
    fflush(pFVar14);
    lVar16 = *(long *)(puVar13 + 8);
    if (bVar26) {
      uVar24 = tagNewIVector_vsdlss(*puVar13,(long)(int)puVar13[2],0,0);
      *(undefined8 *)(puVar13 + 8) = uVar24;
    }
    else {
      uVar24 = newIVector_vsdlss((long)(int)puVar13[2],0,0);
      *(undefined8 *)(puVar13 + 8) = uVar24;
    }
    iVar4 = breakupBlocks_vsdlss(lVar16,puVar13[5],0x80,uVar24);
    puVar13[5] = iVar4;
    bVar28 = param_16 != 0;
    if ((param_17 != 0) && (bVar28)) {
      copyIVector_vsdlss(*(undefined8 *)(puVar13 + 8),(long)iVar4,lVar16);
      iVar4 = breakupBlockAtPartFactBoundary(lVar16,puVar13[5],param_9,*(undefined8 *)(puVar13 + 8))
      ;
      puVar13[5] = iVar4;
    }
    puVar13[7] = iVar4;
    if (lVar16 != 0) {
      casiFree_vsdlss(lVar16);
    }
    casiTimes_vsdlss(uVar12);
    diffTimes_vsdlss(uVar11,uVar12,t_minDegree_vsdlss);
    availMem_vsdlss = param_1 * DAT_00abc298;
    local_c0 = time((time_t *)0x0);
    blocksInProb_vsdlss = puVar13[5];
    chunksInProb_vsdlss = 1;
    casiTimes_vsdlss(uVar8);
    diffTimes_vsdlss(uVar7,uVar8,t_reorder_vsdlss);
    local_c0 = time((time_t *)0x0);
    pFVar14 = (FILE *)getFilePtr_vsdlss(9);
    pcVar15 = ctime(&local_c0);
    fprintf(pFVar14,"Time Stamp at %s : %s","End of Reordering",pcVar15);
    fflush(pFVar14);
    casiGetTimeOfDay(local_48);
    elapsedReorderTime = casiDiffTimeSecs(local_48,local_58);
    uVar29 = 0;
    getFactorStatsOneChunk_vsdlss
              (puVar13,&maxChunkSize_vsdlss,&avgChunkSize_vsdlss,&factorIndexSize_vsdlss,
               &factorValueSize_vsdlss,&factorSize_vsdlss,&factorNZ_vsdlss,&opsToFactor_vsdlss,
               &opsForTriangSolves_vsdlss,&local_c8);
    debugPrint_vsdlss();
    DAT_00ecb038 = puVar13[2];
    DAT_00ecb030 = local_c8;
    DAT_00ecb028 = factorNZ_vsdlss;
    if ((param_22 != 0) && (param_23 != 0)) {
      upperANZ_vsdlss = param_2 + param_5;
      elemsInProb_vsdlss = param_5;
      deleteElemArrNoHeader(param_20);
      casiTimes_vsdlss(uVar10);
      diffTimes_vsdlss(uVar9,uVar10,t_reducedSolve_vsdlss);
LAB_00a13663:
      casiGetTimeOfDay(local_88);
      elapsedReducedSolveTime = casiDiffTimeSecs(local_88,local_98);
      vsdlssStatus_vsdlss = 0;
      return 0;
    }
    iVar4 = enqOrderOnly_vsdlss();
    if (iVar4 != 0) {
      if (bVar27) {
        deleteElemArrNoHeader(param_20);
      }
      upperANZ_vsdlss = param_5 + param_2;
      elemsInProb_vsdlss = param_5;
      casiTimes_vsdlss(uVar10);
      diffTimes_vsdlss(uVar9,uVar10,t_reducedSolve_vsdlss);
      lVar17 = newVector_vsdlss(0,local_130,1);
      writeVecToFile_vsdlss(param_11,param_2,lVar17);
      if (lVar17 != 0) {
        casiFree_vsdlss(lVar17);
      }
      goto LAB_00a13663;
    }
    local_c0 = time((time_t *)0x0);
    pFVar14 = (FILE *)getFilePtr_vsdlss(9);
    pcVar15 = ctime(&local_c0);
    fprintf(pFVar14,"Time Stamp at %s : %s","Start Factor Initialization",pcVar15);
    fflush(pFVar14);
    casiTimes_vsdlss(uVar7);
    if (bVar26) {
      iVar4 = getReduceProblemSizeF_vsdlss();
      if (iVar4 == 0) {
        if (local_d0 != 0) {
          iVar4 = (param_2 - *(int *)(local_d0 + 8)) * iVar2;
          DAT_00ecb040 = tagNewDVector_vsdlss(0,*puVar13,(long)(param_2 + iVar4),0);
          uVar24 = 0;
          uVar18 = 0;
          uVar21 = 0;
          uVar5 = 0xffffffff;
          uVar22 = CONCAT44(uVar29,iVar4);
          uVar20 = 0;
          lVar16 = DAT_00ecb040 + local_130 * 8;
          uVar19 = 0;
          lVar30 = local_d0;
LAB_00a1437a:
          setPppiInfoJrt(uVar5,uVar21,uVar20,uVar19,uVar18,uVar24,lVar16,uVar22,lVar30);
          uVar29 = (undefined4)((ulong)uVar22 >> 0x20);
          setPfInfoJrt_vsdlss(0xffffffff,0,0,0);
        }
      }
      else if (local_d0 != 0) {
        iVar4 = param_3 + param_2 + param_4;
        processPfsForPar(local_d0,iVar4,param_2,lVar17,param_16);
        iVar25 = (param_2 - *(int *)(local_d0 + 8)) * iVar2;
        uVar5 = getPppiTagSave();
        local_180 = param_14;
        if (param_14 == 0) {
          local_180 = tagNewDVector_vsdlss(0,uVar5,(long)(iVar4 + iVar25),0);
        }
        if (param_16 == 0) {
          uVar24 = getPfDeg1Deg2PartRes_vsdlss();
          copyVector_vsdlss(uVar24,(long)iVar4,local_180);
        }
        setPppiResultVecSave(local_180);
        setPppiRedRhsResSave(local_180 + ((long)param_3 + (long)param_4) * 8);
        if (param_14 == 0) {
          setFbsParWorkVecsAndSize(local_180 + (long)iVar4 * 8,iVar25);
        }
        else {
          setFbsParWorkVecsAndSize(0,0);
        }
        uVar6 = getPfTag_vsdlss();
        deleteTag_vsdlss(uVar6);
        setPfInfoPtrsToNull_vsdlss();
        lVar30 = local_d0;
        lVar16 = getFbsParWorkVecs();
        uVar24 = getPppiRedRhsResSave();
        uVar18 = getPppiResultVecSave();
        uVar19 = getPppiReorderVecSave();
        uVar20 = getCpfSave();
        uVar21 = getPppiSave();
        uVar22 = CONCAT44(uVar29,iVar25);
        goto LAB_00a1437a;
      }
    }
    local_180 = lVar17;
    if (bVar27) {
      renumberSortElimDupInMem(param_20,param_2,param_5,param_12,lVar17);
      if (bVar26) {
        if ((((param_8 != 0) && (!bVar28)) || (iVar4 = getReduceProblemSizeF_vsdlss(), iVar4 == 0))
           || (DAT_00ecb048 == 0)) goto LAB_00a14215;
        if (lVar17 != 0) {
          casiFree_vsdlss();
        }
        local_180 = 0;
      }
LAB_00a13de6:
      allocInitValVecsFCLessMem_vsdlss(param_20,puVar13);
      deleteElemArrNoHeader(param_20);
    }
    else {
      if (longVecsInFC == 0) {
        allocValueVecsFactorOneChunk_vsdlss(puVar13);
      }
      else {
        allocLongValVecsFactorOneChunk_vsdlss();
      }
      if (bVar26) {
LAB_00a14215:
        local_180 = tagNewIVector_vsdlss(*puVar13,local_130,0,0);
        copyIVector_vsdlss(lVar17,local_130,local_180);
        if (lVar17 != 0) {
          casiFree_vsdlss();
        }
        if (bVar27) goto LAB_00a13de6;
      }
      initFactorOneChunk_vsdlss(param_5,param_12,local_180,puVar13);
      casiRemove_vsdlss(*(undefined4 *)(param_12 + 4));
      casiRemove_vsdlss(*(undefined4 *)(param_12 + 8));
      casiRemove_vsdlss(*(undefined4 *)(param_12 + 0xc));
      casiRemove_vsdlss(*(undefined4 *)(param_12 + 0x10));
    }
    upperANZ_vsdlss = param_5 + param_2;
    elemsInProb_vsdlss = param_5;
    casiTimes_vsdlss(uVar8);
    diffTimes_vsdlss(uVar7,uVar8,t_permuteA_vsdlss);
    local_c0 = time((time_t *)0x0);
    pFVar14 = (FILE *)getFilePtr_vsdlss(9);
    pcVar15 = ctime(&local_c0);
    fprintf(pFVar14,"Time Stamp at %s : %s","End   Factor Initialization",pcVar15);
    fflush(pFVar14);
    debugPrint_vsdlss("Factor initialized\n");
    if (local_d0 != 0) {
      finishAndScheduleParIntervals(local_d0,puVar13);
    }
    debugPrint_vsdlss("Start in-memory factorization\n");
    casiGetTimeOfDay();
    local_c0 = time((time_t *)0x0);
    pFVar14 = (FILE *)getFilePtr_vsdlss(9);
    pcVar15 = ctime(&local_c0);
    fprintf(pFVar14,"Time Stamp at %s : %s","Start in-memory Factorization",pcVar15);
    fflush(pFVar14);
    casiTimes_vsdlss(uVar7);
    lVar17 = newIVector_vsdlss((long)(int)puVar13[2],0,0);
    getRowToBlockVec_vsdlss(puVar13[5],*(undefined8 *)(puVar13 + 8),lVar17);
    if (((bVar26) && (casiParSolverF != 0)) && ((local_d0 != 0 && (1 < *(int *)(local_d0 + 4))))) {
      factorAOneChunkPar(lVar17,param_8,param_9,puVar13,local_d0,param_21);
    }
    else {
      factorAOneChunkNew_vsdlss(lVar17,param_8,param_9,puVar13);
    }
    if (lVar17 != 0) {
      casiFree_vsdlss(lVar17);
    }
    casiTimes_vsdlss(uVar8);
    diffTimes_vsdlss(uVar7,uVar8,t_factor_vsdlss);
    casiGetTimeOfDay(local_68);
    elapsedFactorTime = casiDiffTimeSecs(local_68,local_78);
    local_c0 = time((time_t *)0x0);
    pFVar14 = (FILE *)getFilePtr_vsdlss(9);
    pcVar15 = ctime(&local_c0);
    fprintf(pFVar14,"Time Stamp at %s : %s","End   in-memory Factorization",pcVar15);
    fflush(pFVar14);
    debugPrint_vsdlss("End   in-memory factorization\n");
    if (((bVar26) && (iVar4 = getReduceProblemSizeF_vsdlss(), iVar4 != 0)) &&
       ((local_d0 != 0 && (param_14 != 0)))) {
      iVar4 = (param_2 - *(int *)(local_d0 + 8)) * iVar2;
      iVar25 = param_2 + param_3 + param_4;
      uVar5 = getPppiTagSave();
      lVar17 = tagNewDVector_vsdlss(0,uVar5,(long)(iVar25 + iVar4),0);
      if (param_16 == 0) {
        copyVector_vsdlss(param_14,(long)iVar25,lVar17);
      }
      setPppiResultVecSave(lVar17);
      setPppiRedRhsResSave(lVar17 + ((long)param_4 + (long)param_3) * 8);
      setFbsParWorkVecsAndSize(lVar17 + (long)iVar25 * 8,iVar4);
      lVar17 = local_d0;
      uVar24 = getFbsParWorkVecs();
      uVar18 = getPppiRedRhsResSave();
      uVar19 = getPppiResultVecSave();
      uVar20 = getPppiReorderVecSave();
      uVar21 = getCpfSave();
      uVar22 = getPppiSave();
      setPppiInfoJrt(uVar5,uVar22,uVar21,uVar20,uVar19,uVar18,uVar24,CONCAT44(uVar29,iVar4),lVar17);
    }
    if ((param_8 == 0) || (param_16 != 0)) {
      if ((SINGLE_SOLVE_ONLY_vsdlss == '\0') || (bVar28)) {
        if (bVar26) {
          DAT_00e3d078 = *puVar13;
          DAT_00ecb050 = local_180;
          DAT_00ecb058 = puVar13;
          setFcInfoJrt_vsdlss(DAT_00e3d078,puVar13,local_180,DAT_00ecb048);
        }
        else {
          writeFactorOneChunk_vsdlss(puVar13);
          writeReorderVec_vsdlss(local_180,param_2);
        }
      }
    }
    else {
      makePartFactTrueCholesky_vsdlss(param_9,puVar13);
      renumWriteRedMat_vsdlss(param_9,puVar13);
      writeInvMapAsPerm_vsdlss(param_2,local_180);
      if (param_10 == 0) {
        writePartFactFromFc_vsdlss(param_9,puVar13);
        puVar13 = (undefined4 *)deleteFactorChunk_vsdlss(puVar13);
      }
      else {
        puVar23 = (undefined8 *)newVector_vsdlss(0,local_130,0);
        *puVar23 = 0;
        readDVectorFNumPermute_vsdlss(local_180,param_13,param_2,puVar23);
        solveLowerTriangForPartFact_vsdlss(param_9,puVar13,puVar23);
        puVar13 = (undefined4 *)deleteFactorChunk_vsdlss(puVar13);
        writeDVectorFNum_vsdlss(0x25,param_9,puVar23 + (param_2 - param_9));
        casiFree_vsdlss(puVar23);
      }
      if (local_e0 != 0) {
        casiFree_vsdlss();
      }
      local_e0 = 0;
    }
    if (local_d8 != (undefined1 *)0x0) {
      casiFree_vsdlss();
    }
    local_d8 = (undefined1 *)0x0;
    if (bVar26) {
      if ((longVecsInFC != 0) && (convertFcToLongValVecs_L(puVar13,local_d0), local_d0 != 0)) {
        recomputeStartEndBlocksForIntervals(local_d0,puVar13);
      }
      getReduceProblemSizeF_vsdlss();
    }
    if (param_22 != 0) {
      vsdlssStatus_vsdlss = 0;
      return 0;
    }
    if (iVar3 == 4) {
      genFcOffdiagValueStats(puVar13);
    }
    if (param_8 != 0 || param_16 != 0) goto LAB_00a13997;
    if (!bVar26) goto LAB_00a1372e;
LAB_00a1314d:
    local_d0 = getFbspi();
    iVar3 = getReduceProblemSizeF_vsdlss();
    if (iVar3 == 0) {
      if (local_d0 == 0) goto LAB_00a1373a;
      iVar2 = (param_2 - *(int *)(local_d0 + 8)) * iVar2;
      lVar17 = DAT_00ecb040;
      if (iVar1 == 0) {
        lVar17 = newVector_vsdlss(0,(long)(param_2 + iVar2),0);
      }
      setFbsParWorkVecsAndSize(lVar17 + (long)param_2 * 8,iVar2);
      goto LAB_00a13750;
    }
    lVar17 = getCpfSave();
    if (lVar17 == 0) {
      lVar16 = getPfReducedRhs_vsdlss();
    }
    else {
      lVar16 = getPppiRedRhsResSave();
    }
    lVar17 = lVar16;
    if (DAT_00ecb048 == 0) {
      lVar17 = param_14;
      if (param_14 == 0) {
        lVar17 = newVector_vsdlss(0,(long)param_2,0);
      }
      if (casiParSolverF == 0) {
        permuteDVector_vsdlss(local_180,param_2,lVar16,lVar17);
      }
      else {
        permuteDVectorPar();
      }
    }
  }
  debugPrint_vsdlss();
  local_c0 = time((time_t *)0x0);
  pFVar14 = (FILE *)getFilePtr_vsdlss(9);
  pcVar15 = ctime(&local_c0);
  fprintf(pFVar14,"Time Stamp at %s : %s","Start solve one load case",pcVar15);
  fflush(pFVar14);
  casiTimes_vsdlss(uVar7);
  casiGetTimeOfDay(local_b8);
  if ((local_d0 == 0) || (lVar16 = getFbsParWorkVecs(), lVar16 == 0)) {
    solveLoadCaseBlocked_vsdlss(puVar13,lVar17);
  }
  else {
    solveLoadCaseBlocked_mt(puVar13,local_d0,lVar17);
  }
  casiGetTimeOfDay(local_a8);
  elapsedLoadCaseSolveTime = casiDiffTimeSecs(local_a8,local_b8);
  casiTimes_vsdlss(uVar8);
  accumDiffTimes_vsdlss(uVar7,uVar8,t_lowerSolve_vsdlss);
  local_c0 = time((time_t *)0x0);
  pFVar14 = (FILE *)getFilePtr_vsdlss(9);
  pcVar15 = ctime(&local_c0);
  fprintf(pFVar14,"Time Stamp at %s : %s","End   solve one load case",pcVar15);
  fflush(pFVar14);
  debugPrint_vsdlss("End   solve one load case\n");
  if (iVar1 == 0) {
    deleteFactorChunk_vsdlss(puVar13);
    debugPrint_vsdlss();
    local_c0 = time((time_t *)0x0);
    pFVar14 = (FILE *)getFilePtr_vsdlss(9);
    pcVar15 = ctime(&local_c0);
    fprintf(pFVar14,"Time Stamp at %s : %s","Start Write/Save Reduced Results",pcVar15);
    fflush(pFVar14);
LAB_00a13c27:
    iVar2 = getReduceProblemSizeF_vsdlss();
    if (((iVar2 == 0) && (iVar1 != 0)) && (param_14 != 0)) {
      if (casiParSolverF == 0) {
        invPermuteDVector_vsdlss(local_180,param_2,lVar17,param_14);
      }
      else {
        invPermuteDVectorPar();
      }
    }
    else {
      invPermuteDVectorInPlace_vsdlss(local_180,param_2,lVar17);
      writeVecToFile_vsdlss(param_11,param_2,lVar17);
    }
    if (local_d0 == 0) {
      if (lVar17 != 0) {
        casiFree_vsdlss();
      }
      if (iVar1 == 0) goto LAB_00a13ca6;
    }
    else {
      setFbsParWorkVecsAndSize(0,0);
      if (iVar1 == 0) {
        if (lVar17 != 0) {
          casiFree_vsdlss(lVar17);
        }
LAB_00a13ca6:
        if (local_180 != 0) {
          casiFree_vsdlss();
        }
      }
    }
  }
  else {
    debugPrint_vsdlss();
    local_c0 = time((time_t *)0x0);
    pFVar14 = (FILE *)getFilePtr_vsdlss(9);
    pcVar15 = ctime(&local_c0);
    fprintf(pFVar14,"Time Stamp at %s : %s","Start Write/Save Reduced Results",pcVar15);
    fflush(pFVar14);
    iVar2 = getReduceProblemSizeF_vsdlss();
    if (iVar2 == 0) goto LAB_00a13c27;
    if (DAT_00ecb048 == 0) {
      lVar16 = getCpfSave();
      if (lVar16 == 0) {
        uVar24 = getPfReducedResult_vsdlss();
      }
      else {
        uVar24 = getPppiRedRhsResSave();
      }
      if (casiParSolverF == 0) {
        invPermuteDVector_vsdlss(local_180,param_2,lVar17,uVar24);
      }
      else {
        invPermuteDVectorPar();
      }
      if ((param_14 == 0) && (lVar17 != 0)) {
        casiFree_vsdlss();
      }
    }
  }
  local_c0 = time((time_t *)0x0);
  pFVar14 = (FILE *)getFilePtr_vsdlss(9);
  pcVar15 = ctime(&local_c0);
  fprintf(pFVar14,"Time Stamp at %s : %s","End   Write/Save Reduced Results",pcVar15);
  fflush(pFVar14);
  debugPrint_vsdlss("End   writing/saving result file\n");
LAB_00a13997:
  casiTimes_vsdlss(uVar10);
  diffTimes_vsdlss(uVar9,uVar10,t_reducedSolve_vsdlss);
  deleteTime_vsdlss(uVar7);
  deleteTime_vsdlss(uVar8);
  deleteTime_vsdlss(uVar9);
  deleteTime_vsdlss(uVar10);
  deleteTime_vsdlss(uVar11);
  deleteTime_vsdlss(uVar12);
  casiGetTimeOfDay(local_88);
  elapsedReducedSolveTime = casiDiffTimeSecs(local_88,local_98);
  vsdlssStatus_vsdlss = 0;
  debugPrint_vsdlss("********** vsdlss1mem ended with %d status code **********\n",0);
  return vsdlssStatus_vsdlss;
}


