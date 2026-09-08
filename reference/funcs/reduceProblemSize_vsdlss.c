/* VSDLSS function (decompiled by Ghidra) */
/* name: reduceProblemSize_vsdlss  addr: 00a10590  size: 5fd */
#include "vsdlss_ref.h"

/* signature: int reduceProblemSize_vsdlss(int param_1,int param_2,undefined4 param_3,undefined8 param_4,int *param_5,int *param_6,int *param_7); */

int reduceProblemSize_vsdlss
              (int param_1,int param_2,undefined4 param_3,undefined8 param_4,int *param_5,
              int *param_6,int *param_7)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  undefined4 *puVar6;
  undefined8 *puVar7;
  undefined8 uVar8;
  long lVar9;
  long lVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  long lVar14;
  int local_94;
  int local_90;
  int local_8c;
  int local_88;
  undefined4 local_84;
  undefined8 local_80;
  undefined1 local_78 [24];
  undefined8 local_60;
  undefined8 local_50;
  
  iVar2 = enqSolveInMem_vsdlss();
  if (iVar2 == 0) {
    DAT_00e3d070 = 0xffffffff;
  }
  else {
    DAT_00e3d070 = createSemiPermTag_vsdlss();
  }
  initElemArrHeader(local_78,param_2,param_1);
  local_60 = newIVector_vsdlss((long)param_2,0,0);
  local_50 = newIVector_vsdlss((long)param_2,0,0);
  readIVectorFNum_vsdlss(0x1b,param_2,local_60);
  readIVectorFNum_vsdlss(0x1c,param_2,local_50);
  checkFromAndToNodes_vsdlss(param_1,param_2,local_60,local_50);
  local_80 = bucketNeighborsIntoNodeBuckets_vsdlss(local_78);
  deleteElemArrNoHeader(local_78);
  lVar5 = (long)param_1;
  if (iVar2 == 0) {
    puVar6 = (undefined4 *)newIVector_vsdlss(lVar5,0,0);
  }
  else {
    puVar6 = (undefined4 *)tagNewIVector_vsdlss(DAT_00e3d070,lVar5,0,0);
  }
  *puVar6 = 0;
  iterReorderDeg123Nodes
            (param_3,param_4,&local_80,puVar6,&DAT_00ec5e7c,&local_90,&local_8c,&local_88);
  local_80 = deleteArrBucketArr_vsdlss(local_80);
  if (ONLY_MATRIX_VALUES_CHANGED != 0) {
    lVar9 = getDeg123ReorderVecPrev();
    if (iVar2 != 0) {
      lVar10 = getReorderVecPrev();
      iVar12 = local_90 + local_8c + local_88;
      iVar4 = param_1 - iVar12;
      if (param_1 < 1) {
        iVar3 = 0;
      }
      else {
        iVar3 = 0;
        lVar14 = 0;
        do {
          iVar13 = puVar6[lVar14 + 1] - iVar12;
          if (0 < iVar13) {
            iVar3 = iVar3 + 1;
            iVar11 = *(int *)(lVar9 + 4 + lVar14 * 4) - iVar12;
            if (((iVar4 < iVar13) || (iVar11 < 1)) || (iVar4 < iVar11)) {
              nrerror_vsdlss(1,"reduceproblem.c",0x2ec);
            }
            *(int *)(lVar10 + (long)iVar11 * 4) = iVar13;
          }
          iVar13 = (int)lVar14;
          lVar14 = lVar14 + 1;
        } while (iVar13 + 2 <= param_1);
      }
      if (iVar3 != iVar4) {
        nrerror_vsdlss(1,"reduceproblem.c",0x2f3);
      }
    }
    if (lVar9 != 0) {
      casiFree_vsdlss(lVar9);
    }
    setDeg123ReorderVecPrev(0);
  }
  iVar4 = local_90 + local_8c;
  deg2NodesInProbOrig_vsdlss = local_8c;
  deg1NodesInProbOrig_vsdlss = local_90;
  deg3NodesInProbOrig_vsdlss = local_88;
  iVar12 = iVar4 + local_88;
  *param_5 = local_90;
  *param_6 = iVar4;
  *param_7 = local_88;
  puVar1 = puVar6;
  if (iVar2 == 0) {
    write_DEG123REORDERVEC_vsdlss(puVar6,param_1);
    puVar1 = DAT_00ec5eb0;
  }
  DAT_00ec5eb0 = puVar1;
  renumberAtEndList(param_1,puVar6,iVar12,param_3,param_4);
  puVar7 = (undefined8 *)newFVector_vsdlss(0,lVar5,0);
  *puVar7 = 0;
  readFVectorFNum_vsdlss(0x1a,param_1,puVar7);
  checkForNegativeDiags_vsdlss(param_1,puVar7);
  handleIsolatedVarsInDiagVec_vsdlss(param_1,puVar7,0);
  permuteFVectorInPlace_vsdlss(puVar6,param_1,puVar7);
  initWriteFromToOffDiag_vsdlss(0x15,0x16,0x17);
  initChunkWrite_vsdlss(0xd);
  local_84 = 0;
  local_94 = 0;
  lVar5 = formMatForDeg1Deg2AndSplit_vsdlss
                    (DAT_00e3d070,param_1,param_2,local_90,local_8c,local_88,puVar6,&local_84,
                     &local_94);
  if (iVar2 == 0) {
    casiFree_vsdlss(puVar6);
    *(undefined8 **)(lVar5 + 0x10) = puVar7;
    factorDeg1Deg2Pf_vsdlss(local_88,lVar5,puVar7,&local_84,&local_94);
    finishChunkWrite_vsdlss();
    writeDeg1Deg2Pf_vsdlss(lVar5);
    *(undefined8 *)(lVar5 + 0x10) = 0;
    deleteDeg1Deg2Pf_vsdlss(lVar5);
  }
  else {
    *(undefined8 **)(lVar5 + 0x10) = puVar7;
    factorDeg1Deg2Pf_vsdlss(local_88,lVar5,puVar7,&local_84,&local_94);
    finishChunkWrite_vsdlss();
    uVar8 = tagNewFVector_vsdlss(0,DAT_00e3d070,(long)iVar4,0);
    *(undefined8 *)(lVar5 + 0x10) = uVar8;
    copyFVector_vsdlss(puVar7,(long)iVar4,uVar8);
    DAT_00ec5ea8 = lVar5;
  }
  iVar3 = param_1 - iVar4;
  writeFVectorFNum_vsdlss(3,iVar3,puVar7 + iVar4);
  casiFree_vsdlss(puVar7);
  uVar8 = formMatForDeg3_vsdlss(DAT_00e3d070,iVar3,local_88,local_84);
  casiRemove_vsdlss(0xd);
  puVar7 = (undefined8 *)newFVector_vsdlss(0,(long)iVar3,0);
  *puVar7 = 0;
  readFVectorFNum_vsdlss(3,iVar3,puVar7);
  casiRemove_vsdlss(3);
  factorDeg3Pf_vsdlss(uVar8,puVar7,&local_94);
  iVar4 = finishWriteFromToOffDiag_vsdlss();
  if (iVar4 != local_94) {
    nrerror_vsdlss(1,"reduceproblem.c",0x36a);
  }
  if (iVar2 == 0) {
    writeDeg3Pf_vsdlss(uVar8);
    deleteDeg3Pf_vsdlss(uVar8);
    writeFVectorFNum_vsdlss(0x14,param_1 - iVar12,puVar7 + local_88);
    casiFree_vsdlss(puVar7);
  }
  else {
    DAT_00ec5ea0 = uVar8;
    writeFVectorFNum_vsdlss(0x14,param_1 - iVar12,puVar7 + local_88);
    casiFree_vsdlss(puVar7);
    setPfInfoJrt_vsdlss(DAT_00e3d070,DAT_00ec5eb0,DAT_00ec5ea8,DAT_00ec5ea0);
  }
  return local_94;
}


