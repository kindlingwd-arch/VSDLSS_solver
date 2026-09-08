/* harness: ~MapPartitioned3d addr=005f2300 size=fa */
#include "vsdlss_ref.h"


/* MapPartitioned3d<double, int>::~MapPartitioned3d() */

void __thiscall MapPartitioned3d<double,int>::~MapPartitioned3d(MapPartitioned3d<double,int> *this)

{
  if (*(long **)(this + 0x140) != (long *)0x0) {
    (**(code **)(**(long **)(this + 0x140) + 0x18))();
    *(undefined8 *)(this + 0x140) = 0;
  }
  if (*(long **)(this + 0x148) != (long *)0x0) {
    (**(code **)(**(long **)(this + 0x148) + 0x18))();
    *(undefined8 *)(this + 0x148) = 0;
  }
  if (*(void **)(this + 0x108) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x108));
  }
  if (*(void **)(this + 0x138) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x138));
  }
  if (*(void **)(this + 0x118) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x118));
  }
  if (*(void **)(this + 0xd8) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0xd8));
  }
  if (*(void **)(this + 0xa0) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0xa0));
  }
  if (*(void **)(this + 0x68) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x68));
  }
  if (*(void **)(this + 0x20) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x20));
  }
  if (*(void **)(this + 0x50) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x50));
  }
  if (*(void **)(this + 0x30) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x30));
    return;
  }
  return;
}


