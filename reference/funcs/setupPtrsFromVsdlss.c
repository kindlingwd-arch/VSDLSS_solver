/* VSDLSS function (decompiled by Ghidra) */
/* name: setupPtrsFromVsdlss  addr: 00a34fa0  size: 3f */
#include "vsdlss_ref.h"

/* signature: void setupPtrsFromVsdlss(void); */

void setupPtrsFromVsdlss(void)

{
  getPfPtrsVsdlss(&DAT_00ece248,&DAT_00ece240,&DAT_00ece238);
  getFcPtrsVsdlss(&DAT_00ece230,&DAT_00ece228,&DAT_00ece220);
  getPppiPtrsVsdlss(&DAT_00ece218,&DAT_00ece210);
  return;
}


