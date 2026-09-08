/* VSDLSS function (decompiled by Ghidra) */
/* name: renameReorderFactorFilesforVsdlss_uwrapper  addr: 00a36c90  size: f3 */
#include "vsdlss_ref.h"

/* signature: void renameReorderFactorFilesforVsdlss_uwrapper(void); */

void renameReorderFactorFilesforVsdlss_uwrapper(void)

{
  int iVar1;
  char *__old;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  undefined8 local_4b8 [4];
  undefined8 local_498 [12];
  char local_438 [1048];
  
  __old = (char *)getFileName_uwrapper(1);
  local_4b8[0] = getFileName_uwrapper(2);
  puVar4 = local_4b8;
  local_4b8[1] = getFileName_uwrapper(3);
  local_4b8[2] = getFileName_uwrapper(4);
  local_4b8[3] = getFileName_uwrapper(5);
  uVar2 = openFileR_uwrapper(0xd);
  while( true ) {
    iVar1 = __isoc99_fscanf(uVar2,&DAT_00acae72,local_438);
    if (iVar1 == -1) {
      uVar3 = getFileName_uwrapper(0xd);
      nrerror_uwrapper(0x45,"uwrapperutils.c",0x5cb,uVar3);
    }
    rename(__old,local_438);
    if (puVar4 == local_498) break;
    __old = (char *)*puVar4;
    puVar4 = puVar4 + 1;
  }
  casiFClose_uwrapper(0xd);
  casiRemove_uwrapper(0xd);
  return;
}


