/* VSDLSS function (decompiled by Ghidra) */
/* name: fileExistsMain_vsdlss  addr: 009d2400  size: 27 */
#include "vsdlss_ref.h"

/* signature: bool fileExistsMain_vsdlss(char *param_1); */

bool fileExistsMain_vsdlss(char *param_1)

{
  FILE *__stream;
  
  __stream = fopen(param_1,"rb");
  if (__stream != (FILE *)0x0) {
    fclose(__stream);
  }
  return __stream != (FILE *)0x0;
}


