/* driver: MOFKQrLjLDXSPAIPP addr=0098d420 size=b6 */
#include "vsdlss_ref.h"


void MOFKQrLjLDXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                      undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                      char *param_9,undefined8 param_10,undefined8 param_11,undefined8 param_12,
                      undefined8 param_13,undefined8 param_14)

{
  char in_AL;
  FILE *__s;
  undefined4 local_e0;
  undefined4 local_dc;
  undefined1 *local_d8;
  undefined1 *local_d0;
  undefined1 local_c8 [8];
  undefined8 local_c0;
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 local_88;
  undefined8 local_78;
  undefined8 local_68;
  undefined8 local_58;
  undefined8 local_48;
  undefined8 local_38;
  undefined8 local_28;
  
  if (in_AL != '\0') {
    local_98 = param_1;
    local_88 = param_2;
    local_78 = param_3;
    local_68 = param_4;
    local_58 = param_5;
    local_48 = param_6;
    local_38 = param_7;
    local_28 = param_8;
  }
  local_c0 = param_10;
  local_b8 = param_11;
  local_b0 = param_12;
  local_a8 = param_13;
  local_a0 = param_14;
  __s = (FILE *)DBQdFIBnQOXSPAIPP(8);
  local_d8 = &stack0x00000008;
  local_e0 = 8;
  local_d0 = local_c8;
  local_dc = 0x30;
  vfprintf(__s,param_9,&local_e0);
  fflush(__s);
  return;
}


