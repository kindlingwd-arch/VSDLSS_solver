/* driver: ABzRDmMBKXSPAIPP addr=0098da10 size=a8 */
#include "vsdlss_ref.h"


FILE * ABzRDmMBKXSPAIPP(char *param_1)

{
  FILE *pFVar1;
  
  if (DAT_00e3b548 == '\0') {
    DAT_00e3b548 = 0;
  }
  else {
    DAT_00e3b548 = 0;
    pFVar1 = fopen("debugprint.cas","r");
    if (pFVar1 == (FILE *)0x0) {
      DAT_00e8bdc0 = 0;
      return (FILE *)0x0;
    }
    DAT_00e8bdc0 = '\x01';
    fclose(pFVar1);
  }
  if (DAT_00e8bdc0 != '\0') {
    pFVar1 = fopen(param_1,"w");
    if (pFVar1 == (FILE *)0x0) {
      KOBOOLOXSPAIPP(0xe,"logfileutils.c",0x136,param_1,&DAT_00b20e86);
    }
    return pFVar1;
  }
  return (FILE *)0x0;
}


