/* driver: MOFKQcOOLOrLdFIBXSPAIPP addr=0098a040 size=9b */
#include "vsdlss_ref.h"


void MOFKQcOOLOrLdFIBXSPAIPP
               (FILE *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               undefined8 param_5,undefined8 param_6)

{
  fwrite("******SPAIPP Error******\n",1,0x19,param_1);
  fprintf(param_1,"Status:%ld\n",param_2);
  fprintf(param_1,"%s\n",param_4);
  fprintf(param_1,"Version:%s:SPAIPP%s:Source File:%s:Line:%ld\n",&DAT_00b1c383,param_3,param_5,
          param_6);
  fwrite("******SPAIPP Error******\n",1,0x19,param_1);
  return;
}


