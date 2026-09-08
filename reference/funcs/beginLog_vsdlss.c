/* VSDLSS function (decompiled by Ghidra) */
/* name: beginLog_vsdlss  addr: 009f8aa0  size: 148 */
#include "vsdlss_ref.h"

/* signature: void beginLog_vsdlss(void); */

void beginLog_vsdlss(void)

{
  FILE *__stream;
  char *pcVar1;
  time_t local_10;
  
  __stream = (FILE *)openFileA_vsdlss(8);
  fputc(10,__stream);
  if (SILENT_vsdlss == 0) {
    fputc(10,stdout);
  }
  copyrightLog_vsdlss();
  fputc(10,__stream);
  if (SILENT_vsdlss == 0) {
    fputc(10,stdout);
  }
  local_10 = time((time_t *)0x0);
  pcVar1 = ctime(&local_10);
  fprintf(__stream,"Job:%s\tat:%s\n",jobName_vsdlss,pcVar1);
  if (SILENT_vsdlss == 0) {
    local_10 = time((time_t *)0x0);
    pcVar1 = ctime(&local_10);
    fprintf(stdout,"Job:%s\tat:%s\n",jobName_vsdlss,pcVar1);
  }
  fwrite("******************************************************************************\n",1,0x4f,
         __stream);
  if (SILENT_vsdlss == 0) {
    fwrite("******************************************************************************\n",1,0x4f
           ,stdout);
  }
  fflush(__stream);
  if (SILENT_vsdlss == 0) {
    fflush(stdout);
  }
  return;
}


