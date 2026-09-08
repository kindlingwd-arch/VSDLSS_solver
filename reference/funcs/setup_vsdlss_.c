/* VSDLSS function (decompiled by Ghidra) */
/* name: setup_vsdlss_  addr: 005fdce0  size: 181 */
#include "vsdlss_ref.h"

/* signature: void __thiscall CasiSolver<long>::setup_vsdlss_(CasiSolver<long> *this,Symmetric *param_1); */

/* CasiSolver<long>::setup_vsdlss_(Symmetric<double, int>&) */

void __thiscall CasiSolver<long>::setup_vsdlss_(CasiSolver<long> *this,Symmetric *param_1)

{
  undefined1 *__src;
  ulong __n;
  long lVar1;
  long lVar2;
  FILE *__stream;
  ulong local_60;
  ulong *local_58;
  ulong local_50;
  ulong local_48 [3];
  
  __src = *(undefined1 **)(this + 8);
  __n = *(ulong *)(this + 0x10);
  local_58 = local_48;
  if ((__src + __n != (undefined1 *)0x0) && (__src == (undefined1 *)0x0)) {
                    /* WARNING: Subroutine does not return */
    std::__throw_logic_error("basic_string::_M_construct null not valid");
  }
  local_60 = __n;
  if (__n < 0x10) {
    if (__n == 1) {
      local_48[0] = CONCAT71(local_48[0]._1_7_,*__src);
      goto LAB_005fdd44;
    }
    if (__n == 0) goto LAB_005fdd44;
  }
  else {
    local_58 = (ulong *)std::__cxx11::string::_M_create((ulong *)&local_58,(ulong)&local_60);
    local_48[0] = local_60;
  }
  memcpy(local_58,__src,__n);
LAB_005fdd44:
  *(undefined1 *)((long)local_58 + local_60) = 0;
  local_50 = local_60;
  if (0x3fffffffffffffff - local_60 < 4) {
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 005fde61 to 005fde65 has its CatchHandler @ 005fde75 */
    std::__throw_length_error("basic_string::append");
  }
                    /* try { // try from 005fdd74 to 005fdd78 has its CatchHandler @ 005fde75 */
  std::__cxx11::string::_M_append((char *)&local_58,0xabcfe0);
                    /* try { // try from 005fdd83 to 005fdd87 has its CatchHandler @ 005fde70 */
  __stream = fopen((char *)local_58,"w");
  if (__stream != (FILE *)0x0) {
    fclose(__stream);
  }
  if (local_58 != local_48) {
    tc_cfree(local_58);
  }
  Symmetric<double,int>::solver_write<long>((Symmetric<double,int> *)param_1,(string *)(this + 8));
  *(long *)(this + 0x28) = (long)*(int *)param_1;
  lVar1 = *(long *)(param_1 + 0x20);
  lVar2 = *(long *)(param_1 + 0x18);
  *(undefined8 *)(this + 0x38) = 0;
  *(long *)(this + 0x30) = lVar1 - lVar2 >> 4;
  return;
}


