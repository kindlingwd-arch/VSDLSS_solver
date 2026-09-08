/* harness: ~CasiSolver addr=005fd260 size=33 */
#include "vsdlss_ref.h"


/* CasiSolver<long>::~CasiSolver() */

void __thiscall CasiSolver<long>::~CasiSolver(CasiSolver<long> *this)

{
  *(undefined ***)this = &PTR_run_00abd158;
  if (*(CasiSolver<long> **)(this + 8) != this + 0x18) {
    tc_cfree(*(CasiSolver<long> **)(this + 8));
  }
  tc_cfree(this);
  return;
}


