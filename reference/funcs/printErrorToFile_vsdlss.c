/* VSDLSS function (decompiled by Ghidra) */
/* name: printErrorToFile_vsdlss  addr: 009f5500  size: 9d */
#include "vsdlss_ref.h"

/* signature: void printErrorToFile_vsdlss(FILE *param_1,uint param_2,undefined8 param_3,undefined8 param_4,undefined8 param_5,uint param_6); */

void printErrorToFile_vsdlss
               (FILE *param_1,uint param_2,undefined8 param_3,undefined8 param_4,undefined8 param_5,
               uint param_6)

{
  fwrite("******vsdlss Error******\n",1,0x19,param_1);
  fprintf(param_1,"Status:%d\n",(ulong)param_2);
  fprintf(param_1,"%s\n",param_4);
  fprintf(param_1,"Version:%s:vsdlss%s:Source File:%s:Line:%d\n",&DAT_00b1c383,param_3,param_5,
          (ulong)param_6);
  fwrite("******vsdlss Error******\n",1,0x19,param_1);
  return;
}


