/* VSDLSS function (decompiled by Ghidra) */
/* name: writeDeg1Deg2Pf_vsdlss  addr: 009e3910  size: 1c6 */
#include "vsdlss_ref.h"

/* signature: void writeDeg1Deg2Pf_vsdlss(undefined4 *param_1); */

void writeDeg1Deg2Pf_vsdlss(undefined4 *param_1)

{
  FILE *__s;
  size_t sVar1;
  int iVar2;
  int iVar3;
  undefined4 local_34;
  int local_30;
  int local_2c [3];
  
  local_2c[0] = param_1[2];
  local_30 = param_1[1];
  local_34 = *param_1;
  iVar3 = local_30 + local_2c[0];
  iVar2 = local_2c[0] + iVar3;
  __s = (FILE *)openFileW_vsdlss(0xf);
  sVar1 = fwrite(&local_34,4,1,__s);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x10c,"writeDeg1Deg2Pf:nVars",1,sVar1 & 0xffffffff);
  }
  sVar1 = fwrite(&local_30,4,1,__s);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x113,"writeDeg1Deg2Pf:nDeg1Nodes",1,sVar1 & 0xffffffff);
  }
  sVar1 = fwrite(local_2c,4,1,__s);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x11a,"writeDeg1Deg2Pf:nDeg2Nodes",1,sVar1 & 0xffffffff);
  }
  iVar3 = iVar3 + 1;
  sVar1 = fwrite(*(void **)(param_1 + 4),8,(long)iVar3,__s);
  if ((int)sVar1 != iVar3) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x121,"writeDeg1Deg2Pf:pfDiagonalVec(pf)",iVar3,
                   sVar1 & 0xffffffff);
  }
  iVar2 = iVar2 + 1;
  sVar1 = fwrite(*(void **)(param_1 + 6),4,(long)iVar2,__s);
  if ((int)sVar1 != iVar2) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x128,"writeDeg1Deg2Pf:pfIndex1And2Vec(pf)",iVar2,
                   sVar1 & 0xffffffff);
  }
  sVar1 = fwrite(*(void **)(param_1 + 8),8,(long)iVar2,__s);
  if (iVar2 != (int)sVar1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x12f,"writeDeg1Deg2Pf:pfValue1And2Vec(pf)",iVar2,
                   sVar1 & 0xffffffff);
  }
  casiFClose_vsdlss(0xf);
  return;
}


