/* VSDLSS function (decompiled by Ghidra) */
/* name: printWarningToFile_vsdlss  addr: 009f5460  size: 9b */
#include "vsdlss_ref.h"

/* signature: void printWarningToFile_vsdlss(FILE *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,undefined8 param_5,uint param_6); */

void printWarningToFile_vsdlss
               (FILE *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               undefined8 param_5,uint param_6)

{
  fwrite("******vsdlss Warning******\n",1,0x1b,param_1);
  fwrite("Status:Warning\n",1,0xf,param_1);
  fprintf(param_1,"%s\n",param_4);
  fprintf(param_1,"Version:%s:vsdlss%s:Source File:%s:Line:%d\n",&DAT_00b1c383,param_3,param_5,
          (ulong)param_6);
  fwrite("******vsdlss Warning******\n",1,0x1b,param_1);
  return;
}


