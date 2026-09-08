/* harness: SetModifiedColumn addr=0090c410 size=fe */
#include "vsdlss_ref.h"


/* cColumnNormEnforcer::SetModifiedColumn(RationalApproxMatrix&, int) */

void __thiscall
cColumnNormEnforcer::SetModifiedColumn
          (cColumnNormEnforcer *this,RationalApproxMatrix *param_1,int param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  if (0 < *(int *)this) {
    lVar4 = 0;
    do {
      lVar2 = lVar4 * 0x40;
      PoleResidueForm::Denormalize
                ((PoleResidueForm *)(*(long *)(this + 0x10) + lVar2),*(double *)(this + 0x58));
      lVar3 = *(long *)(this + 0x10) + lVar2;
      lVar1 = ((long)param_2 * *(long *)param_1 + lVar4) * 0x40 + *(long *)(param_1 + 0x10);
      FUN_0090b840(lVar1,lVar3);
      FUN_0090b840(lVar1 + 0x18,lVar3 + 0x18);
      *(undefined8 *)(lVar1 + 0x30) = *(undefined8 *)(lVar3 + 0x30);
      *(undefined8 *)(lVar1 + 0x38) = *(undefined8 *)(lVar3 + 0x38);
      if (param_2 != (int)lVar4) {
        lVar2 = lVar2 + *(long *)(this + 0x10);
        lVar1 = ((long)param_2 + *(long *)param_1 * lVar4) * 0x40 + *(long *)(param_1 + 0x10);
        FUN_0090b840(lVar1,lVar2);
        FUN_0090b840(lVar1 + 0x18,lVar2 + 0x18);
        *(undefined8 *)(lVar1 + 0x30) = *(undefined8 *)(lVar2 + 0x30);
        *(undefined8 *)(lVar1 + 0x38) = *(undefined8 *)(lVar2 + 0x38);
      }
      lVar4 = lVar4 + 1;
    } while ((int)lVar4 < *(int *)this);
  }
  return;
}


