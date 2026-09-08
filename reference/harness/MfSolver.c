/* harness: MfSolver addr=005fca50 size=df */
#include "vsdlss_ref.h"


/* MfSolver::MfSolver(std::__cxx11::string const&) */

void __thiscall MfSolver::MfSolver(MfSolver *this,string *param_1)

{
  MfSolver *__src;
  ulong __n;
  MfSolver *__dest;
  ulong local_20;
  
  __dest = this + 0x18;
  *(undefined ***)this = &PTR_run_00abd188;
  *(MfSolver **)(this + 8) = __dest;
  __src = *(MfSolver **)param_1;
  __n = *(ulong *)(param_1 + 8);
  if ((__src + __n != (MfSolver *)0x0) && (__src == (MfSolver *)0x0)) {
                    /* WARNING: Subroutine does not return */
    std::__throw_logic_error("basic_string::_M_construct null not valid");
  }
  local_20 = __n;
  if (__n < 0x10) {
    if (__n == 1) {
      this[0x18] = *__src;
      goto LAB_005fca9b;
    }
    if (__n == 0) goto LAB_005fca9b;
  }
  else {
    __dest = (MfSolver *)std::__cxx11::string::_M_create((ulong *)(this + 8),(ulong)&local_20);
    *(MfSolver **)(this + 8) = __dest;
    *(ulong *)(this + 0x18) = local_20;
  }
  memcpy(__dest,__src,__n);
  __dest = *(MfSolver **)(this + 8);
LAB_005fca9b:
  *(ulong *)(this + 0x10) = local_20;
  __dest[local_20] = (MfSolver)0x0;
  *(undefined8 *)(this + 0x28) = 0;
  *(undefined8 *)(this + 0x30) = 0;
  this[0x38] = (MfSolver)0x0;
  *(undefined4 *)(this + 0x40) = 0;
  *(undefined4 *)(this + 0x60) = 0;
  *(undefined8 *)(this + 0x88) = 0;
  *(undefined8 *)(this + 0xb0) = 0;
  return;
}


