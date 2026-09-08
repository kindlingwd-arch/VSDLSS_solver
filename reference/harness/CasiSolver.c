/* harness: CasiSolver addr=005fdc30 size=a0 */
#include "vsdlss_ref.h"


/* CasiSolver<long>::CasiSolver(std::__cxx11::string const&) */

void __thiscall CasiSolver<long>::CasiSolver(CasiSolver<long> *this,string *param_1)

{
  CasiSolver<long> *__src;
  ulong __n;
  CasiSolver<long> *__dest;
  ulong local_20;
  
  __dest = this + 0x18;
  *(undefined ***)this = &PTR_run_00abd158;
  *(CasiSolver<long> **)(this + 8) = __dest;
  __src = *(CasiSolver<long> **)param_1;
  __n = *(ulong *)(param_1 + 8);
  if ((__src + __n != (CasiSolver<long> *)0x0) && (__src == (CasiSolver<long> *)0x0)) {
                    /* WARNING: Subroutine does not return */
    std::__throw_logic_error("basic_string::_M_construct null not valid");
  }
  local_20 = __n;
  if (__n < 0x10) {
    if (__n == 1) {
      this[0x18] = *__src;
      goto LAB_005fdc77;
    }
    if (__n == 0) goto LAB_005fdc77;
  }
  else {
    __dest = (CasiSolver<long> *)
             std::__cxx11::string::_M_create((ulong *)(this + 8),(ulong)&local_20);
    *(CasiSolver<long> **)(this + 8) = __dest;
    *(ulong *)(this + 0x18) = local_20;
  }
  memcpy(__dest,__src,__n);
  __dest = *(CasiSolver<long> **)(this + 8);
LAB_005fdc77:
  *(ulong *)(this + 0x10) = local_20;
  __dest[local_20] = (CasiSolver<long>)0x0;
  return;
}


