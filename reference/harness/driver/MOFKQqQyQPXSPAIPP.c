/* driver: MOFKQqQyQPXSPAIPP addr=009a59e0 size=e70 */
#include "vsdlss_ref.h"


void MOFKQqQyQPXSPAIPP(void)

{
  double dVar1;
  ulong uVar2;
  long lVar3;
  ulong uVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  FILE *__s;
  char *pcVar8;
  long lVar9;
  undefined8 uVar10;
  long lVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  time_t local_40 [2];
  
  lVar5 = BKNmOABOmKIVXSPAIPP();
  lVar6 = BKNmRQmCaLOBXSPAIPP();
  lVar7 = BKNqLISBgKkBJXSPAIPP();
  uVar4 = KLABPgKnOLzmOFDXSPAIPP;
  lVar3 = ABD3lLABPgKnOLzmOFDXSPAIPP;
  uVar2 = KLABPgKnOLzXSPAIPP;
  lVar11 = ABD1lLABPgKnOLzmOFDXSPAIPP;
  lVar9 = ABD2lLABPgKnOLzmOFDXSPAIPP;
  dVar1 = Cy_QLOqFWBXSPAIPP;
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXMOBMLPQMOL_XSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXJFKbBDOBBXSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXAFSFABgKQLaERKHPXSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXOBLOABOXSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXPVJcIFJXSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXMBOJRQBYXSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXCy_QLOXSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXCy_QLOiBOKBIXSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXCy_QLOuFQEFKZIL_HXSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXCy_QLOgmXSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXILTBOqLISBXSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXILTBOqLISBgmXSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXRMMBOqLISBXSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXRMMBOqLISBgmXSPAIPP);
  _LKSBOQrLbB_FqB_LKAPXSPAIPP(QXOBAR_BAqLISBXSPAIPP);
  __s = (FILE *)DBQdFIBnQOXSPAIPP(9);
  fwrite("******************************************************************************\n",1,0x4f,
         __s);
  fwrite("(C) Copyright 1992-2011 Computational Applications and System Integration Inc.\n    All rights Reserved.\n"
         ,1,0x68,__s);
  fwrite("******************************************************************************\n",1,0x4f,
         __s);
  fputc(10,__s);
  local_40[0] = time((time_t *)0x0);
  pcVar8 = ctime(local_40);
  fprintf(__s,"Job:%s\tat:%s\n",GLzlyJBXSPAIPP,pcVar8);
  fputc(10,__s);
  if (kYrpgvXafYlecbXSPAIPP == '\0') {
    fprintf(__s,"\tLoad case solution(Elapsed)\t\t%g\n",BIyMPBAjLyAayPBqLISBrFJB);
    dVar1 = DAT_00ab7268;
    if (_yPFnyOqLISBOd == 0) {
      fprintf(__s,"\tLoad case solution\t\t\t%g\t  %g\n",
              ((double)*QXILTBOqLISBXSPAIPP + (double)*QXRMMBOqLISBXSPAIPP) * DAT_00ab7268,
              ((double)QXILTBOqLISBXSPAIPP[1] + (double)QXRMMBOqLISBXSPAIPP[1]) * DAT_00ab7268);
      fprintf(__s,"\t\tLoad case solution IO\t\t%g\t  %g\n",
              ((double)*QXILTBOqLISBgmXSPAIPP + (double)*QXRMMBOqLISBgmXSPAIPP) * dVar1,
              ((double)QXILTBOqLISBgmXSPAIPP[1] + (double)QXRMMBOqLISBgmXSPAIPP[1]) * dVar1);
    }
    fwrite("******************************************************************************\n",1,0x4f
           ,__s);
    fprintf(__s,"\tTotal Reduced Solve Time (Elapsed): %g secs\n",BIyMPBApBAR_BAqLISBrFJB);
    if (_yPFnyOqLISBOd == 0) {
      fprintf(__s,"\tTotal Reduced Solve CP Time:  User: %g secs: System: %g secs\n",
              (double)*QXOBAR_BAqLISBXSPAIPP * DAT_00ab7268,
              DAT_00ab7268 * (double)QXOBAR_BAqLISBXSPAIPP[1]);
    }
    fwrite("******************************************************************************\n",1,0x4f
           ,__s);
    fprintf(__s,"\tPre-post Time (Elapsed): %g secs\n",BIyMPBAnOBnLPQnOL_rFJB);
    if (_yPFnyOqLISBOd == 0) {
      fprintf(__s,"\tPre and Post-Processing Time: User: %g secs: System: %g secs\n",
              (double)*QXMOBMLPQMOL_XSPAIPP * DAT_00ab7268,
              DAT_00ab7268 * (double)QXMOBMLPQMOL_XSPAIPP[1]);
    }
  }
  else {
    lVar11 = lVar11 + lVar9;
    dVar14 = (double)(lVar11 + lVar3);
    dVar15 = (double)(lVar11 + lVar9 + lVar3 * 3);
    lVar9 = DBQpBAR_BnOLzIBJqFWBdXSPAIPP();
    if (lVar9 == 0) {
      fwrite("\t****** Preprocessing has been deactivated ******\n",1,0x32,__s);
      fwrite("\t****** Statistics for Problem Solution ******\n",1,0x2f,__s);
    }
    else {
      fprintf(__s,"\tNodes (Original): %ld\n",KLABPgKnOLzmOFDXSPAIPP);
      fprintf(__s,"\tOff Diagonals (Original): %ld\n",BIBJPgKnOLzmOFDXSPAIPP);
      fprintf(__s,"\tNodes Eliminated in Pre-processing: %ld\n",(long)dVar14);
      fprintf(__s,"\t\tDegree 1 Nodes: %ld\n",ABD1lLABPgKnOLzmOFDXSPAIPP);
      fprintf(__s,"\t\tDegree 2 Nodes: %ld\n",ABD2lLABPgKnOLzmOFDXSPAIPP);
      fprintf(__s,"\t\tDegree 3 Nodes: %ld\n",ABD3lLABPgKnOLzmOFDXSPAIPP);
      fprintf(__s,"\tTotal Non-zeros in Partial Factors: %ld\n",(long)dVar15);
      fwrite("******************************************************************************\n",1,
             0x4f,__s);
      fwrite("\t****** Statistics for Reduced Problem Solution ******\n",1,0x37,__s);
    }
    fputc(10,__s);
    if (lVar5 != 0) {
      fwrite("******************************************************************************\n",1,
             0x4f,__s);
      fwrite("\t****** Ordering Test Run Only ******\n",1,0x26,__s);
      fwrite("******************************************************************************\n",1,
             0x4f,__s);
      fputc(10,__s);
    }
    fprintf(__s,"\tNodes: %ld\n",KLABPgKnOLzXSPAIPP);
    fprintf(__s,"\tOffDiagonals: %ld\n",BIBJPgKnOLzXSPAIPP);
    fprintf(__s,"\tMax Level Found: %ld\n",JyUjBSBIgKpakXSPAIPP);
    fprintf(__s,"\tNumber of Separating Levels: %ld\n",KRJqBMyOyQLOPgKnOLzXSPAIPP);
    if (FKFQmOABOqBIB_QBAgKnOLzXSPAIPP == 0) {
      fwrite("\tNo Intial Ordering Before Reordering\n",1,0x26,__s);
    }
    else {
      fwrite("\tInitial Order is Selected\n",1,0x1b,__s);
    }
    lVar9 = BKNmOABOqBIB_QLOXSPAIPP();
    if (lVar9 == 0) {
      fwrite("\t*** MLD Reordering Selected ***\n",1,0x21,__s);
    }
    else {
      fwrite("\t*** MMD Reordering Selected ***\n",1,0x21,__s);
    }
    fprintf(__s,"\tIndistinguishable Row Blocks: %ld\n",zIL_HPgKnOLzXSPAIPP);
    fprintf(__s,"\tFactor Chunks: %ld\n",_ERKHPgKnOLzXSPAIPP);
    fprintf(__s,"\tNonzeros in Upper Matrix : %g\n",(double)RMMBOYlxXSPAIPP);
    fprintf(__s,"\tNonzeros in Factor: %g\n",Cy_QLOlxXSPAIPP);
    if (0 < RMMBOYlxXSPAIPP) {
      fprintf(__s,"\tFill Ratio: %g\n",Cy_QLOlxXSPAIPP / (double)RMMBOYlxXSPAIPP);
    }
    fputc(10,__s);
    fprintf(__s,"\tAvailable Memory (MB): %g\n",DAT_00ab67b0 * ySyFIkBJXSPAIPP);
    if (lVar6 == 0) {
      fwrite("\t*** Out-of-core NOT activated. Available Memory ignored ***\n",1,0x3d,__s);
    }
    else {
      fwrite("\t*** Out-of-core activated ***\n",1,0x1f,__s);
    }
    if (lVar7 == 0) {
      fwrite("\t*** Solve-In-Mem NOT activated ***\n",1,0x24,__s);
    }
    else {
      fwrite("\t*** Solve-In-Mem activated ***\n",1,0x20,__s);
    }
    if (_yPFY_QFSyQBnyOAFPLd != 0) {
      fwrite("\t*** Aux Solver in Use ***\n",1,0x1b,__s);
    }
    fprintf(__s,"\tMaximum Chunk Size (MB): %g\n",DAT_00ab67b0 * JyUaERKHqFWBXSPAIPP);
    fprintf(__s,"\tAverage Chunk Size (MB): %g\n",DAT_00ab67b0 * ySDaERKHqFWBXSPAIPP);
    fprintf(__s,"\tFactor Index Size (MB): %g\n",DAT_00ab67b0 * Cy_QLOgKABUqFWBXSPAIPP);
    fprintf(__s,"\tFactor Value Size (MB): %g\n",DAT_00ab67b0 * Cy_QLOtyIRBqFWBXSPAIPP);
    fprintf(__s,"\tFactor Size (MB): %g\n",DAT_00ab67b0 * Cy_QLOqFWBXSPAIPP);
    fputc(10,__s);
    fprintf(__s,"\tOperations for Factoring: %g\n",LMPrLdy_QLOXSPAIPP);
    fprintf(__s,"\tOperations for One Load Case Solve: %g\n",LMPdLOrOFyKDqLISBPXSPAIPP);
    fputc(10,__s);
    fwrite("\tDETAILS OF SOLVER CP TIME(secs)\t\tUser\t  System\n",1,0x30,__s);
    fprintf(__s,"\tReordering (Elapsed)\t\t\t%g\n",BIyMPBApBLOABOrFJB);
    if (_yPFnyOqLISBOd == 0) {
      fprintf(__s,"\tReordering\t\t\t\t%g\t  %g\n",(double)*QXOBLOABOXSPAIPP * DAT_00ab7268,
              (double)QXOBLOABOXSPAIPP[1] * DAT_00ab7268);
      fprintf(__s,"\t\tMinDegree\t\t\t%g\t  %g\n",(double)*QXJFKbBDOBBXSPAIPP * DAT_00ab7268,
              (double)QXJFKbBDOBBXSPAIPP[1] * DAT_00ab7268);
      fprintf(__s,"\t\tdivideIntoChunks\t\t%g\t  %g\n",
              (double)*QXAFSFABgKQLaERKHPXSPAIPP * DAT_00ab7268,
              (double)QXAFSFABgKQLaERKHPXSPAIPP[1] * DAT_00ab7268);
      fprintf(__s,"\tSymbolic Factorization\t\t\t%g\t  %g\n",
              (double)*QXPVJcIFJXSPAIPP * DAT_00ab7268,(double)QXPVJcIFJXSPAIPP[1] * DAT_00ab7268);
    }
    fprintf(__s,"\tPermuteA\t\t\t\t%g\t  %g\n",(double)*QXMBOJRQBYXSPAIPP * DAT_00ab7268,
            (double)QXMBOJRQBYXSPAIPP[1] * DAT_00ab7268);
    fprintf(__s,"\tFactorization (Elapsed)\t\t\t%g\n",BIyMPBAdy_QLOrFJB);
    if (_yPFnyOqLISBOd == 0) {
      fprintf(__s,"\tFactorization\t\t\t\t%g\t  %g\n",(double)*QXCy_QLOXSPAIPP * DAT_00ab7268,
              (double)QXCy_QLOXSPAIPP[1] * DAT_00ab7268);
    }
    fprintf(__s,"\t\tInitialization and IO\t\t%g\t  %g\n",(double)*QXCy_QLOgmXSPAIPP * DAT_00ab7268,
            (double)QXCy_QLOgmXSPAIPP[1] * DAT_00ab7268);
    fprintf(__s,"\tLoad case solution(Elapsed)\t\t%g\n",BIyMPBAjLyAayPBqLISBrFJB);
    if (_yPFnyOqLISBOd == 0) {
      fprintf(__s,"\tLoad case solution\t\t\t%g\t  %g\n",
              ((double)*QXILTBOqLISBXSPAIPP + (double)*QXRMMBOqLISBXSPAIPP) * DAT_00ab7268,
              ((double)QXILTBOqLISBXSPAIPP[1] + (double)QXRMMBOqLISBXSPAIPP[1]) * DAT_00ab7268);
      fprintf(__s,"\t\tLoad case solution IO\t\t%g\t  %g\n",
              ((double)*QXILTBOqLISBgmXSPAIPP + (double)*QXRMMBOqLISBgmXSPAIPP) * DAT_00ab7268,
              ((double)QXILTBOqLISBgmXSPAIPP[1] + (double)QXRMMBOqLISBgmXSPAIPP[1]) * DAT_00ab7268);
    }
    fwrite("******************************************************************************\n",1,0x4f
           ,__s);
    fprintf(__s,"\tTotal Reduced Solve Time (Elapsed): %g secs\n",BIyMPBApBAR_BAqLISBrFJB);
    if (_yPFnyOqLISBOd == 0) {
      fprintf(__s,"\tTotal Reduced Solve CP Time:  User: %g secs: System: %g secs\n",
              (double)*QXOBAR_BAqLISBXSPAIPP * DAT_00ab7268,
              (double)QXOBAR_BAqLISBXSPAIPP[1] * DAT_00ab7268);
    }
    fwrite("******************************************************************************\n",1,0x4f
           ,__s);
    fprintf(__s,"\tPre-post Time (Elapsed): %g secs\n",BIyMPBAnOBnLPQnOL_rFJB);
    if (_yPFnyOqLISBOd == 0) {
      fprintf(__s,"\tPre and Post-Processing Time: User: %g secs: System: %g secs\n",
              (double)*QXMOBMLPQMOL_XSPAIPP * DAT_00ab7268,
              (double)QXMOBMLPQMOL_XSPAIPP[1] * DAT_00ab7268);
    }
    fwrite("******************************************************************************\n",1,0x4f
           ,__s);
    dVar12 = 0.0;
    if (0.0 < BIyMPBAdy_QLOrFJB) {
      dVar12 = (DAT_00ab67b0 * LMPrLdy_QLOXSPAIPP) / BIyMPBAdy_QLOrFJB;
    }
    fprintf(__s,"\tFactoring MFLOP Rate(Elapsed):%g MFlops\n",dVar12);
    if (_yPFnyOqLISBOd == 0) {
      dVar12 = 0.0;
      if (0 < *QXCy_QLOXSPAIPP) {
        dVar12 = (DAT_00ab67b0 * LMPrLdy_QLOXSPAIPP) / ((double)*QXCy_QLOXSPAIPP * DAT_00ab7268);
      }
      fprintf(__s,"\tFactoring MFLOP Rate:%g MFlops\n",dVar12);
    }
    dVar12 = 0.0;
    if (0.0 < BIyMPBAjLyAayPBqLISBrFJB) {
      dVar12 = (DAT_00ab67b0 * LMPdLOrOFyKDqLISBPXSPAIPP) / BIyMPBAjLyAayPBqLISBrFJB;
    }
    fprintf(__s,"\tLoad Case Solution MFLOP Rate(Elapsed):%g MFlops\n",dVar12);
    if (_yPFnyOqLISBOd == 0) {
      dVar12 = 0.0;
      if (0 < *QXILTBOqLISBXSPAIPP + *QXRMMBOqLISBXSPAIPP) {
        dVar12 = (DAT_00ab67b0 * LMPdLOrOFyKDqLISBPXSPAIPP) /
                 (((double)*QXILTBOqLISBXSPAIPP + (double)*QXRMMBOqLISBXSPAIPP) * DAT_00ab7268);
      }
      fprintf(__s,"\tLoad Case Solution MFLOP Rate:%g MFlops\n",dVar12);
    }
    fwrite("******************************************************************************\n",1,0x4f
           ,__s);
    lVar9 = uVar4 << 3;
    if (lVar9 < 0) {
      dVar12 = (double)((uVar4 & 0x1fffffffffffffff) << 2);
      dVar12 = dVar12 + dVar12;
    }
    else {
      dVar12 = (double)lVar9;
    }
    lVar9 = uVar2 << 3;
    if (lVar9 < 0) {
      dVar13 = (double)((uVar2 & 0x1fffffffffffffff) << 2);
      dVar13 = dVar13 + dVar13;
    }
    else {
      dVar13 = (double)lVar9;
    }
    fprintf(__s,"\tApproximate Disk Usage (MB): %g\n",
            (dVar12 + dVar12 + dVar13 +
             (dVar14 + dVar15) * DAT_00ab6798 + dVar15 * DAT_00ab6798 + dVar1 +
            dVar14 * DAT_00ab6798) * DAT_00ab67b0);
  }
  fwrite("******************************************************************************\n",1,0x4f,
         __s);
  uVar10 = BKNrEOByAPgKnLLI();
  fprintf(__s,"\tNumber of threads: %ld\n",uVar10);
  fwrite("******************************************************************************\n",1,0x4f,
         __s);
  fprintf(__s,"\tTotal Time (Elapsed): %g secs\n",BIyMPBArLQyIrFJB);
  fwrite("******************************************************************************\n",1,0x4f,
         __s);
  fflush(__s);
  return;
}


