/* VSDLSS function (decompiled by Ghidra) */
/* name: createSemiPermTag_vsdlss  addr: 009fb910  size: 1c */
#include "vsdlss_ref.h"

/* signature: void createSemiPermTag_vsdlss(void); */

void createSemiPermTag_vsdlss(void)

{
  int iVar1;
  
  iVar1 = createTag_vsdlss();
  (&DAT_00ec5980)[iVar1] = 1;
  return;
}


