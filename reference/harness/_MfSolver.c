/* harness: ~MfSolver addr=005fceb0 size=17 */
#include "vsdlss_ref.h"


/* MfSolver::~MfSolver() */

void __thiscall MfSolver::~MfSolver(MfSolver *this)

{
  ~MfSolver(this);
  tc_cfree(this);
  return;
}


