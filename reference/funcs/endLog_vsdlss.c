/* VSDLSS function (decompiled by Ghidra) */
/* name: endLog_vsdlss  addr: 009f8c10  size: 75 */
#include "vsdlss_ref.h"

/* signature: void endLog_vsdlss(void); */

void endLog_vsdlss(void)

{
  FILE *__s;
  
  __s = (FILE *)getFilePtr_vsdlss(8);
  fwrite("******************************************************************************\n",1,0x4f,
         __s);
  if (SILENT_vsdlss == 0) {
    fwrite("******************************************************************************\n",1,0x4f
           ,stdout);
  }
  fflush(__s);
  if (SILENT_vsdlss == 0) {
    fflush(stdout);
  }
  casiFClose_vsdlss(8);
  return;
}


