/* driver: OBKyJBgKPYKAeBQlLABrLZIL_HXSPAIPP addr=009895b0 size=1a9 */
#include "vsdlss_ref.h"


long OBKyJBgKPYKAeBQlLABrLZIL_HXSPAIPP(long param_1,long *param_2)

{
  long lVar1;
  long *plVar2;
  ulong *puVar3;
  ulong uVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long *plVar9;
  ulong uVar10;
  long lVar11;
  long *plVar12;
  long lVar13;
  
  puVar3 = (ulong *)*param_2;
  uVar4 = *puVar3;
  lVar1 = uVar4 * 8 + 8;
  lVar7 = KBTgtB_QLOXSPAIPP(uVar4,1,0);
  lVar8 = _yPFkyIIL_XSPAIPP(lVar1);
  if (lVar8 == 0) {
    KOBOOLOXSPAIPP(0xd,"indistnodesetsutils.c",0x87,lVar1,"polyNewVectorNoInit()");
  }
  uVar10 = 1;
  if (uVar4 != 0) {
    do {
      lVar11 = uVar10 * 8;
      lVar13 = uVar10 * 8;
      uVar10 = uVar10 + 1;
      *(undefined8 *)(lVar8 + *(long *)(param_1 + lVar13) * 8) = *(undefined8 *)(puVar3[1] + lVar11)
      ;
    } while (uVar10 <= uVar4);
    lVar11 = 8;
    do {
      *(undefined8 *)(puVar3[1] + lVar11) = *(undefined8 *)(lVar8 + lVar11);
      lVar11 = lVar11 + 8;
    } while (lVar1 != lVar11);
  }
  if (lVar8 != 0) {
    _yPFdOBBXSPAIPP(lVar8);
  }
  if ((long)uVar4 < 1) {
    lVar11 = 0;
    lVar8 = ABIBQBgKAFPQlLABqBQPXSPAIPP(puVar3);
  }
  else {
    lVar11 = 0;
    plVar9 = (long *)(puVar3[1] + 8);
    plVar2 = plVar9 + uVar4;
    do {
      plVar12 = (long *)*plVar9;
      if (plVar12 != (long *)0x0) {
        lVar11 = lVar11 + 1;
        do {
          lVar8 = *plVar12;
          plVar12 = (long *)plVar12[1];
          *(long *)(lVar7 + *(long *)(param_1 + lVar8 * 8) * 8) = lVar11;
        } while (plVar12 != (long *)0x0);
      }
      plVar9 = plVar9 + 1;
    } while (plVar9 != plVar2);
    lVar8 = ABIBQBgKAFPQlLABqBQPXSPAIPP(puVar3);
    if (1 < (long)uVar4) {
      lVar13 = 2;
      do {
        lVar5 = *(long *)(lVar7 + lVar13 * 8);
        lVar6 = *(long *)(lVar7 + -8 + lVar13 * 8);
        if ((lVar5 != lVar6) && (lVar5 != lVar6 + 1)) {
          KOBOOLOXSPAIPP(1,"indistnodesetsutils.c",0x9e);
        }
        lVar13 = lVar13 + 1;
      } while (lVar13 <= (long)uVar4);
    }
  }
  if (*(long *)(lVar7 + -8 + lVar1) != lVar11) {
    KOBOOLOXSPAIPP(1,"indistnodesetsutils.c",0xa3);
  }
  *param_2 = lVar8;
  return lVar7;
}


