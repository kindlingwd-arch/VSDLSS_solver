/* VSDLSS function (decompiled by Ghidra) */
/* name: casiSub123RowContribFromIndRangeWrapper_vsdlss  addr: 009e0c50  size: 5d */
#include "vsdlss_ref.h"

/* signature: void casiSub123RowContribFromIndRangeWrapper_vsdlss(int *param_1); */

void casiSub123RowContribFromIndRangeWrapper_vsdlss(int *param_1)

{
  int iVar1;
  
  iVar1 = *param_1;
  if (iVar1 == 2) {
    sub2RowContribFromIndRange_vsdlss(param_1[1],param_1[2],param_1[3],*(undefined8 *)(param_1 + 4))
    ;
    return;
  }
  if (iVar1 != 3) {
    if (iVar1 != 1) {
      nrerror_vsdlss(1,"casipar.c",0x9e9);
      return;
    }
    sub1RowContribFromIndRange_vsdlss(param_1[1],param_1[2],param_1[3],*(undefined8 *)(param_1 + 4))
    ;
    return;
  }
  sub3RowContribFromIndRange_vsdlss(param_1[1],param_1[2],param_1[3],*(undefined8 *)(param_1 + 4));
  return;
}


