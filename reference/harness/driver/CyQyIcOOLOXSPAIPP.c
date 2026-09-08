/* driver: CyQyIcOOLOXSPAIPP addr=0095a840 size=137 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void CyQyIcOOLOXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                      undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                      undefined8 param_9,undefined8 param_10,undefined8 param_11,char *param_12,
                      undefined8 param_13,undefined8 param_14)

{
  char in_AL;
  undefined4 local_f0;
  undefined4 local_ec;
  undefined1 *local_e8;
  undefined1 *local_e0;
  undefined1 local_d8 [32];
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 local_98;
  undefined8 local_88;
  undefined8 local_78;
  undefined8 local_68;
  undefined8 local_58;
  undefined8 local_48;
  undefined8 local_38;
  
  if (in_AL != '\0') {
    local_a8 = param_1;
    local_98 = param_2;
    local_88 = param_3;
    local_78 = param_4;
    local_68 = param_5;
    local_58 = param_6;
    local_48 = param_7;
    local_38 = param_8;
  }
  local_e8 = &stack0x00000008;
  local_e0 = local_d8;
  local_f0 = 0x20;
  local_ec = 0x30;
  local_b8 = param_13;
  local_b0 = param_14;
  fwrite("A Fatal Error has Occurred. Processing is terminated.\n",1,0x36,stderr);
  fprintf(stderr,"Status:%ld;Source File:%s;Line:%ld;\n",param_9,param_10,param_11);
  vfprintf(stderr,param_12,&local_f0);
  fflush(stderr);
  if (DAT_00e56340 != (FILE *)0x0) {
    fwrite("A Fatal Error has Occurred. Processing is terminated.\n",1,0x36,DAT_00e56340);
    fprintf(DAT_00e56340,"Status:%ld;Source File:%s;Line:%ld;\n",param_9,param_10,param_11);
    fflush(DAT_00e56340);
  }
  _DAT_00e56140 = param_9;
                    /* WARNING: Subroutine does not return */
  longjmp((__jmp_buf_tag *)&DAT_00e56160,(int)param_9);
}


