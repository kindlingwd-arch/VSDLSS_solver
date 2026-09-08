/* harness: CopyColumn addr=00914780 size=34 */
#include "vsdlss_ref.h"


/* FortranDoubleMatrix::CopyColumn(long, long) */

void __thiscall FortranDoubleMatrix::CopyColumn(FortranDoubleMatrix *this,long param_1,long param_2)

{
  long lVar1;
  
  if (param_1 != param_2) {
    lVar1 = *(long *)this + 1;
    memcpy((void *)(*(long *)(this + 0x18) + param_1 * lVar1 * 8),
           (void *)(*(long *)(this + 0x18) + lVar1 * param_2 * 8),lVar1 * 8);
    return;
  }
  return;
}


