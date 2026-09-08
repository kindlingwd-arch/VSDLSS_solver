/* VSDLSS function (decompiled by Ghidra) */
/* name: printStats_vsdlss  addr: 00a0bb20  size: e5b */
#include "vsdlss_ref.h"

/* signature: void printStats_vsdlss(void); */

void printStats_vsdlss(void)

{
  uint uVar1;
  double dVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  FILE *__s;
  char *pcVar10;
  long lVar11;
  uint uVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  time_t local_40 [2];
  
  iVar6 = enqOrderOnly_vsdlss();
  iVar7 = enqOutOfCore_vsdlss();
  iVar8 = enqSolveInMem_vsdlss();
  uVar5 = nodesInProbOrig_vsdlss;
  uVar1 = deg1NodesInProbOrig_vsdlss;
  uVar4 = deg2NodesInProbOrig_vsdlss;
  uVar3 = nodesInProb_vsdlss;
  uVar12 = deg3NodesInProbOrig_vsdlss;
  dVar2 = factorSize_vsdlss;
  convertToDeciSeconds_vsdlss(t_prepostproc_vsdlss);
  convertToDeciSeconds_vsdlss(t_minDegree_vsdlss);
  convertToDeciSeconds_vsdlss(t_divideIntoChunks_vsdlss);
  convertToDeciSeconds_vsdlss(t_reorder_vsdlss);
  convertToDeciSeconds_vsdlss(t_symElim_vsdlss);
  convertToDeciSeconds_vsdlss(t_permuteA_vsdlss);
  convertToDeciSeconds_vsdlss(t_factor_vsdlss);
  convertToDeciSeconds_vsdlss(t_factorKernel_vsdlss);
  convertToDeciSeconds_vsdlss(t_factorWithinBlock_vsdlss);
  convertToDeciSeconds_vsdlss(t_factorIO_vsdlss);
  convertToDeciSeconds_vsdlss(t_lowerSolve_vsdlss);
  convertToDeciSeconds_vsdlss(t_lowerSolveIO_vsdlss);
  convertToDeciSeconds_vsdlss(t_upperSolve_vsdlss);
  convertToDeciSeconds_vsdlss(t_upperSolveIO_vsdlss);
  convertToDeciSeconds_vsdlss(t_reducedSolve_vsdlss);
  __s = (FILE *)getFilePtr_vsdlss(9);
  fwrite("******************************************************************************\n",1,0x4f,
         __s);
  fwrite("(C) Copyright 1992-2011 Computational Applications and System Integration Inc.\n    All rights Reserved.\n"
         ,1,0x68,__s);
  fwrite("******************************************************************************\n",1,0x4f,
         __s);
  fputc(10,__s);
  local_40[0] = time((time_t *)0x0);
  pcVar10 = ctime(local_40);
  fprintf(__s,"Job:%s\tat:%s\n",jobName_vsdlss,pcVar10);
  fputc(10,__s);
  if (MATRIX_CHANGED_vsdlss == '\0') {
    fprintf(__s,"\tLoad case solution(Elapsed)\t\t%g\n",elapsedLoadCaseSolveTime);
    dVar2 = DAT_00ab7268;
    if (casiParSolverF == 0) {
      fprintf(__s,"\tLoad case solution\t\t\t%g\t  %g\n",
              ((double)*t_lowerSolve_vsdlss + (double)*t_upperSolve_vsdlss) * DAT_00ab7268,
              ((double)t_lowerSolve_vsdlss[1] + (double)t_upperSolve_vsdlss[1]) * DAT_00ab7268);
      fprintf(__s,"\t\tLoad case solution IO\t\t%g\t  %g\n",
              ((double)*t_lowerSolveIO_vsdlss + (double)*t_upperSolveIO_vsdlss) * dVar2,
              ((double)t_lowerSolveIO_vsdlss[1] + (double)t_upperSolveIO_vsdlss[1]) * dVar2);
    }
    fwrite("******************************************************************************\n",1,0x4f
           ,__s);
    fprintf(__s,"\tTotal Reduced Solve Time (Elapsed): %g secs\n",elapsedReducedSolveTime);
    if (casiParSolverF == 0) {
      fprintf(__s,"\tTotal Reduced Solve CP Time:  User: %g secs: System: %g secs\n",
              (double)*t_reducedSolve_vsdlss * DAT_00ab7268,
              DAT_00ab7268 * (double)t_reducedSolve_vsdlss[1]);
    }
    fwrite("******************************************************************************\n",1,0x4f
           ,__s);
    fprintf(__s,"\tPre-post Time (Elapsed): %g secs\n",elapsedPrePostProcTime);
    if (casiParSolverF == 0) {
      fprintf(__s,"\tPre and Post-Processing Time: User: %g secs: System: %g secs\n",
              (double)*t_prepostproc_vsdlss * DAT_00ab7268,
              DAT_00ab7268 * (double)t_prepostproc_vsdlss[1]);
    }
  }
  else {
    iVar9 = uVar1 + uVar4;
    uVar1 = iVar9 + uVar12;
    uVar12 = uVar4 + iVar9 + uVar12 * 3;
    iVar9 = getReduceProblemSizeF_vsdlss();
    if (iVar9 == 0) {
      fwrite("\t****** Preprocessing has been deactivated ******\n",1,0x32,__s);
      fwrite("\t****** Statistics for Problem Solution ******\n",1,0x2f,__s);
    }
    else {
      fprintf(__s,"\tNodes (Original): %d\n",(ulong)nodesInProbOrig_vsdlss);
      fprintf(__s,"\tOff Diagonals (Original): %d\n",(ulong)elemsInProbOrig_vsdlss);
      fprintf(__s,"\tNodes Eliminated in Pre-processing: %d\n",(ulong)uVar1);
      fprintf(__s,"\t\tDegree 1 Nodes: %d\n",(ulong)deg1NodesInProbOrig_vsdlss);
      fprintf(__s,"\t\tDegree 2 Nodes: %d\n",(ulong)deg2NodesInProbOrig_vsdlss);
      fprintf(__s,"\t\tDegree 3 Nodes: %d\n",(ulong)deg3NodesInProbOrig_vsdlss);
      fprintf(__s,"\tTotal Non-zeros in Partial Factors: %d\n",(ulong)uVar12);
      fwrite("******************************************************************************\n",1,
             0x4f,__s);
      fwrite("\t****** Statistics for Reduced Problem Solution ******\n",1,0x37,__s);
    }
    fputc(10,__s);
    if (iVar6 != 0) {
      fwrite("******************************************************************************\n",1,
             0x4f,__s);
      fwrite("\t****** Ordering Test Run Only ******\n",1,0x26,__s);
      fwrite("******************************************************************************\n",1,
             0x4f,__s);
      fputc(10,__s);
    }
    fprintf(__s,"\tNodes: %d\n",(ulong)nodesInProb_vsdlss);
    fprintf(__s,"\tOffDiagonals: %d\n",(ulong)elemsInProb_vsdlss);
    fprintf(__s,"\tMax Level Found: %d\n",(ulong)maxLevelInRCM_vsdlss);
    fprintf(__s,"\tNumber of Separating Levels: %d\n",(ulong)numSeparatorsInProb_vsdlss);
    if (initOrderSelectedInProb_vsdlss == 0) {
      fwrite("\tNo Intial Ordering Before Reordering\n",1,0x26,__s);
    }
    else {
      fwrite("\tInitial Order is Selected\n",1,0x1b,__s);
    }
    iVar6 = enqOrderSelector_vsdlss();
    if (iVar6 == 0) {
      fwrite("\t*** MLD Reordering Selected ***\n",1,0x21,__s);
    }
    else {
      fwrite("\t*** MMD Reordering Selected ***\n",1,0x21,__s);
    }
    fprintf(__s,"\tIndistinguishable Row Blocks: %d\n",(ulong)blocksInProb_vsdlss);
    fprintf(__s,"\tFactor Chunks: %d\n",(ulong)chunksInProb_vsdlss);
    fprintf(__s,"\tNonzeros in Upper Matrix : %g\n",(double)upperANZ_vsdlss);
    fprintf(__s,"\tNonzeros in Factor: %g\n",factorNZ_vsdlss);
    if (0 < upperANZ_vsdlss) {
      fprintf(__s,"\tFill Ratio: %g\n",factorNZ_vsdlss / (double)upperANZ_vsdlss);
    }
    fputc(10,__s);
    fprintf(__s,"\tAvailable Memory (MB): %g\n",DAT_00ab67b0 * availMem_vsdlss);
    if (iVar7 == 0) {
      fwrite("\t*** Out-of-core NOT activated. Available Memory ignored ***\n",1,0x3d,__s);
    }
    else {
      fwrite("\t*** Out-of-core activated ***\n",1,0x1f,__s);
    }
    if (iVar8 == 0) {
      fwrite("\t*** Solve-In-Mem NOT activated ***\n",1,0x24,__s);
    }
    else {
      fwrite("\t*** Solve-In-Mem activated ***\n",1,0x20,__s);
    }
    if (casiActivatePardisoF != 0) {
      fwrite("\t*** Aux Solver in Use ***\n",1,0x1b,__s);
    }
    fprintf(__s,"\tMaximum Chunk Size (MB): %g\n",DAT_00ab67b0 * maxChunkSize_vsdlss);
    fprintf(__s,"\tAverage Chunk Size (MB): %g\n",DAT_00ab67b0 * avgChunkSize_vsdlss);
    fprintf(__s,"\tFactor Index Size (MB): %g\n",DAT_00ab67b0 * factorIndexSize_vsdlss);
    fprintf(__s,"\tFactor Value Size (MB): %g\n",DAT_00ab67b0 * factorValueSize_vsdlss);
    fprintf(__s,"\tFactor Size (MB): %g\n",DAT_00ab67b0 * factorSize_vsdlss);
    fputc(10,__s);
    fprintf(__s,"\tOperations for Factoring: %g\n",opsToFactor_vsdlss);
    fprintf(__s,"\tOperations for One Load Case Solve: %g\n",opsForTriangSolves_vsdlss);
    fputc(10,__s);
    fwrite("\tDETAILS OF SOLVER CP TIME(secs)\t\tUser\t  System\n",1,0x30,__s);
    fprintf(__s,"\tReordering (Elapsed)\t\t\t%g\n",elapsedReorderTime);
    if (casiParSolverF == 0) {
      fprintf(__s,"\tReordering\t\t\t\t%g\t  %g\n",(double)*t_reorder_vsdlss * DAT_00ab7268,
              (double)t_reorder_vsdlss[1] * DAT_00ab7268);
      fprintf(__s,"\t\tMinDegree\t\t\t%g\t  %g\n",(double)*t_minDegree_vsdlss * DAT_00ab7268,
              (double)t_minDegree_vsdlss[1] * DAT_00ab7268);
      fprintf(__s,"\t\tdivideIntoChunks\t\t%g\t  %g\n",
              (double)*t_divideIntoChunks_vsdlss * DAT_00ab7268,
              (double)t_divideIntoChunks_vsdlss[1] * DAT_00ab7268);
      fprintf(__s,"\tSymbolic Factorization\t\t\t%g\t  %g\n",
              (double)*t_symElim_vsdlss * DAT_00ab7268,(double)t_symElim_vsdlss[1] * DAT_00ab7268);
    }
    fprintf(__s,"\tPermuteA\t\t\t\t%g\t  %g\n",(double)*t_permuteA_vsdlss * DAT_00ab7268,
            (double)t_permuteA_vsdlss[1] * DAT_00ab7268);
    fprintf(__s,"\tFactorization (Elapsed)\t\t\t%g\n",elapsedFactorTime);
    if (casiParSolverF == 0) {
      fprintf(__s,"\tFactorization\t\t\t\t%g\t  %g\n",(double)*t_factor_vsdlss * DAT_00ab7268,
              (double)t_factor_vsdlss[1] * DAT_00ab7268);
    }
    fprintf(__s,"\t\tInitialization and IO\t\t%g\t  %g\n",(double)*t_factorIO_vsdlss * DAT_00ab7268,
            (double)t_factorIO_vsdlss[1] * DAT_00ab7268);
    fprintf(__s,"\tLoad case solution(Elapsed)\t\t%g\n",elapsedLoadCaseSolveTime);
    if (casiParSolverF == 0) {
      fprintf(__s,"\tLoad case solution\t\t\t%g\t  %g\n",
              ((double)*t_lowerSolve_vsdlss + (double)*t_upperSolve_vsdlss) * DAT_00ab7268,
              ((double)t_lowerSolve_vsdlss[1] + (double)t_upperSolve_vsdlss[1]) * DAT_00ab7268);
      fprintf(__s,"\t\tLoad case solution IO\t\t%g\t  %g\n",
              ((double)*t_lowerSolveIO_vsdlss + (double)*t_upperSolveIO_vsdlss) * DAT_00ab7268,
              ((double)t_lowerSolveIO_vsdlss[1] + (double)t_upperSolveIO_vsdlss[1]) * DAT_00ab7268);
    }
    fwrite("******************************************************************************\n",1,0x4f
           ,__s);
    fprintf(__s,"\tTotal Reduced Solve Time (Elapsed): %g secs\n",elapsedReducedSolveTime);
    if (casiParSolverF == 0) {
      fprintf(__s,"\tTotal Reduced Solve CP Time:  User: %g secs: System: %g secs\n",
              (double)*t_reducedSolve_vsdlss * DAT_00ab7268,
              (double)t_reducedSolve_vsdlss[1] * DAT_00ab7268);
    }
    fwrite("******************************************************************************\n",1,0x4f
           ,__s);
    fprintf(__s,"\tPre-post Time (Elapsed): %g secs\n",elapsedPrePostProcTime);
    if (casiParSolverF == 0) {
      fprintf(__s,"\tPre and Post-Processing Time: User: %g secs: System: %g secs\n",
              (double)*t_prepostproc_vsdlss * DAT_00ab7268,
              (double)t_prepostproc_vsdlss[1] * DAT_00ab7268);
    }
    fwrite("******************************************************************************\n",1,0x4f
           ,__s);
    dVar13 = 0.0;
    if (0.0 < elapsedFactorTime) {
      dVar13 = (DAT_00ab67b0 * opsToFactor_vsdlss) / elapsedFactorTime;
    }
    fprintf(__s,"\tFactoring MFLOP Rate(Elapsed):%g MFlops\n",dVar13);
    if (casiParSolverF == 0) {
      dVar13 = 0.0;
      if (0 < *t_factor_vsdlss) {
        dVar13 = (DAT_00ab67b0 * opsToFactor_vsdlss) / ((double)*t_factor_vsdlss * DAT_00ab7268);
      }
      fprintf(__s,"\tFactoring MFLOP Rate:%g MFlops\n",dVar13);
    }
    dVar13 = 0.0;
    if (0.0 < elapsedLoadCaseSolveTime) {
      dVar13 = (DAT_00ab67b0 * opsForTriangSolves_vsdlss) / elapsedLoadCaseSolveTime;
    }
    fprintf(__s,"\tLoad Case Solution MFLOP Rate(Elapsed):%g MFlops\n",dVar13);
    if (casiParSolverF == 0) {
      dVar13 = 0.0;
      if (0 < *t_lowerSolve_vsdlss + *t_upperSolve_vsdlss) {
        dVar13 = (DAT_00ab67b0 * opsForTriangSolves_vsdlss) /
                 (((double)*t_lowerSolve_vsdlss + (double)*t_upperSolve_vsdlss) * DAT_00ab7268);
      }
      fprintf(__s,"\tLoad Case Solution MFLOP Rate:%g MFlops\n",dVar13);
    }
    dVar15 = (double)(int)uVar1;
    dVar13 = (double)(int)uVar12;
    fwrite("******************************************************************************\n",1,0x4f
           ,__s);
    lVar11 = (long)(int)uVar5 << 2;
    if (lVar11 < 0) {
      dVar14 = (double)(((long)(int)uVar5 & 0x3fffffffffffffffU) << 1);
      dVar14 = dVar14 + dVar14;
    }
    else {
      dVar14 = (double)lVar11;
    }
    lVar11 = (long)(int)uVar3 << 2;
    if (lVar11 < 0) {
      dVar16 = (double)(((long)(int)uVar3 & 0x3fffffffffffffffU) << 1);
      dVar16 = dVar16 + dVar16;
    }
    else {
      dVar16 = (double)lVar11;
    }
    fprintf(__s,"\tApproximate Disk Usage (MB): %g\n",
            (dVar14 + dVar14 + dVar16 +
             (dVar13 + dVar15) * DAT_00ab6798 + dVar13 * DAT_00ad4f20 + dVar2 +
            DAT_00ab6798 * dVar15) * DAT_00ab67b0);
  }
  fwrite("******************************************************************************\n",1,0x4f,
         __s);
  uVar12 = enqThreadsInPool();
  fprintf(__s,"\tNumber of threads: %d\n",(ulong)uVar12);
  fwrite("******************************************************************************\n",1,0x4f,
         __s);
  fprintf(__s,"\tTotal Time (Elapsed): %g secs\n",elapsedTotalTime);
  fwrite("******************************************************************************\n",1,0x4f,
         __s);
  fflush(__s);
  return;
}


