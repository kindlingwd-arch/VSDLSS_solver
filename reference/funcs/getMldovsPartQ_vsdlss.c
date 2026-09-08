/* VSDLSS function (decompiled by Ghidra) */
/* name: getMldovsPartQ_vsdlss  addr: 009fce90  size: 8f */
#include "vsdlss_ref.h"

/* signature: long getMldovsPartQ_vsdlss(undefined4 param_1,int param_2,long param_3); */

long getMldovsPartQ_vsdlss(undefined4 param_1,int param_2,long param_3)

{
  long lVar1;
  
  lVar1 = *(long *)(param_3 + (long)param_2 * 8);
  *(undefined4 *)(lVar1 + 4) = param_1;
  *(undefined4 *)(lVar1 + 8) = 0;
  if (param_2 == 1) {
    *(undefined8 *)(lVar1 + 0x10) = *(undefined8 *)(param_3 + 0x60);
    *(undefined8 *)(lVar1 + 0x18) = *(undefined8 *)(param_3 + 0x68);
    *(undefined8 *)(lVar1 + 0x20) = *(undefined8 *)(param_3 + 0x70);
    initPqi(lVar1);
    return lVar1;
  }
  if (param_2 != 2) {
    nrerror_vsdlss(1,"mldorder.c",0x2f6);
    initPqi(lVar1);
    return lVar1;
  }
  *(undefined8 *)(lVar1 + 0x10) = *(undefined8 *)(param_3 + 0x78);
  *(undefined8 *)(lVar1 + 0x18) = *(undefined8 *)(param_3 + 0x80);
  *(undefined8 *)(lVar1 + 0x20) = *(undefined8 *)(param_3 + 0x88);
  initPqi(lVar1);
  return lVar1;
}


