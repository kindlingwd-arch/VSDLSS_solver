/* VSDLSS function (decompiled by Ghidra) */
/* name: insertJobIntoJrt_vsdlss  addr: 009f6450  size: ac */
#include "vsdlss_ref.h"

/* signature: void insertJobIntoJrt_vsdlss(void); */

void insertJobIntoJrt_vsdlss(void)

{
  int iVar1;
  int *piVar2;
  long lVar3;
  
  iVar1 = findJobInJrtReturn();
  if (0 < iVar1) {
    nrerror_vsdlss(1,"jobreloadmanager.c",0xc6);
  }
  piVar2 = &DAT_00eb1f48;
  iVar1 = 1;
  do {
    if (*piVar2 == 0) {
      lVar3 = (long)iVar1 * 0x488;
      *(undefined4 *)(&DAT_00eb1ac0 + lVar3) = 1;
      strcpy((char *)(lVar3 + 0xeb1ac4),jobName_vsdlss);
      *(undefined4 *)(&DAT_00eb1ec4 + lVar3) = 1;
      *(undefined4 *)(&DAT_00eb1ec8 + lVar3) = 0;
      initPfAndFcEntriesByPosJrt(iVar1);
      return;
    }
    iVar1 = iVar1 + 1;
    piVar2 = piVar2 + 0x122;
  } while (iVar1 != 0x41);
  nrerror_vsdlss(0x2f,"jobreloadmanager.c",0xd8,"MAX_NUMBER_JOBS");
  return;
}


