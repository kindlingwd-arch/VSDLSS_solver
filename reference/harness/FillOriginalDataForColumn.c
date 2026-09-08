/* harness: FillOriginalDataForColumn addr=008d6730 size=94 */
#include "vsdlss_ref.h"


/* cMatrixFit::FillOriginalDataForColumn(std::vector<std::vector<std::complex<double>,
   std::allocator<std::complex<double> > >, std::allocator<std::vector<std::complex<double>,
   std::allocator<std::complex<double> > > > >&, int) */

void __thiscall cMatrixFit::FillOriginalDataForColumn(cMatrixFit *this,vector *param_1,int param_2)

{
  long lVar1;
  undefined8 *puVar2;
  long lVar3;
  long lVar4;
  undefined8 *puVar5;
  long lVar6;
  long *plVar7;
  long lVar8;
  long lVar9;
  
  if (0 < *(int *)(this + 0x78)) {
    lVar9 = 0;
    lVar6 = 0;
    do {
      lVar1 = *(long *)param_1;
      lVar8 = 0;
      lVar3 = 0;
      if (0 < *(int *)(this + 0x7c)) {
        do {
          lVar4 = lVar3 + 1;
          puVar5 = (undefined8 *)(lVar3 * 0x10 + *(long *)(lVar1 + lVar6));
          plVar7 = (long *)(*(long *)(this + 0x640) + lVar8);
          lVar8 = lVar8 + 0x48;
          puVar2 = (undefined8 *)(((*plVar7 + 1) * (long)param_2 + lVar9) * 0x10 + plVar7[3]);
          *puVar5 = *puVar2;
          puVar5[1] = puVar2[1];
          lVar3 = lVar4;
        } while ((int)lVar4 < *(int *)(this + 0x7c));
      }
      lVar9 = lVar9 + 1;
      lVar6 = lVar6 + 0x18;
    } while ((int)lVar9 < *(int *)(this + 0x78));
    return;
  }
  return;
}


