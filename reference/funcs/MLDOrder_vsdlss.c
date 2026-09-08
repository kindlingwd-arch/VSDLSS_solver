/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDOrder_vsdlss  addr: 00a07db0  size: 210 */
#include "vsdlss_ref.h"

/* signature: void MLDOrder_vsdlss(int param_1,undefined4 param_2,undefined8 param_3,undefined8 param_4,undefined4 param_5,undefined8 param_6,undefined8 param_7,undefined4 *param_8,long *param_9); */

void MLDOrder_vsdlss(int param_1,undefined4 param_2,undefined8 param_3,undefined8 param_4,
                    undefined4 param_5,undefined8 param_6,undefined8 param_7,undefined4 *param_8,
                    long *param_9)

{
  int iVar1;
  undefined8 uVar2;
  long lVar3;
  long lVar4;
  undefined4 local_bc;
  long local_b8;
  undefined8 local_b0;
  undefined1 local_a8 [120];
  
  if (ONLY_MATRIX_VALUES_CHANGED == 0) {
    getMLDGraphFromElemArr_vsdlss();
    iVar1 = enqDumpMetis();
    if (iVar1 != 0) {
      dumpMetis(local_a8);
      nrerror_vsdlss(0x39,"mldorder.c",0x1911);
    }
    initIVector_vsdlss(param_7,(long)param_1,0xffffffff);
    local_bc = 0;
    MLDOrderInternal_mt(local_a8,param_5,param_6,param_7,&local_bc,*param_8,&local_b8);
    deleteMLDGraphNoHeader(local_a8);
    MLDcheckForProperReordering_vsdlss(param_1,local_bc,param_7);
  }
  else {
    lVar3 = getReorderVecPrev();
    lVar4 = getFbspiPrev();
    copyIVector_vsdlss(lVar3,(long)param_1,param_7);
    if (lVar3 != 0) {
      casiFree_vsdlss(lVar3);
    }
    setReorderVecPrev(0);
    if (lVar4 == 0) {
      local_b8 = 0;
      local_b0 = 0;
      goto LAB_00a07e88;
    }
    copyFbspi(*param_8,lVar4,&local_b8);
    deleteFbspi(lVar4);
    setFbspiPrev(0);
  }
  if (local_b8 == 0) {
    local_b0 = 0;
  }
  else {
    copyFbspi(*param_8,local_b8,&local_b0);
  }
LAB_00a07e88:
  setFbspiFromMLD(local_b0);
  getMLDGraphFromElemArr_vsdlss(param_1,param_2,param_3,param_4,local_a8);
  uVar2 = newIVector_vsdlss((long)param_1,0,0);
  *(undefined8 *)(param_8 + 8) = uVar2;
  MMDFromMLDStubNew_vsdlss(local_a8,param_5,param_6,param_7,param_8);
  deleteMLDGraphNoHeader(local_a8);
  *param_9 = local_b8;
  return;
}


