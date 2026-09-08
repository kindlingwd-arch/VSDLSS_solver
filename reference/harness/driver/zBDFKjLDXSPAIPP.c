/* driver: zBDFKjLDXSPAIPP addr=0098d4e0 size=148 */
#include "vsdlss_ref.h"


void zBDFKjLDXSPAIPP(void)

{
  FILE *__stream;
  char *pcVar1;
  time_t local_10;
  
  __stream = (FILE *)LMBKdFIBYXSPAIPP(8);
  fputc(10,__stream);
  if (qgjclrXSPAIPP == 0) {
    fputc(10,stdout);
  }
  _LMVOFDEQjLDXSPAIPP();
  fputc(10,__stream);
  if (qgjclrXSPAIPP == 0) {
    fputc(10,stdout);
  }
  local_10 = time((time_t *)0x0);
  pcVar1 = ctime(&local_10);
  fprintf(__stream,"Job:%s\tat:%s\n",GLzlyJBXSPAIPP,pcVar1);
  if (qgjclrXSPAIPP == 0) {
    local_10 = time((time_t *)0x0);
    pcVar1 = ctime(&local_10);
    fprintf(stdout,"Job:%s\tat:%s\n",GLzlyJBXSPAIPP,pcVar1);
  }
  fwrite("******************************************************************************\n",1,0x4f,
         __stream);
  if (qgjclrXSPAIPP == 0) {
    fwrite("******************************************************************************\n",1,0x4f
           ,stdout);
  }
  fflush(__stream);
  if (qgjclrXSPAIPP == 0) {
    fflush(stdout);
  }
  return;
}


