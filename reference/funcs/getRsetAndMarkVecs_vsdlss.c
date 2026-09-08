/* VSDLSS function (decompiled by Ghidra) */
/* name: getRsetAndMarkVecs_vsdlss  addr: 00a1a310  size: 158 */
#include "vsdlss_ref.h"

/* signature: void getRsetAndMarkVecs_vsdlss(int param_1,int param_2,undefined8 *param_3,undefined8 *param_4); */

void getRsetAndMarkVecs_vsdlss(int param_1,int param_2,undefined8 *param_3,undefined8 *param_4)

{
  char *pcVar1;
  undefined4 *puVar2;
  undefined1 *puVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int local_48;
  int local_44;
  undefined4 local_40;
  undefined4 local_3c [3];
  
  puVar2 = (undefined4 *)newIVector_vsdlss((long)param_1,0,0);
  *puVar2 = 0;
  readIVectorFNum_vsdlss(0x19,param_1,puVar2);
  getMinAndMaxIVector_vsdlss(puVar2,param_1,&local_48,&local_44,&local_40,local_3c);
  if (local_48 < 1) {
    nrerror_vsdlss(0x2d,"vsdlss1memutils.c",0xa9a,local_48,local_40,1,param_2);
  }
  if (param_2 < local_44) {
    nrerror_vsdlss(0x2d,"vsdlss1memutils.c",0xaa2,local_44,local_3c[0],1,param_2);
  }
  puVar3 = (undefined1 *)newCVector_vsdlss((long)param_2,1,0);
  *puVar3 = 0;
  piVar6 = puVar2 + 1;
  iVar4 = 1;
  if (0 < param_1) {
    do {
      while( true ) {
        pcVar1 = puVar3 + *piVar6;
        if (*pcVar1 == '\0') break;
        iVar5 = iVar4 + 1;
        nrerror_vsdlss(0x2e,"vsdlss1memutils.c",0xab2,*piVar6,iVar4);
        piVar6 = piVar6 + 1;
        *pcVar1 = '\x01';
        iVar4 = iVar5;
        if (param_1 < iVar5) goto LAB_00a1a3f9;
      }
      iVar4 = iVar4 + 1;
      piVar6 = piVar6 + 1;
      *pcVar1 = '\x01';
    } while (iVar4 <= param_1);
  }
LAB_00a1a3f9:
  *param_3 = puVar2;
  *param_4 = puVar3;
  return;
}


