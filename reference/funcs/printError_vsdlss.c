/* VSDLSS function (decompiled by Ghidra) */
/* name: printError_vsdlss  addr: 009f52e0  size: 17b */
#include "vsdlss_ref.h"

/* signature: void printError_vsdlss(uint param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,uint param_5); */

void printError_vsdlss(uint param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                      uint param_5)

{
  char local_138 [8];
  undefined8 local_130;
  undefined8 local_128;
  undefined2 local_120;
  
  builtin_strncpy(local_138,"******vs",8);
  local_130 = 0x7272452073736c64;
  local_128 = 0x2a2a2a2a2a2a726f;
  local_120 = 10;
  cwCasiCasmsg_vsdlss(local_138);
  sprintf(local_138,"Status:%d\n",(ulong)param_1);
  cwCasiCasmsg_vsdlss(local_138);
  sprintf(local_138,"Version:%s\n",&DAT_00b1c383);
  cwCasiCasmsg_vsdlss(local_138);
  sprintf(local_138,"vsdlss%s\n",param_2);
  cwCasiCasmsg_vsdlss(local_138);
  sprintf(local_138,"%s\n",param_3);
  cwCasiCasmsg_vsdlss(local_138);
  builtin_strncpy(local_138,"Source F",8);
  local_130 = CONCAT35(local_130._5_3_,0xa656c69);
  cwCasiCasmsg_vsdlss(local_138);
  sprintf(local_138,"%s\n",param_4);
  cwCasiCasmsg_vsdlss(local_138);
  sprintf(local_138,"Line:%d\n",(ulong)param_5);
  cwCasiCasmsg_vsdlss(local_138);
  builtin_strncpy(local_138,"******vs",8);
  local_130 = 0x7272452073736c64;
  local_128 = 0x2a2a2a2a2a2a726f;
  local_120 = 10;
  cwCasiCasmsg_vsdlss(local_138);
  return;
}


