/* harness: ~MatrixEqn addr=0061e0d0 size=30 */
#include "vsdlss_ref.h"


/* MatrixEqn::~MatrixEqn() */

void __thiscall MatrixEqn::~MatrixEqn(MatrixEqn *this)

{
  if (*(void **)(this + 0x188) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x188));
  }
  if (*(void **)(this + 0x168) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x168));
  }
  MapPartitioned3d<double,int>::~MapPartitioned3d((MapPartitioned3d<double,int> *)(this + 8));
  return;
}


