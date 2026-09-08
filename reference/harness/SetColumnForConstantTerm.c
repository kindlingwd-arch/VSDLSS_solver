/* harness: SetColumnForConstantTerm addr=008ff7f0 size=6d */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* cSmatrixFit::SetColumnForConstantTerm() */

void __thiscall cSmatrixFit::SetColumnForConstantTerm(cSmatrixFit *this)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  int iVar4;
  long lVar5;
  
  uVar3 = _DAT_00ab67f8;
  uVar1 = *(undefined8 *)(this + 0x70);
  uVar2 = *(undefined8 *)(this + 0x68);
  if (0 < *(long *)(this + 0x58)) {
    lVar5 = 0;
    iVar4 = 0;
    do {
      while ((*(undefined8 *)
               (*(long *)(this + 0xb0) +
               ((*(long *)(this + 0x98) + 1) * (long)((int)uVar2 + (int)uVar1 * 2) + (long)iVar4) *
               8) = uVar3, lVar5 != 0 || (this[0x78] == (cSmatrixFit)0x0))) {
        iVar4 = iVar4 + 2;
        lVar5 = lVar5 + 1;
        if (*(long *)(this + 0x58) <= lVar5) {
          return;
        }
      }
      iVar4 = iVar4 + 1;
      lVar5 = 1;
    } while (1 < *(long *)(this + 0x58));
  }
  return;
}


