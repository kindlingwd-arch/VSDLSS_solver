/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDOrderInternal_vsdlss  addr: 00a059b0  size: 704 */
#include "vsdlss_ref.h"

/* signature: void MLDOrderInternal_vsdlss(long param_1,long param_2,undefined8 param_3,int *param_4,undefined4 param_5,undefined4 param_6); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void MLDOrderInternal_vsdlss
               (long param_1,long param_2,undefined8 param_3,int *param_4,undefined4 param_5,
               undefined4 param_6)

{
  int iVar1;
  undefined8 uVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined4 uVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  long lVar16;
  undefined8 uVar17;
  undefined8 uVar18;
  undefined8 uVar19;
  undefined8 uVar20;
  undefined8 uVar21;
  ulong uVar22;
  int *piVar23;
  long lVar24;
  ulong uVar25;
  long lVar26;
  double dVar27;
  double dVar28;
  double dVar29;
  double dVar30;
  int local_dc;
  int local_58;
  int local_3c [3];
  
  local_dc = *param_4;
  iVar1 = *(int *)(param_1 + 8);
  lVar16 = tagNewMLDOrderVecs(param_5,iVar1);
  lVar24 = (long)iVar1;
  uVar20 = *(undefined8 *)(lVar16 + 0x58);
  dVar30 = (double)iVar1 * _DAT_00b1a140;
  initIVector_vsdlss(uVar20,lVar24,0);
  uVar21 = *(undefined8 *)(lVar16 + 0x50);
  initIVector_vsdlss(uVar21,lVar24,0);
  uVar2 = *(undefined8 *)(lVar16 + 0x70);
  initIVector_vsdlss(uVar2,lVar24,0);
  uVar17 = *(undefined8 *)(lVar16 + 0x90);
  initIVector_vsdlss(uVar17,lVar24,0);
  lVar24 = *(long *)(lVar16 + 0x40);
  *(undefined8 *)(lVar24 + 8) = *(undefined8 *)(lVar16 + 0x98);
  initCountingMark_vsdlss(iVar1,lVar24);
  lVar3 = *(long *)(lVar16 + 0x30);
  uVar18 = *(undefined8 *)(lVar16 + 0x60);
  uVar19 = *(undefined8 *)(lVar16 + 0x68);
  uVar4 = *(undefined8 *)(lVar16 + 0xa8);
  *(int *)(lVar3 + 8) = iVar1;
  *(undefined8 *)(lVar3 + 0x10) = uVar18;
  initQ_vsdlss(lVar3);
  uVar10 = findConnCompsAndMakeContig_2
                     (param_6,local_dc,param_2 + (long)(iVar1 - local_dc) * 4,param_1,param_3,uVar20
                      ,uVar21,uVar2,uVar17,iVar1,uVar4,uVar19,*(undefined8 *)(lVar24 + 8),lVar3);
  uVar17 = tagNewAStack(param_5,iVar1);
  uVar18 = tagNewAStack(param_5,iVar1);
  uVar19 = tagNewAStack(param_5,iVar1);
  MLDPushComponents_vsdlss(uVar10,uVar20,uVar21,uVar2,uVar17,uVar18,uVar19);
  setCountingMarkToNull_vsdlss(lVar24);
  setQToNull_vsdlss(lVar3);
  uVar20 = tagNewMLDGraphHeaderOnly_vsdlss(param_5);
  dVar27 = _DAT_00ab67f8;
  local_58 = 1;
  if (((1 < iVar1) && (0.0 < DAT_00ec5d88)) && (DAT_00ec5d88 < _DAT_00ab67f8)) {
    dVar28 = log((double)iVar1);
    dVar29 = log(DAT_00ec5d88);
    dVar27 = dVar27 - dVar28 / dVar29;
    local_58 = (int)(dVar27 + dVar27);
  }
  uVar21 = tagNewMLDGraphVector(param_5,local_58,1,0);
  iVar11 = iVar1 + 0x1f;
  if (-1 < iVar1) {
    iVar11 = iVar1;
  }
LAB_00a05c50:
  do {
    iVar12 = emptyAStack_vsdlss(uVar17);
    while( true ) {
      if (iVar12 != 0) {
        *param_4 = local_dc;
        return;
      }
      dVar27 = (double)getTagCurrSizeBytes_vsdlss(param_6);
      if (dVar30 <= dVar27) {
        deleteBlocksInitTag(param_6);
      }
      iVar13 = popAStack_vsdlss(uVar17);
      iVar14 = popAStack_vsdlss(uVar18);
      uVar25 = (ulong)iVar14;
      iVar15 = popAStack_vsdlss(uVar19);
      lVar24 = setCompGraphPointers_vsdlss(param_1,param_3,iVar13,iVar14,iVar15,uVar20,local_3c);
      iVar12 = 2;
      if (1 < DAT_00ec5d80) {
        iVar12 = DAT_00ec5d80;
      }
      if (iVar12 < local_3c[0]) break;
      local_dc = local_dc + iVar14;
      lVar3 = param_2 + (long)(iVar1 - local_dc) * 4;
      MMDFromMLD(param_6,uVar20,lVar3,lVar16);
      if (uVar25 == 0) goto LAB_00a05c50;
      uVar22 = 1;
      do {
        *(undefined4 *)(lVar3 + uVar22 * 4) =
             *(undefined4 *)(lVar24 + (long)*(int *)(lVar3 + uVar22 * 4) * 4);
        uVar22 = uVar22 + 1;
      } while (uVar22 <= uVar25);
      iVar12 = emptyAStack_vsdlss(uVar17);
    }
    MLDFindPartition_vsdlss(local_58,param_6,uVar20,uVar21,lVar16);
    iVar12 = MLDConvertEdgeSepToNodeSep_vsdlss(param_6,uVar20,lVar16);
    if (iVar12 < 1) {
      local_dc = local_dc + iVar14;
      lVar3 = param_2 + (long)(iVar1 - local_dc) * 4;
      MMDFromMLD(param_6,uVar20,lVar3,lVar16);
      if (uVar25 != 0) {
        uVar22 = 1;
        do {
          *(undefined4 *)(lVar3 + uVar22 * 4) =
               *(undefined4 *)(lVar24 + (long)*(int *)(lVar3 + uVar22 * 4) * 4);
          uVar22 = uVar22 + 1;
        } while (uVar22 <= uVar25);
      }
    }
    else {
      local_dc = local_dc + iVar12;
      lVar3 = param_2 + (long)(iVar1 - local_dc) * 4;
      copyIVector_vsdlss(*(undefined8 *)(lVar16 + 0xb0),(long)iVar12,lVar3);
      if (iVar11 >> 5 <= iVar14) {
        deleteBlocksInitTag(param_6);
      }
      lVar5 = *(long *)(lVar16 + 0x58);
      lVar26 = (long)iVar14;
      initIVector_vsdlss(lVar5,lVar26,0);
      uVar2 = *(undefined8 *)(lVar16 + 0x50);
      initIVector_vsdlss(uVar2,lVar26,0);
      lVar6 = *(long *)(lVar16 + 0x70);
      initIVector_vsdlss(lVar6,lVar26,0);
      uVar4 = *(undefined8 *)(lVar16 + 0x90);
      initIVector_vsdlss(uVar4,lVar26,0);
      lVar26 = *(long *)(lVar16 + 0x40);
      *(undefined8 *)(lVar26 + 8) = *(undefined8 *)(lVar16 + 0x98);
      initCountingMark_vsdlss(iVar14,lVar26);
      lVar7 = *(long *)(lVar16 + 0x30);
      uVar8 = *(undefined8 *)(lVar16 + 0xa8);
      uVar9 = *(undefined8 *)(lVar16 + 0x68);
      *(undefined8 *)(lVar7 + 0x10) = *(undefined8 *)(lVar16 + 0x60);
      *(int *)(lVar7 + 8) = iVar14;
      initQ_vsdlss(lVar7);
      iVar12 = findConnCompsAndMakeContig_2
                         (param_6,iVar12,lVar3,uVar20,lVar24,lVar5,uVar2,lVar6,uVar4,iVar1,uVar8,
                          uVar9,*(undefined8 *)(lVar26 + 8),lVar7);
      if (0 < iVar12) {
        piVar23 = (int *)(lVar5 + 4);
        lVar24 = (ulong)(iVar12 - 1) * 4;
        do {
          *piVar23 = *piVar23 + iVar13 + -1;
          piVar23 = piVar23 + 1;
        } while (piVar23 != (int *)(lVar5 + 8 + lVar24));
        piVar23 = (int *)(lVar6 + 4);
        do {
          *piVar23 = *piVar23 + iVar15 + -1;
          piVar23 = piVar23 + 1;
        } while (piVar23 != (int *)(lVar6 + 8 + lVar24));
      }
      MLDPushComponents_vsdlss(iVar12,lVar5,uVar2,lVar6,uVar17,uVar18,uVar19);
      setCountingMarkToNull_vsdlss(lVar26);
      setQToNull_vsdlss(lVar7);
    }
  } while( true );
}


