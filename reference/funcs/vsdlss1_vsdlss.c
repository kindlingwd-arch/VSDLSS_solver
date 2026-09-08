/* VSDLSS function (decompiled by Ghidra) */
/* name: vsdlss1_vsdlss  addr: 00a1b110  size: d21 */
#include "vsdlss_ref.h"

/* signature: undefined4 vsdlss1_vsdlss(double param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined4 param_8); */

undefined4
vsdlss1_vsdlss(double param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined8 param_5,
              undefined8 param_6,undefined8 param_7,undefined4 param_8)

{
  int iVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  FILE *pFVar9;
  char *pcVar10;
  long lVar11;
  undefined8 uVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  undefined8 uVar16;
  long local_70;
  long local_68;
  long local_60;
  undefined8 local_58;
  long local_50;
  long local_48;
  time_t local_40 [2];
  
  if (param_2 == 0) {
LAB_00a1b136:
    vsdlssStatus_vsdlss = 0;
  }
  else {
    elemsInProb_vsdlss = 0;
    upperANZ_vsdlss = 0;
    varsInProb_vsdlss = param_2;
    nodesInProb_vsdlss = param_2;
    uVar3 = newTime_vsdlss();
    uVar4 = newTime_vsdlss();
    uVar5 = newTime_vsdlss();
    uVar6 = newTime_vsdlss();
    uVar7 = newTime_vsdlss();
    uVar8 = newTime_vsdlss();
    debugPrint_vsdlss("********** Starting vsdlss1 **********\n");
    casiTimes_vsdlss();
    if (MATRIX_CHANGED_vsdlss == '\0') {
      lVar13 = readChunkInfo_vsdlss();
      lVar11 = readBlockInfo_vsdlss();
    }
    else {
      local_40[0] = time((time_t *)0x0);
      pFVar9 = (FILE *)getFilePtr_vsdlss(9);
      pcVar10 = ctime(local_40);
      fprintf(pFVar9,"Time Stamp at %s : %s","Start of Reordering",pcVar10);
      fflush(pFVar9);
      casiTimes_vsdlss(uVar3);
      local_70 = getGraphFromElemArr_vsdlss(param_2,param_3,param_7);
      iVar1 = *(int *)(local_70 + 4);
      writeGraph_vsdlss(local_70);
      debugPrint_vsdlss("Graph has been obtained and saved\n");
      casiTimes_vsdlss();
      local_40[0] = time((time_t *)0x0);
      pFVar9 = (FILE *)getFilePtr_vsdlss(9);
      pcVar10 = ctime(local_40);
      fprintf(pFVar9,"Time Stamp at %s : %s","Before Minimum Degree",pcVar10);
      fflush(pFVar9);
      minDegreeOrder_vsdlss(&local_70,&local_68,&local_60,&local_58);
      local_70 = deleteGraph_vsdlss();
      local_40[0] = time((time_t *)0x0);
      pFVar9 = (FILE *)getFilePtr_vsdlss(9);
      pcVar10 = ctime(local_40);
      fprintf(pFVar9,"Time Stamp at %s : %s","After  Minimum Degree",pcVar10);
      fflush(pFVar9);
      casiTimes_vsdlss(uVar8);
      diffTimes_vsdlss(uVar7,uVar8,t_minDegree_vsdlss);
      lVar11 = getRootsInElimTree_vsdlss(local_68,param_2);
      if (lVar11 == 0) {
        nrerror_vsdlss(1,"vsdlss1.c",0x70);
      }
      uVar12 = convertChildToParentToGraph_vsdlss(local_68,param_2);
      if (local_68 != 0) {
        casiFree_vsdlss();
      }
      local_68 = 0;
      debugPrint_vsdlss("Elimination tree vector is converted to graph\n");
      availMem_vsdlss = param_1 * DAT_00abc298;
      iVar2 = 0x12;
      if (param_2 < 0x13) {
        iVar2 = param_2;
      }
      factorBlockMem_vsdlss = computeFactorBlockMem_vsdlss(param_2,local_60,iVar2);
      debugPrint_vsdlss("Start breakupIndistNodeSets\n");
      breakupIndistNodeSets_vsdlss(factorBlockMem_vsdlss,iVar2,local_60,local_58);
      debugPrint_vsdlss("End   breakupIndistNodeSets\n");
      uVar16 = computeOverheadMem_vsdlss(local_58,local_60);
      factorChunkMem_vsdlss =
           computeFactorChunkMem_vsdlss(availMem_vsdlss,factorBlockMem_vsdlss,uVar16,param_2);
      debugPrint_vsdlss();
      local_40[0] = time((time_t *)0x0);
      pFVar9 = (FILE *)getFilePtr_vsdlss(9);
      pcVar10 = ctime(local_40);
      fprintf(pFVar9,"Time Stamp at %s : %s","Before Divide into Chunks",pcVar10);
      fflush(pFVar9);
      casiTimes_vsdlss(uVar7);
      lVar13 = divideIntoChunks_vsdlss
                         (factorChunkMem_vsdlss,param_4,param_5,uVar12,lVar11,local_60,local_58,
                          &local_50,&local_48);
      casiTimes_vsdlss(uVar8);
      diffTimes_vsdlss(uVar7,uVar8,t_divideIntoChunks_vsdlss);
      local_40[0] = time((time_t *)0x0);
      pFVar9 = (FILE *)getFilePtr_vsdlss(9);
      pcVar10 = ctime(local_40);
      fprintf(pFVar9,"Time Stamp at %s : %s","After  Divide into Chunks",pcVar10);
      fflush(pFVar9);
      chunksInProb_vsdlss = *(undefined4 *)(lVar13 + 8);
      deleteGraph_vsdlss(uVar12);
      deleteEntireCEdgelist_vsdlss(lVar11);
      lVar11 = getBlockInfoFromIns_vsdlss(param_2,local_50,&local_58);
      blocksInProb_vsdlss = *(undefined4 *)(lVar11 + 4);
      local_58 = deleteIndistNodeSets_vsdlss(local_58);
      debugPrint_vsdlss("BlockInfo has been obtained\n");
      casiTimes_vsdlss(uVar4);
      diffTimes_vsdlss(uVar3,uVar4,t_reorder_vsdlss);
      local_40[0] = time((time_t *)0x0);
      pFVar9 = (FILE *)getFilePtr_vsdlss(9);
      pcVar10 = ctime(local_40);
      fprintf(pFVar9,"Time Stamp at %s : %s","End of Reordering;Start Read Graph",pcVar10);
      fflush(pFVar9);
      local_70 = readGraph_vsdlss();
      debugPrint_vsdlss();
      local_40[0] = time((time_t *)0x0);
      pFVar9 = (FILE *)getFilePtr_vsdlss(9);
      pcVar10 = ctime(local_40);
      fprintf(pFVar9,"Time Stamp at %s : %s","End   Read Graph;Start Symbolic Elimination",pcVar10);
      fflush(pFVar9);
      casiTimes_vsdlss(uVar3);
      uVar12 = symbolicElimination_vsdlss(&local_70,lVar11,lVar13,local_60,local_50,local_48);
      casiTimes_vsdlss(uVar4);
      diffTimes_vsdlss(uVar3,uVar4,t_symElim_vsdlss);
      local_40[0] = time((time_t *)0x0);
      pFVar9 = (FILE *)getFilePtr_vsdlss(9);
      pcVar10 = ctime(local_40);
      fprintf(pFVar9,"Time Stamp at %s : %s","End   Symbolic Elimination",pcVar10);
      fflush(pFVar9);
      if (local_48 != 0) {
        casiFree_vsdlss();
      }
      local_48 = 0;
      if (local_60 != 0) {
        casiFree_vsdlss();
      }
      local_60 = 0;
      local_70 = deleteGraph_vsdlss(local_70);
      casiRemove_vsdlss(0xb);
      flushCelFreeStructures_vsdlss();
      debugPrint_vsdlss("Preprocessing for reordering complete\n");
      getFactorStats_vsdlss
                (lVar13,lVar11,&maxChunkSize_vsdlss,&avgChunkSize_vsdlss,&factorIndexSize_vsdlss,
                 &factorValueSize_vsdlss,&factorSize_vsdlss,&factorNZ_vsdlss,&opsToFactor_vsdlss,
                 &opsForTriangSolves_vsdlss);
      debugPrint_vsdlss("Factor Stats obtained\n");
      iVar2 = enqOrderOnly_vsdlss();
      lVar14 = local_50;
      if (iVar2 != 0) {
        casiTimes_vsdlss(uVar6);
        diffTimes_vsdlss(uVar5,uVar6,t_reducedSolve_vsdlss);
        lVar11 = newVector_vsdlss(0,(long)param_2,1);
        writeVecToFile_vsdlss(param_6,param_2,lVar11);
        if (lVar11 != 0) {
          casiFree_vsdlss(lVar11);
        }
        removeAllChunkFiles_vsdlss(lVar13);
        goto LAB_00a1b136;
      }
      local_50 = 0;
      writeReorderVec_vsdlss(lVar14,param_2);
      local_40[0] = time((time_t *)0x0);
      pFVar9 = (FILE *)getFilePtr_vsdlss(9);
      pcVar10 = ctime(local_40);
      fprintf(pFVar9,"Time Stamp at %s : %s","Start Matrix Permutation",pcVar10);
      fflush(pFVar9);
      casiTimes_vsdlss(uVar3);
      iVar2 = renumberElemArrAndWriteChunkFiles_vsdlss(param_2,param_3,param_7,lVar14,lVar13);
      if (iVar2 * 2 != iVar1) {
        nrerror_vsdlss(1,"vsdlss1.c",0x108);
      }
      upperANZ_vsdlss = param_2 + iVar2;
      elemsInProb_vsdlss = iVar2;
      if (lVar14 != 0) {
        casiFree_vsdlss(lVar14);
      }
      casiTimes_vsdlss(uVar4);
      diffTimes_vsdlss(uVar3,uVar4,t_permuteA_vsdlss);
      local_40[0] = time((time_t *)0x0);
      pFVar9 = (FILE *)getFilePtr_vsdlss(9);
      pcVar10 = ctime(local_40);
      fprintf(pFVar9,"Time Stamp at %s : %s","End   Matrix Permutation",pcVar10);
      fflush(pFVar9);
      debugPrint_vsdlss("Matrix permuted\n");
      computeSplitBlockForChunks_vsdlss(lVar11,lVar13);
      debugPrint_vsdlss();
      local_40[0] = time((time_t *)0x0);
      pFVar9 = (FILE *)getFilePtr_vsdlss(9);
      pcVar10 = ctime(local_40);
      fprintf(pFVar9,"Time Stamp at %s : %s","Start Factorization",pcVar10);
      fflush(pFVar9);
      casiTimes_vsdlss(uVar3);
      factorA_vsdlss(lVar13,lVar11,uVar12);
      casiTimes_vsdlss(uVar4);
      diffTimes_vsdlss(uVar3,uVar4,t_factor_vsdlss);
      local_40[0] = time((time_t *)0x0);
      pFVar9 = (FILE *)getFilePtr_vsdlss(9);
      pcVar10 = ctime(local_40);
      fprintf(pFVar9,"Time Stamp at %s : %s","End   Factorization",pcVar10);
      fflush(pFVar9);
      deleteAffectedByChunk_vsdlss(uVar12);
      debugPrint_vsdlss("End   factorization\n");
      if (SINGLE_SOLVE_ONLY_vsdlss == '\0') {
        writeChunkInfo_vsdlss(lVar13);
        writeBlockInfo_vsdlss(lVar11);
      }
    }
    lVar14 = readReorderVec_vsdlss();
    lVar15 = newVector_vsdlss(0,(long)param_2,0);
    readDVectorFNum_vsdlss(param_8,param_2,lVar15);
    permuteDVectorInPlace_vsdlss(lVar14,param_2,lVar15);
    debugPrint_vsdlss();
    local_40[0] = time((time_t *)0x0);
    pFVar9 = (FILE *)getFilePtr_vsdlss(9);
    pcVar10 = ctime(local_40);
    fprintf(pFVar9,"Time Stamp at %s : %s","Start Forward Substitution",pcVar10);
    fflush(pFVar9);
    casiTimes_vsdlss(uVar3);
    forwardSubstitute_vsdlss(lVar13,lVar11,lVar15);
    casiTimes_vsdlss(uVar4);
    accumDiffTimes_vsdlss(uVar3,uVar4,t_lowerSolve_vsdlss);
    local_40[0] = time((time_t *)0x0);
    pFVar9 = (FILE *)getFilePtr_vsdlss(9);
    pcVar10 = ctime(local_40);
    fprintf(pFVar9,"Time Stamp at %s : %s","End   Forward Substitution",pcVar10);
    fflush(pFVar9);
    debugPrint_vsdlss("End   forwardSubstitute\n");
    debugPrint_vsdlss();
    local_40[0] = time((time_t *)0x0);
    pFVar9 = (FILE *)getFilePtr_vsdlss(9);
    pcVar10 = ctime(local_40);
    fprintf(pFVar9,"Time Stamp at %s : %s","Start Back Substitution",pcVar10);
    fflush(pFVar9);
    casiTimes_vsdlss(uVar3);
    backSubstitute_vsdlss(lVar13,lVar11,lVar15);
    casiTimes_vsdlss(uVar4);
    accumDiffTimes_vsdlss(uVar3,uVar4,t_upperSolve_vsdlss);
    local_40[0] = time((time_t *)0x0);
    pFVar9 = (FILE *)getFilePtr_vsdlss(9);
    pcVar10 = ctime(local_40);
    fprintf(pFVar9,"Time Stamp at %s : %s","End   Back Substitution",pcVar10);
    fflush(pFVar9);
    debugPrint_vsdlss("End   backSubstitute\n");
    if (SINGLE_SOLVE_ONLY_vsdlss != '\0') {
      removeAllChunkFiles_vsdlss(lVar13);
    }
    deleteChunkInfo_vsdlss(lVar13);
    deleteBlockInfo_vsdlss(lVar11);
    invPermuteDVectorInPlace_vsdlss(lVar14,param_2,lVar15);
    if (lVar14 != 0) {
      casiFree_vsdlss(lVar14);
    }
    debugPrint_vsdlss();
    local_40[0] = time((time_t *)0x0);
    pFVar9 = (FILE *)getFilePtr_vsdlss(9);
    pcVar10 = ctime(local_40);
    fprintf(pFVar9,"Time Stamp at %s : %s","Start Write Reduced Results",pcVar10);
    fflush(pFVar9);
    writeVecToFile_vsdlss(param_6,param_2,lVar15);
    if (lVar15 != 0) {
      casiFree_vsdlss();
    }
    local_40[0] = time((time_t *)0x0);
    pFVar9 = (FILE *)getFilePtr_vsdlss(9);
    pcVar10 = ctime(local_40);
    fprintf(pFVar9,"Time Stamp at %s : %s","End   Write Reduced Results",pcVar10);
    fflush(pFVar9);
    debugPrint_vsdlss("End   writing result file\n");
    casiTimes_vsdlss(uVar6);
    diffTimes_vsdlss(uVar5,uVar6,t_reducedSolve_vsdlss);
    deleteTime_vsdlss(uVar3);
    deleteTime_vsdlss(uVar4);
    deleteTime_vsdlss(uVar5);
    deleteTime_vsdlss(uVar6);
    deleteTime_vsdlss(uVar7);
    deleteTime_vsdlss(uVar8);
    vsdlssStatus_vsdlss = 0;
    debugPrint_vsdlss("********** vsdlss1 ended with %d status code **********\n",0);
  }
  return vsdlssStatus_vsdlss;
}


