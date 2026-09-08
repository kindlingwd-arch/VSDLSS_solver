/* VSDLSS function (decompiled by Ghidra) */
/* name: vsdlss  addr: 009dad10  size: 1fb5 */
#include "vsdlss_ref.h"

/* signature: uint vsdlss(undefined8 param_1,char *param_2,int param_3,int param_4,int param_5,int param_6,undefined4 param_7,undefined8 param_8,int param_9,int param_10,int param_11,char param_12,undefined8 param_13,int param_14,int param_15,undefined8 param_16,undefined8 param_17,undefined4 param_18,undefined8 param_19,undefined8 param_20,undefined8 param_21,undefined8 param_22,undefined8 param_23,undefined8 param_24,undefined8 param_25,undefined8 param_26,undefined8 param_27,undefined8 param_28,int param_29,int param_30,int param_31,int param_32,undefined8 param_33,undefined8 param_34,int param_35,int param_36,int param_37); */

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint vsdlss(undefined8 param_1,char *param_2,int param_3,int param_4,int param_5,int param_6,
           undefined4 param_7,undefined8 param_8,int param_9,int param_10,int param_11,char param_12
           ,undefined8 param_13,int param_14,int param_15,undefined8 param_16,undefined8 param_17,
           undefined4 param_18,undefined8 param_19,undefined8 param_20,undefined8 param_21,
           undefined8 param_22,undefined8 param_23,undefined8 param_24,undefined8 param_25,
           undefined8 param_26,undefined8 param_27,undefined8 param_28,int param_29,int param_30,
           int param_31,int param_32,undefined8 param_33,undefined8 param_34,int param_35,
           int param_36,int param_37)

{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  FILE *pFVar10;
  char *pcVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  int *piVar15;
  undefined4 *puVar16;
  long lVar17;
  long lVar18;
  int iVar19;
  ulong uVar20;
  bool bVar21;
  bool bVar22;
  bool bVar23;
  double dVar24;
  undefined8 in_stack_fffffffffffffda8;
  ulong in_stack_fffffffffffffdb0;
  undefined4 uVar25;
  undefined8 in_stack_fffffffffffffdb8;
  undefined8 in_stack_fffffffffffffdc0;
  undefined4 uVar26;
  undefined8 in_stack_fffffffffffffdd8;
  int local_198;
  uint local_17c;
  uint local_16c;
  long local_168;
  byte local_159;
  long local_158;
  undefined1 *local_140;
  int local_108;
  int local_104;
  int local_100;
  int local_fc;
  time_t local_f8;
  time_t local_f0;
  time_t local_e8;
  int *local_e0;
  undefined1 local_d8 [16];
  undefined1 local_c8 [16];
  undefined1 local_b8 [16];
  undefined8 local_a8 [2];
  undefined1 local_98 [4];
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined1 local_78 [24];
  undefined8 local_60;
  undefined8 local_50;
  undefined8 local_48;
  undefined8 local_40;
  
  uVar8 = (undefined4)((ulong)in_stack_fffffffffffffdb8 >> 0x20);
  uVar25 = (undefined4)((ulong)in_stack_fffffffffffffda8 >> 0x20);
  uVar26 = (undefined4)((ulong)in_stack_fffffffffffffdc0 >> 0x20);
  uVar9 = (undefined4)((ulong)in_stack_fffffffffffffdd8 >> 0x20);
  local_fc = 0;
  local_e0 = (int *)0x0;
  setNumThreadsInBlasLib(1);
  initCasiMutexes();
  casiGetTimeOfDay(local_b8);
  initTimerPool();
  bVar21 = param_14 - 3U < 2;
  if (bVar21) {
    param_14 = param_14 >> 1;
  }
  local_17c = (uint)!bVar21;
  appMallocFunPtr_vsdlss = param_20;
  appFreeFunPtr_vsdlss = param_21;
  DAT_00e96564 = param_3;
  vsdlssStatus_vsdlss = _setjmp((__jmp_buf_tag *)vsdlssEnv_vsdlss);
  if (vsdlssStatus_vsdlss != 0) {
    fprintf(stderr,"Level:%d\n%s\n",(ulong)vsdlssStatus_vsdlss,vsdlssMessage_vsdlss);
    if (casiParSolverF != 0) {
      finishCasiParScheduler();
    }
    finishCasiParSolver();
    iVar4 = fileExists_vsdlss(6);
    if (iVar4 != 0) {
      uVar12 = readChunkInfo_vsdlss();
      removeAllChunkFiles_vsdlss(uVar12);
      deleteChunkInfo_vsdlss(uVar12);
    }
    closeAllFiles_vsdlss();
    removeSemiPermFiles_vsdlss();
    removeTempFiles_vsdlss();
    cleanUpMemory_vsdlss();
    deleteAllSemiPermTags_vsdlss();
    finishCasiMutexes();
    return vsdlssStatus_vsdlss;
  }
  setReduceProblemSizeF_vsdlss(param_9);
  MATRIX_CHANGED_vsdlss = param_5 != 2;
  SINGLE_SOLVE_ONLY_vsdlss = param_5 == 0;
  ONLY_MATRIX_VALUES_CHANGED = (uint)(param_5 == 4);
  uVar5 = enqDenseSolve_vsdlss();
  local_16c = uVar5 >> 0x1f;
  if (param_10 == 0 && param_36 == 0) {
    local_159 = param_3 <= (int)uVar5 | (byte)(uVar5 >> 0x1f);
    if (local_159 == 0) {
      bVar2 = false;
    }
    else {
      MATRIX_CHANGED_vsdlss = '\x01';
      SINGLE_SOLVE_ONLY_vsdlss = '\x01';
      bVar2 = true;
    }
  }
  else {
    bVar2 = false;
    local_159 = 0;
    local_16c = 0;
  }
  uVar20 = (ulong)param_3;
  deg1NodesInProbOrig_vsdlss = 0;
  deg2NodesInProbOrig_vsdlss = 0;
  deg3NodesInProbOrig_vsdlss = 0;
  varsInProb_vsdlss = 0;
  nodesInProb_vsdlss = 0;
  iVar4 = param_3 + 0xf;
  if (-1 < param_3) {
    iVar4 = param_3;
  }
  maxChunkSize_vsdlss = 0;
  elemsInProb_vsdlss = 0;
  avgChunkSize_vsdlss = 0;
  if (0x100000 < param_3) {
    uVar20 = 0x100000;
  }
  factorIndexSize_vsdlss = 0;
  factorValueSize_vsdlss = 0;
  blocksInProb_vsdlss = 0;
  READ_BUFF_SIZE = (long)(iVar4 >> 4);
  if ((ulong)(long)(iVar4 >> 4) < uVar20) {
    READ_BUFF_SIZE = uVar20;
  }
  factorSize_vsdlss = 0;
  factorNZ_vsdlss = 0;
  chunksInProb_vsdlss = 0;
  uVar20 = 0x100000;
  if (param_4 < 0x100001) {
    uVar20 = (long)param_4;
  }
  opsToFactor_vsdlss = 0;
  upperANZ_vsdlss = 0;
  opsForTriangSolves_vsdlss = 0;
  maxLevelInRCM_vsdlss = 0;
  dgemmOpCnt = 0;
  numSeparatorsInProb_vsdlss = 0;
  factorWithinBlockOpCnt = 0;
  initOrderSelectedInProb_vsdlss = 0;
  elapsedReorderTime = 0;
  elapsedFactorTime = 0;
  elapsedReducedSolveTime = 0;
  elapsedPardisoReorderTime = 0;
  elapsedPardisoFactorTime = 0;
  elapsedPardisoLoadCaseSolveTime = 0;
  iVar4 = param_4 + 0xf;
  if (-1 < param_4) {
    iVar4 = param_4;
  }
  if (uVar20 <= (ulong)(long)(iVar4 >> 4)) {
    uVar20 = (long)(iVar4 >> 4);
  }
  varsInProbOrig_vsdlss = param_3;
  elemsInProbOrig_vsdlss = param_4;
  nodesInProbOrig_vsdlss = param_3;
  setFtrwCHUNKSIZE_ITEMS(uVar20);
  setCmuCHUNK_WRITE_CHUNKSIZE(uVar20);
  initOutOfCore_vsdlss(param_6);
  if (param_10 != 0 || param_36 != 0) {
    setOutOfCore_vsdlss(0);
  }
  iVar4 = enqOutOfCore_vsdlss();
  if ((iVar4 == 0) &&
     ((((param_10 == 0 || (param_31 != 0)) && (param_12 != 'L')) && (param_12 != 'U')))) {
    initSolveInMem_vsdlss(param_14);
    initRedProbInMem(bVar21 && param_9 != 0);
  }
  else {
    setSolveInMem_vsdlss(0);
    setRedProbInMem(0);
  }
  iVar6 = enqSolveInMem_vsdlss();
  enqRedProbInMem();
  enqLongVecsInFC();
  bVar21 = iVar6 != 0;
  longVecsInFC = 0;
  bVar22 = param_10 != 0;
  if ((bVar21) && (bVar22)) {
    if (param_31 == 0) {
      initOrderSelector_vsdlss(param_15);
LAB_009db473:
      setOrderSelector_vsdlss(1);
      bVar1 = true;
      bVar3 = false;
    }
    else {
      initOrderSelector_vsdlss(param_15);
      bVar1 = false;
      bVar3 = true;
    }
  }
  else {
    initOrderSelector_vsdlss(param_15);
    bVar1 = true;
    bVar3 = false;
    if (bVar22) goto LAB_009db473;
  }
  casiActivatePardisoF = enqActivatePardiso();
  initThreadsInPool(param_35);
  inWorkerThreadsF = 0;
  casiParSolverF = 0;
  iVar7 = getSeqParSelector();
  if (iVar7 == 1) {
    casiParSolverF = 1;
  }
  if (DAT_00e3bd60 == 0) {
    if (iVar6 != DAT_00e96594) {
      nrerror_vsdlss(0x30,"vsdlss.c",0x2c1,iVar6,iVar6 + 2,DAT_00e96594,
                     CONCAT44(uVar25,DAT_00e96594 + 2));
    }
    if (param_35 != DAT_00e96590) {
      nrerror_vsdlss(0x46,"vsdlss.c",0x2ca,param_35);
    }
    iVar7 = strcmp(jobName_vsdlss,param_2);
    if (iVar7 == 0) {
      if (((MATRIX_CHANGED_vsdlss != '\0') || (param_31 != 0)) && (ONLY_MATRIX_VALUES_CHANGED == 0))
      {
        deleteJobFromJrtCleanUp_vsdlss();
        setPfInfoPtrsToNull_vsdlss();
        setFcInfoPtrsToNull_vsdlss();
        setPppiInfoPtrsToNull();
      }
    }
    else {
      if (param_31 != 0) {
        deleteJobFromJrtCleanUp_vsdlss();
      }
      if (iVar6 == 1) {
        uVar12 = getMemAllocBlockSize_vsdlss();
        initCasiMallocFree_vsdlss(uVar12);
        saveJobToDiskFreeMemoryJrt_vsdlss();
        setPfInfoPtrsToNull_vsdlss();
        setFcInfoPtrsToNull_vsdlss();
        setPppiInfoPtrsToNull();
        cleanUpMemory_vsdlss();
      }
    }
  }
  else {
    DAT_00e3bd60 = 0;
    DAT_00e96590 = param_35;
    DAT_00e96594 = iVar6;
    initJobReloadManager_vsdlss();
  }
  bVar23 = param_31 == 0;
  if ((((bVar21) && (bVar23)) && (param_36 == 0)) && (local_168 = getRhsResultVec(), local_168 != 0)
     ) {
    local_168 = local_168 + -8;
  }
  else {
    local_168 = 0;
  }
  initStaticVars_vsdlss();
  dVar24 = ((double)param_4 + (double)param_4) / (double)param_3;
  if (dVar24 < DAT_00ab72f0) {
    if (_DAT_00b19718 <= dVar24) {
      dVar24 = _DAT_00ab67f8 - (DAT_00ab72f0 - dVar24) * DAT_00b11d60;
      INIT_ALLOC_MULT_vsdlss = (dVar24 + dVar24) * DAT_00b19720;
    }
    else {
      INIT_ALLOC_MULT_vsdlss = DAT_00b19720;
    }
  }
  else {
    INIT_ALLOC_MULT_vsdlss = DAT_00ab72a0;
  }
  NUMERICAL_ZERO_vsdlss = DAT_00ac53f8;
  ZeroPivot_VAL_vsdlss = _DAT_00b19728;
  strcpy(jobName_vsdlss,param_2);
  getBaseJobName_vsdlss(jobName_vsdlss,param_16);
  initFManager_vsdlss(jobName_vsdlss,param_17);
  if (param_3 < 1) {
    nrerror_vsdlss(0x1f,"vsdlss.c",0x312,param_3);
  }
  callInitMemManager_vsdlss(param_3,param_5);
  openFileA_vsdlss();
  if ((-1 < param_5) && (initCasiParSolver(), casiParSolverF != 0)) {
    initCasiParScheduler();
  }
  if (ONLY_MATRIX_VALUES_CHANGED != 0) {
    saveReorderInfo();
    deleteJobFromJrtCleanUp_vsdlss();
    setPfInfoPtrsToNull_vsdlss();
    setFcInfoPtrsToNull_vsdlss();
    setPppiInfoPtrsToNull();
  }
  if ((bVar21) && (param_5 == 2)) {
    loadJobFromDiskJrt_vsdlss();
    setPfAndFcInfoPtrs_vsdlss();
  }
  if (param_12 == 'L') {
    uVar12 = 0;
LAB_009db606:
    readAndSolvePartDataPartFact_vsdlss(jobName_vsdlss,param_11,uVar12);
    closeAllFiles_vsdlss();
    cleanUpMemory_vsdlss();
    finishCasiMutexes();
    vsdlssStatus_vsdlss = 0;
    return 0;
  }
  if (param_12 == 'U') {
    uVar12 = 1;
    goto LAB_009db606;
  }
  local_f8 = time((time_t *)0x0);
  pFVar10 = (FILE *)getFilePtr_vsdlss(9);
  pcVar11 = ctime(&local_f8);
  fprintf(pFVar10,"Time Stamp at %s : %s","Start",pcVar11);
  fflush(pFVar10);
  uVar25 = (undefined4)(in_stack_fffffffffffffdb0 >> 0x20);
  if (param_5 < 0) {
    if ((casiActivatePardisoF != 0) && (iVar4 = getPardisoAlreadyCalledF(), iVar4 != 0)) {
      casiPardisoMem(0,0,0,0,1,1,0,in_stack_fffffffffffffdb0 & 0xffffffff00000000,0);
    }
    iVar4 = fileExists_vsdlss(6);
    if (iVar4 != 0) {
      uVar12 = readChunkInfo_vsdlss();
      removeAllChunkFiles_vsdlss(uVar12);
      deleteChunkInfo_vsdlss(uVar12);
    }
    if (casiParSolverF != 0) {
      finishCasiParScheduler();
    }
    finishCasiParSolver();
    closeAllFiles_vsdlss();
    removeSemiPermFiles_vsdlss();
    removeTempFiles_vsdlss();
    cleanUpMemory_vsdlss();
    if (bVar21) {
      deleteJobFromJrtCleanUp_vsdlss();
      setPfInfoPtrsToNull_vsdlss();
      setFcInfoPtrsToNull_vsdlss();
      setPppiInfoPtrsToNull();
    }
    vsdlssStatus_vsdlss = 0;
    finishCasiMutexes();
    return vsdlssStatus_vsdlss;
  }
  uVar13 = newTime_vsdlss();
  uVar14 = newTime_vsdlss();
  t_prepostproc_vsdlss = newTime_vsdlss();
  t_reducedSolve_vsdlss = newTime_vsdlss();
  t_minDegree_vsdlss = newTime_vsdlss();
  t_divideIntoChunks_vsdlss = newTime_vsdlss();
  t_reorder_vsdlss = newTime_vsdlss();
  t_symElim_vsdlss = newTime_vsdlss();
  t_permuteA_vsdlss = newTime_vsdlss();
  t_factor_vsdlss = newTime_vsdlss();
  t_factorIO_vsdlss = newTime_vsdlss();
  t_lowerSolve_vsdlss = newTime_vsdlss();
  t_lowerSolveIO_vsdlss = newTime_vsdlss();
  t_upperSolve_vsdlss = newTime_vsdlss();
  t_upperSolveIO_vsdlss = newTime_vsdlss();
  t_work1_vsdlss = newTime_vsdlss();
  t_work2_vsdlss = newTime_vsdlss();
  t_work3_vsdlss = newTime_vsdlss();
  t_work4 = newTime_vsdlss();
  t_work5 = newTime_vsdlss();
  t_dgemm = newTime_vsdlss();
  t_subBlockContribFromRowRange = newTime_vsdlss();
  t_factorWithinBlock_vsdlss = newTime_vsdlss();
  t_factorKernel_vsdlss = newTime_vsdlss();
  setFileName_vsdlss(0x19,param_19);
  setFileName_vsdlss(0x1a,param_22);
  setFileName_vsdlss(0x1b,param_23);
  setFileName_vsdlss(0x1c,param_24);
  setFileName_vsdlss(0x1d,param_25);
  setFileName_vsdlss(0x29,param_28);
  setFileName_vsdlss(0x1e,param_26);
  setFileName_vsdlss(0x27,param_33);
  setFileName_vsdlss(0x28,param_34);
  beginLog_vsdlss();
  uVar12 = param_8;
  printHeaderToLog_vsdlss
            (param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,
             CONCAT44(uVar25,param_9),CONCAT44(uVar8,param_10),CONCAT44(uVar26,param_11),param_13,
             param_14,CONCAT44(uVar9,param_15),param_18,param_19,param_22,param_23,param_24,param_25
             ,param_26,local_168,param_27,param_29,param_30,param_31,param_33,param_34,param_36,
             param_37);
  uVar9 = (undefined4)((ulong)uVar12 >> 0x20);
  uVar26 = (undefined4)((ulong)param_13 >> 0x20);
  if (param_6 != iVar4) {
    printToLog_vsdlss("Out-of-core flag %d from %s.hdr is changed to %d by file outofcore.cas\n",
                      param_6,param_2,iVar4);
  }
  if (param_14 != iVar6) {
    printToLog_vsdlss("Solve-In_Memory flag %d from %s.hdr is changed to %d by file solveinmem.cas\n"
                      ,param_14,param_2,iVar6);
  }
  iVar7 = enqOrderSelector_vsdlss();
  if (iVar7 != param_15) {
    uVar8 = enqOrderSelector_vsdlss();
    printToLog_vsdlss("Order Selector %d from %s.hdr is changed to %d by file orderselector.cas\n",
                      param_15,param_2,uVar8);
  }
  if ((param_30 - 1U < 2) && (!bVar3)) {
    if (MATRIX_CHANGED_vsdlss != '\0') {
      uVar12 = newElemArrHeaderOnly_vsdlss(param_4,param_3);
      readMatrixFromFilePrintToLog_vsdlss(uVar12);
      deleteElemArr_vsdlss(uVar12);
    }
    if (bVar22) {
      puVar16 = (undefined4 *)newIVector_vsdlss((long)param_11,0,0);
      *puVar16 = 0;
      readIVectorFNum_vsdlss(0x19,param_11,puVar16);
      printIVectorToLog_vsdlss("rset Vector",puVar16,param_11);
      casiFree_vsdlss(puVar16);
    }
    if (local_168 == 0) {
      iVar7 = fileExists_vsdlss(0x1e);
      if ((iVar7 != 0) && (param_36 == 0 && param_31 == 0)) {
        lVar17 = newVector_vsdlss(0,(long)param_3,0);
        readVecFromFile_vsdlss(param_26,param_3,lVar17);
        printDVectorToLog_vsdlss("RHS Vector",lVar17,param_3);
        if (lVar17 != 0) {
          casiFree_vsdlss(lVar17);
        }
      }
    }
    else {
      printDVectorToLog_vsdlss("RHS Vector",local_168,param_3);
    }
    if (param_30 == 1) {
      casiWarning_vsdlss(0x20,"vsdlss.c",0x3ee);
      goto LAB_009dbb29;
    }
  }
  casiGetTimeOfDay(local_d8);
  casiTimes_vsdlss(uVar13);
  if ((bVar1) && (local_159 != 0)) {
    uVar12 = newElemArr_vsdlss(param_4,param_3);
    readMatrixFromFileAndCheck_vsdlss(uVar12);
    handleIsolatedVarsInEa_vsdlss(uVar12);
    lVar17 = newVector_vsdlss(0,(long)param_3,1);
    if (local_168 == 0) {
      if (param_36 == 0 && param_31 == 0) {
        readVecFromFile_vsdlss(param_26,param_3,lVar17);
      }
    }
    else {
      copyVector_vsdlss(local_168,(long)param_3,lVar17);
    }
    local_158 = denseSolve_vsdlss(uVar12,lVar17);
    deleteElemArr_vsdlss(uVar12);
    if (lVar17 != 0) {
      casiFree_vsdlss(lVar17);
    }
  }
  else {
    local_158 = 0;
  }
  if (local_16c != 0) {
    if (local_168 == 0) {
      writeVecToFile_vsdlss(param_27,param_3,local_158);
      if (param_30 - 2U < 2) {
        printDVectorToLog_vsdlss("Result Vector",local_158,param_3);
      }
    }
    else {
      copyVector_vsdlss(local_158,(long)param_3);
      if (param_30 - 2U < 2) {
        printDVectorToLog_vsdlss("Result Vector",local_168,param_3);
      }
    }
    if (local_158 != 0) {
      casiFree_vsdlss();
    }
    endLog_vsdlss();
LAB_009dbb29:
    closeAllFiles_vsdlss();
    removeSemiPermFiles_vsdlss();
    removeTempFiles_vsdlss();
    cleanUpMemory_vsdlss();
    deleteAllSemiPermTags_vsdlss();
    finishCasiMutexes();
    vsdlssStatus_vsdlss = 0;
    return 0;
  }
  if (MATRIX_CHANGED_vsdlss == '\0') {
    local_140 = (undefined1 *)0x0;
    local_198 = 0;
  }
  else {
    if (bVar21) {
      deleteJobFromJrtCleanUp_vsdlss();
      insertJobIntoJrt_vsdlss();
      setPfInfoPtrsToNull_vsdlss();
      setFcInfoPtrsToNull_vsdlss();
      setPppiInfoPtrsToNull();
    }
    iVar7 = getReduceProblemSizeF_vsdlss();
    if (iVar7 == 0) {
      deg1NodesInProbOrig_vsdlss = 0;
      deg2NodesInProbOrig_vsdlss = 0;
      deg3NodesInProbOrig_vsdlss = 0;
      local_94 = 0x1a;
      local_90 = 0x1b;
      local_8c = 0x1c;
      local_88 = 0x1d;
      DAT_00e96570 = 0;
      DAT_00e9656c = 0;
      DAT_00e96568 = 0;
      local_140 = (undefined1 *)0x0;
      local_198 = param_4;
      if (param_36 != 0) {
        lVar17 = (long)param_4;
        initElemArrHeader(local_78,param_4,param_3);
        local_60 = newIVector_vsdlss(lVar17,0,0);
        local_50 = newIVector_vsdlss(lVar17,0,0);
        if ((param_37 == 0) || (iVar7 = enqGenMatStats_vsdlss(), iVar7 != 0)) {
          local_48 = newFVector_vsdlss(0,lVar17,0);
          local_40 = newFVector_vsdlss(0,(long)param_3,0);
        }
        else {
          local_48 = 0;
          local_40 = 0;
        }
        fillEaVectorsHmslss(param_3,param_4,local_60,local_50,local_48,local_40,
                            CONCAT44(uVar9,param_37));
        local_a8[0] = 0;
        getAtEndListSave(&local_fc,local_a8);
        if (local_fc < 1) {
          local_fc = 0;
          local_e0 = (int *)0x0;
        }
        else {
          local_e0 = (int *)newIVector_vsdlss((long)local_fc,0,0);
          copyIVector_vsdlss(local_a8[0],(long)local_fc,local_e0);
        }
        local_140 = local_78;
      }
    }
    else {
      if (param_31 == 0) {
LAB_009dc82f:
        lVar17 = 0;
        iVar7 = 0;
      }
      else {
        if (!bVar3) {
          readAtEndList(0x27,param_3,&local_fc,&local_e0);
          goto LAB_009dc82f;
        }
        lVar17 = readMatrixA12_vsdlss(param_3,param_32);
        putInAtleastOneNZInA12(lVar17);
        if (*(int *)(lVar17 + 0x10) < 1) {
          iVar7 = 0;
        }
        else {
          piVar15 = (int *)(*(long *)(lVar17 + 0x30) + 4);
          iVar7 = 0;
          do {
            iVar7 = iVar7 + *piVar15;
            piVar15 = piVar15 + 1;
          } while (piVar15 !=
                   (int *)(*(long *)(lVar17 + 0x30) + 8 + (ulong)(*(int *)(lVar17 + 0x10) - 1) * 4))
          ;
        }
        if (param_11 != param_32) {
          nrerror_vsdlss(1,"vsdlss.c",0x44c);
        }
        local_fc = param_32;
        local_e0 = (int *)newIVector_vsdlss((long)param_32,0,0);
        if (0 < local_fc) {
          iVar19 = (param_3 - local_fc) + 1;
          piVar15 = local_e0;
          do {
            piVar15 = piVar15 + 1;
            *piVar15 = iVar19;
            iVar19 = iVar19 + 1;
          } while (iVar19 != (param_3 - local_fc) + 1 + local_fc);
        }
      }
      if (bVar21) {
        local_140 = local_78;
        local_198 = reduceProblemSizeInMem
                              (param_3,param_4,local_fc,local_e0,lVar17,iVar7,&local_108,&local_104,
                               &local_100,local_140,CONCAT44(uVar26,local_17c));
        if (bVar3) {
          deleteSparseFactor_vsdlss(lVar17);
        }
      }
      else {
        local_198 = reduceProblemSize_vsdlss
                              (param_3,param_4,local_fc,local_e0,&local_108,&local_104,&local_100);
        local_140 = (undefined1 *)0x0;
      }
      DAT_00e96570 = local_108;
      local_94 = 0x14;
      local_90 = 0x15;
      local_8c = 0x16;
      local_88 = 0x17;
      DAT_00e9656c = local_104 - local_108;
      DAT_00e96568 = local_100;
      if (local_100 + local_104 == param_3) {
        setPppiInfoPtrsToNull();
      }
    }
  }
  uVar9 = 0;
  if (bVar23) {
    iVar7 = getReduceProblemSizeF_vsdlss();
    if (iVar7 == 0) {
      local_104 = 0;
      local_100 = 0;
      uVar9 = 0x1e;
    }
    else {
      if (bVar21) {
        if (MATRIX_CHANGED_vsdlss != '\0') {
          uVar9 = getPfTag_vsdlss();
          uVar12 = tagNewDVector_vsdlss(0,uVar9,(long)param_3,0);
          setPfResAndRhsVecs(uVar12);
        }
        lVar17 = getCpfSave();
        if (lVar17 != 0) {
          preSolveCpf(param_26,local_168,param_3,&local_104,&local_100);
          uVar9 = 5;
          goto LAB_009dbc6d;
        }
      }
      preSolve_vsdlss(param_26,local_168,param_3,&local_104,&local_100);
      uVar9 = 5;
    }
  }
LAB_009dbc6d:
  casiTimes_vsdlss(uVar14);
  accumDiffTimes_vsdlss(uVar13,uVar14,t_prepostproc_vsdlss);
  casiGetTimeOfDay(local_c8);
  elapsedPrePostProcTime = (double)casiDiffTimeSecs(local_c8,local_d8);
  local_e8 = time((time_t *)0x0);
  pFVar10 = (FILE *)getFilePtr_vsdlss(9);
  pcVar11 = ctime(&local_e8);
  fprintf(pFVar10,"Time Stamp at %s : %s","Before Solving Reduced Problem",pcVar11);
  fflush(pFVar10);
  if (iVar4 == 0) {
    if (casiActivatePardisoF == 0) {
      vsdlssStatus_vsdlss = vsdlss1Mem_vsdlss(param_1);
    }
    else {
      vsdlssStatus_vsdlss =
           casiPardisoMem((param_3 - local_104) - local_100,local_198,local_98,local_140,1,0,
                          param_27,uVar9,local_168);
    }
  }
  else {
    vsdlssStatus_vsdlss =
         vsdlss1_vsdlss(param_1,(param_3 - local_104) - local_100,local_198,param_7,param_8,param_27
                        ,local_98,uVar9,param_29);
  }
  local_e8 = time((time_t *)0x0);
  pFVar10 = (FILE *)getFilePtr_vsdlss(9);
  pcVar11 = ctime(&local_e8);
  fprintf(pFVar10,"Time Stamp at %s : %s","After  Solving Reduced Problem",pcVar11);
  fflush(pFVar10);
  if (vsdlssStatus_vsdlss != 0) {
                    /* WARNING: Subroutine does not return */
    longjmp((__jmp_buf_tag *)vsdlssEnv_vsdlss,vsdlssStatus_vsdlss);
  }
  if ((bVar3 || param_36 != 0) && (0 < local_fc)) {
    local_fc = 0;
    if (local_e0 != (int *)0x0) {
      casiFree_vsdlss();
    }
    local_e0 = (int *)0x0;
  }
  casiGetTimeOfDay(local_d8);
  casiTimes_vsdlss(uVar13);
  if ((bVar23) && (iVar4 = enqOrderOnly_vsdlss(), iVar4 == 0)) {
    if (bVar22) {
      if (param_30 - 2U < 2) {
        printPartFactDetailsToLog_vsdlss(param_3,param_11,param_18);
      }
      goto LAB_009dbe47;
    }
    iVar4 = getReduceProblemSizeF_vsdlss();
    bVar22 = false;
    if (iVar4 == 0) {
      if ((bVar2) || (param_30 - 2U < 2)) {
        lVar17 = newVector_vsdlss(0,(long)param_3,0);
        if (local_168 == 0) {
          readVecFromFile_vsdlss(param_27,param_3,lVar17);
          if (bVar2) goto LAB_009dccda;
          if (1 < param_30 - 2U) goto LAB_009dcb60;
          goto LAB_009dcd27;
        }
        copyVector_vsdlss(local_168,(long)param_3);
        if (bVar2) goto LAB_009dcb1f;
        if (param_30 - 2U < 2) goto LAB_009dcc60;
        goto LAB_009dcb60;
      }
      if (iVar6 == 0) goto LAB_009dbe47;
    }
    else {
      if (bVar21) {
        lVar17 = getCpfSave();
        if (lVar17 == 0) {
          lVar17 = getPfDeg1Deg2PartRes_vsdlss();
        }
        else {
          lVar17 = getPppiResultVecSave();
        }
        lVar18 = getCpfSave();
        if (lVar18 == 0) goto LAB_009dcc1d;
        postSolveCpf(param_27,local_168,param_3,&local_104,&local_100,lVar17);
      }
      else {
        lVar17 = newVector_vsdlss(0,(long)param_3,0);
LAB_009dcc1d:
        postSolve_vsdlss(param_27,local_168,param_3,&local_104,&local_100,lVar17);
      }
      if (bVar2) {
        if (local_168 == 0) {
LAB_009dccda:
          checkResults_vsdlss(param_3,lVar17,local_158);
        }
        else {
LAB_009dcb1f:
          checkResults_vsdlss(param_3,local_168,local_158);
        }
        if (local_158 != 0) {
          casiFree_vsdlss();
        }
      }
      if (param_30 - 2U < 2) {
        if (local_168 == 0) {
LAB_009dcd27:
          printDVectorToLog_vsdlss("Result Vector",lVar17,param_3);
        }
        else {
LAB_009dcc60:
          printDVectorToLog_vsdlss("Result Vector",local_168,param_3);
        }
      }
LAB_009dcb60:
      if (iVar6 == 0) {
        if (lVar17 != 0) {
          casiFree_vsdlss(lVar17);
        }
        goto LAB_009dbe47;
      }
    }
  }
  else {
LAB_009dbe47:
    bVar22 = iVar6 == 0 && param_31 != 0;
    if (bVar22) {
      renameOutputFilesForUWrapper();
    }
  }
  casiTimes_vsdlss(uVar14);
  accumDiffTimes_vsdlss(uVar13,uVar14,t_prepostproc_vsdlss);
  casiGetTimeOfDay(local_c8);
  dVar24 = (double)casiDiffTimeSecs(local_c8,local_d8);
  elapsedPrePostProcTime = dVar24 + elapsedPrePostProcTime;
  casiGetTimeOfDay(local_a8);
  elapsedTotalTime = casiDiffTimeSecs(local_a8,local_b8);
  if (param_29 != 0) {
    printStats_vsdlss();
  }
  local_f0 = time((time_t *)0x0);
  pFVar10 = (FILE *)getFilePtr_vsdlss(9);
  pcVar11 = ctime(&local_f0);
  fprintf(pFVar10,"Time Stamp at %s : %s",&DAT_00b190af,pcVar11);
  fflush(pFVar10);
  endLog_vsdlss();
  if (SINGLE_SOLVE_ONLY_vsdlss == '\0') {
    closeAllFiles_vsdlss();
    if ((SINGLE_SOLVE_ONLY_vsdlss == '\0') || (!bVar23)) goto LAB_009dbf60;
LAB_009dc2c0:
    if (bVar21) {
      deleteJobFromJrtCleanUp_vsdlss();
      deleteAllSemiPermTags_vsdlss();
      setPfInfoPtrsToNull_vsdlss();
      setFcInfoPtrsToNull_vsdlss();
      setPppiInfoPtrsToNull();
      goto code_r0x009dbf70;
    }
  }
  else {
    if (bVar23) {
      if (casiParSolverF != 0) {
        finishCasiParScheduler();
      }
      finishCasiParSolver();
      closeAllFiles_vsdlss();
      if (SINGLE_SOLVE_ONLY_vsdlss != '\0') goto LAB_009dc2c0;
    }
    else {
      closeAllFiles_vsdlss();
    }
LAB_009dbf60:
    if (!bVar22) goto code_r0x009dbf70;
  }
  removeSemiPermFiles_vsdlss();
  deleteAllSemiPermTags_vsdlss();
code_r0x009dbf70:
  removeTempFiles_vsdlss();
  casiFClose_vsdlss(10);
  casiFClose_vsdlss(9);
  cleanUpMemory_vsdlss();
  finishCasiMutexes();
  return vsdlssStatus_vsdlss;
}


