/* VSDLSS function (decompiled by Ghidra) */
/* name: readMatrixA12_vsdlss  addr: 009daa00  size: 207 */
#include "vsdlss_ref.h"

/* signature: undefined4 * readMatrixA12_vsdlss(int param_1,int param_2); */

undefined4 * readMatrixA12_vsdlss(int param_1,int param_2)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  int iVar10;
  undefined1 local_88 [4];
  int local_84;
  int local_80;
  
  puVar6 = (undefined4 *)newSparseFactor_vsdlss(param_2,1);
  uVar1 = *puVar6;
  uVar7 = openFileR_vsdlss(0x29);
  readIVectorFp(uVar7,1,local_88);
  iVar3 = local_84;
  if (local_84 != param_2) {
    nrerror_vsdlss(0xc,"vsdlss.c",0x102,param_2,local_84,&DAT_00b19051);
  }
  iVar10 = 1;
  if (0 < iVar3) {
    do {
      readIVectorFp(uVar7,2,local_88);
      iVar5 = local_80;
      iVar4 = local_84;
      if ((param_2 < local_84) || (local_84 < 0)) {
        nrerror_vsdlss(0xf,"vsdlss.c",0x10f,param_2);
        if (iVar5 <= param_1) goto LAB_009daaa7;
LAB_009dab48:
        nrerror_vsdlss(0x11,"vsdlss.c",0x115,iVar5,param_1);
      }
      else {
        if (param_1 < local_80) goto LAB_009dab48;
LAB_009daaa7:
        if (iVar5 < 0) goto LAB_009dab48;
      }
      uVar8 = 0;
      uVar9 = 0;
      if (iVar5 != 0) {
        uVar8 = tagNewIVector_vsdlss(uVar1,(long)iVar5,0,0);
        readIVectorFp(uVar7,iVar5,uVar8);
        rangeCheckIVector_vsdlss(uVar8,iVar5,1,param_1,"A12 Index Vector");
        uVar9 = tagNewDVector_vsdlss(0,uVar1,(long)iVar5,0);
        readDVectorFp(uVar7,iVar5,uVar9);
      }
      iVar10 = iVar10 + 1;
      *(int *)(*(long *)(puVar6 + 0xc) + (long)iVar4 * 4) = iVar5;
      lVar2 = *(long *)(puVar6 + 0x10);
      uVar9 = tagNewIVVWithPtrs_vsdlss(uVar1,uVar8,uVar9);
      *(undefined8 *)(lVar2 + (long)iVar4 * 8) = uVar9;
    } while (iVar10 <= iVar3);
  }
  casiFClose_vsdlss(0x29);
  return puVar6;
}


