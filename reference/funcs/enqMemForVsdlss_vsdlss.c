/* VSDLSS function (decompiled by Ghidra) */
/* name: enqMemForVsdlss_vsdlss  addr: 009e7890  size: 4d */
#include "vsdlss_ref.h"

/* signature: undefined8 enqMemForVsdlss_vsdlss(void); */

undefined8 enqMemForVsdlss_vsdlss(void)

{
  FILE *__stream;
  undefined8 local_10;
  
  local_10 = 0;
  __stream = fopen("sdsmem.cas","r");
  if (__stream != (FILE *)0x0) {
    __isoc99_fscanf(__stream,"%lg",&local_10);
    fclose(__stream);
  }
  return local_10;
}


