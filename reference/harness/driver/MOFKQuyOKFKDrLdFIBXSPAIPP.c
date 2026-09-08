/* driver: MOFKQuyOKFKDrLdFIBXSPAIPP addr=00989fa0 size=99 */
#include "vsdlss_ref.h"


void MOFKQuyOKFKDrLdFIBXSPAIPP
               (FILE *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               undefined8 param_5,undefined8 param_6)

{
  fwrite("******SPAIPP Warning******\n",1,0x1b,param_1);
  fwrite("Status:Warning\n",1,0xf,param_1);
  fprintf(param_1,"%s\n",param_4);
  fprintf(param_1,"Version:%s:SPAIPP%s:Source File:%s:Line:%ld\n",&DAT_00b1c383,param_3,param_5,
          param_6);
  fwrite("******SPAIPP Warning******\n",1,0x1b,param_1);
  return;
}


