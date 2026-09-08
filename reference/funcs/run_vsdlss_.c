/* VSDLSS function (decompiled by Ghidra) */
/* name: run_vsdlss_  addr: 005fde90  size: 81 */
#include "vsdlss_ref.h"

/* signature: void __thiscall CasiSolver<long>::run_vsdlss_(CasiSolver<long> *this,undefined8 param_2,undefined8 param_3); */

/* CasiSolver<long>::run_vsdlss_(SolverAttributes::SolveCase, double*) */

void __thiscall
CasiSolver<long>::run_vsdlss_(CasiSolver<long> *this,undefined8 param_2,undefined8 param_3)

{
  undefined4 uVar1;
  undefined8 local_58;
  char *local_50;
  undefined *local_48;
  undefined1 *local_40;
  undefined *local_38;
  undefined8 local_30;
  undefined8 local_28;
  undefined4 local_10;
  
  DAT_00e413e8 = param_3;
  FUN_005fbba0(this + 8,param_2,*(undefined8 *)(this + 0x28),*(undefined8 *)(this + 0x30),0);
  local_30 = *(undefined8 *)(this + 8);
  local_58 = 0x30;
  local_50 = "vsdlss";
  local_48 = &DAT_00abcfec;
  local_38 = &DAT_00abcfef;
  local_10 = 5;
  local_28 = 0;
  local_40 = (undefined1 *)&local_58;
  uVar1 = solverVsdl64(5,&local_50);
  FUN_005fb920(uVar1);
  return;
}


