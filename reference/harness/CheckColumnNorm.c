/* harness: CheckColumnNorm addr=0090e3c0 size=12c */
#include "vsdlss_ref.h"


/* cColumnNormEnforcer::CheckColumnNorm(double&) */

bool __thiscall cColumnNormEnforcer::CheckColumnNorm(cColumnNormEnforcer *this,double *param_1)

{
  double *pdVar1;
  double dVar2;
  ulong uVar3;
  ulong uVar4;
  double *pdVar5;
  int iVar6;
  double *pdVar7;
  double dVar8;
  
  pdVar5 = *(double **)(this + 200);
  pdVar7 = *(double **)(this + 0xc0);
  uVar4 = (long)pdVar5 - (long)pdVar7 >> 3;
  iVar6 = (int)(*(long *)(this + 0x80) - *(long *)(this + 0x78) >> 3);
  if ((int)uVar4 < iVar6) {
    uVar3 = (ulong)iVar6;
    if (uVar4 < uVar3) {
      std::vector<double,std::allocator<double>>::_M_default_append
                ((vector<double,std::allocator<double>> *)(this + 0xc0),uVar3 - uVar4);
      pdVar7 = *(double **)(this + 0xc0);
      pdVar5 = *(double **)(this + 200);
    }
    else if ((uVar3 < uVar4) && (pdVar1 = pdVar7 + uVar3, pdVar5 != pdVar1)) {
      *(double **)(this + 200) = pdVar1;
      pdVar5 = pdVar1;
    }
  }
  if (pdVar7 == pdVar5) {
    if (*(int *)this < 1) {
      dVar8 = *pdVar7;
      goto LAB_0090e465;
    }
LAB_0090e413:
    iVar6 = 0;
    do {
      iVar6 = iVar6 + 1;
      FillFitVal((int)this);
    } while (iVar6 < *(int *)this);
  }
  else {
    memset(pdVar7,0,(long)pdVar5 - (long)pdVar7);
    if (0 < *(int *)this) goto LAB_0090e413;
  }
  pdVar7 = *(double **)(this + 0xc0);
  if (pdVar7 == *(double **)(this + 200)) {
    dVar8 = *pdVar7;
  }
  else {
    dVar2 = *pdVar7;
    while (dVar8 = dVar2, pdVar7 = pdVar7 + 1, *(double **)(this + 200) != pdVar7) {
      dVar2 = *pdVar7;
      if (*pdVar7 <= dVar8) {
        dVar2 = dVar8;
      }
    }
  }
LAB_0090e465:
  *param_1 = dVar8;
  return dVar8 < *(double *)(this + 0x280) + *(double *)(this + 0x278);
}


