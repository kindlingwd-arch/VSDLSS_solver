/* VSDLSS function (decompiled by Ghidra) */
/* name: loadJobFromDiskJrt_vsdlss  addr: 009f67d0  size: 1dc */
#include "vsdlss_ref.h"

/* signature: void loadJobFromDiskJrt_vsdlss(void); */

void loadJobFromDiskJrt_vsdlss(void)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  undefined8 uVar5;
  long lVar6;
  undefined4 *puVar7;
  long lVar8;
  long lVar9;
  
  iVar2 = findJobInJrtReturn();
  if (iVar2 == -1) {
    FUN_009f60f0();
  }
  lVar9 = (long)iVar2;
  lVar8 = lVar9 * 0x488;
  if (*(int *)(&DAT_00eb1ec4 + lVar8) == 0) {
    nrerror_vsdlss(0x45,"jobreloadmanager.c",0x151);
    if (*(int *)(&DAT_00eb1ec8 + lVar8) == 0) {
      nrerror_vsdlss(1,"jobreloadmanager.c",0x154);
      return;
    }
    uVar3 = createSemiPermTag_vsdlss();
    *(undefined4 *)(&DAT_00eb1ecc + lVar8) = uVar3;
    uVar5 = tagRead_DEG123REORDERVEC_vsdlss(uVar3);
    *(undefined8 *)(&DAT_00eb1ed0 + lVar8) = uVar5;
    uVar5 = readDeg1Deg2Pf_vsdlss(*(undefined4 *)(&DAT_00eb1ecc + lVar8));
    *(undefined8 *)(&DAT_00eb1ed8 + lVar8) = uVar5;
    lVar6 = readDeg3Pf_vsdlss(*(undefined4 *)(&DAT_00eb1ecc + lVar8));
    piVar1 = *(int **)(&DAT_00eb1ed8 + lVar8);
    *(long *)(&DAT_00eb1ee0 + lVar8) = lVar6;
    iVar2 = *piVar1;
    if (iVar2 - (piVar1[1] + piVar1[2] + *(int *)(lVar6 + 4)) < 1) {
      *(undefined8 *)(&DAT_00eb1ef0 + lVar8) = 0;
      *(undefined4 *)(&DAT_00eb1ee8 + lVar8) = 0xffffffff;
      *(undefined8 *)(&DAT_00eb1ef8 + lVar8) = 0;
      *(undefined4 *)(&DAT_00eb1f00 + lVar8) = 0;
    }
    else {
      puVar7 = (undefined4 *)readFactorOneChunk_vsdlss();
      iVar4 = puVar7[5];
      uVar3 = *puVar7;
      *(undefined4 **)(&DAT_00eb1ef0 + lVar8) = puVar7;
      lVar8 = *(long *)(puVar7 + 8);
      uVar5 = tagNewIVector_vsdlss(uVar3,(long)iVar4,0,0);
      *(undefined8 *)(puVar7 + 8) = uVar5;
      copyIVector_vsdlss(lVar8,(long)(int)puVar7[5],uVar5);
      if (lVar8 != 0) {
        casiFree_vsdlss(lVar8);
      }
      lVar8 = lVar9 * 0x488;
      *(undefined4 *)(&DAT_00eb1ee8 + lVar8) = **(undefined4 **)(&DAT_00eb1ef0 + lVar8);
      setSemiPermF_vsdlss();
      uVar5 = tagReadReorderVec_vsdlss(*(undefined4 *)(&DAT_00eb1ee8 + lVar8));
      *(undefined8 *)(&DAT_00eb1ef8 + lVar8) = uVar5;
      uVar3 = checkIdentityPerm(*(undefined4 *)(*(long *)(&DAT_00eb1ef0 + lVar8) + 8),uVar5);
      *(undefined4 *)(&DAT_00eb1f00 + lVar8) = uVar3;
    }
    *(undefined4 *)(&DAT_00eb1ec4 + lVar9 * 0x488) = 1;
    iVar4 = enqSolveInMem_vsdlss();
    if (iVar4 != 0) {
      uVar5 = tagNewDVector_vsdlss(0,*(undefined4 *)(&DAT_00eb1ecc + lVar9 * 0x488),(long)iVar2,0);
      setPfAndFcInfoPtrs_vsdlss();
      setPfResAndRhsVecs(uVar5);
      return;
    }
  }
  return;
}


