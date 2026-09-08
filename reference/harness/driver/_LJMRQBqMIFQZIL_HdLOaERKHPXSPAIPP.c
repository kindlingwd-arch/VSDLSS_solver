/* driver: _LJMRQBqMIFQZIL_HdLOaERKHPXSPAIPP addr=00985d00 size=1a8 */
#include "vsdlss_ref.h"


void _LJMRQBqMIFQZIL_HdLOaERKHPXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  double dVar5;
  long lVar6;
  ulong uVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  
  dVar5 = DAT_00ab7228;
  lVar1 = *(long *)(param_2 + 0x10);
  if (0 < lVar1) {
    lVar2 = *(long *)(param_2 + 0x48);
    lVar9 = 8;
    lVar3 = *(long *)(param_2 + 0x58);
    do {
      lVar10 = *(long *)(lVar2 + lVar9);
      lVar4 = *(long *)(lVar3 + lVar9);
      if (lVar10 <= lVar4) {
        dVar11 = 0.0;
        lVar6 = lVar10;
        do {
          while( true ) {
            uVar7 = *(ulong *)(*(long *)(param_1 + 0x20) + lVar6 * 8);
            lVar8 = *(long *)(*(long *)(param_1 + 0x18) + lVar6 * 8);
            if ((long)(uVar7 * 8) < 0) {
              dVar12 = (double)((uVar7 & 0x1fffffffffffffff) << 2);
              dVar12 = dVar12 + dVar12;
            }
            else {
              dVar12 = (double)(long)(uVar7 * 8);
            }
            uVar7 = uVar7 * lVar8 - ((lVar8 + -1) * lVar8) / 2;
            lVar8 = uVar7 * 8;
            if (lVar8 < 0) break;
            lVar6 = lVar6 + 1;
            dVar11 = (double)lVar8 + dVar12 + dVar11;
            if (lVar6 == lVar4 + 1) goto LAB_00985df1;
          }
          lVar6 = lVar6 + 1;
          dVar14 = (double)((uVar7 & 0x1fffffffffffffff) << 2);
          dVar11 = dVar14 + dVar14 + dVar12 + dVar11;
        } while (lVar6 != lVar4 + 1);
LAB_00985df1:
        lVar6 = lVar10 * 8;
        dVar12 = 0.0;
        do {
          uVar7 = *(ulong *)(*(long *)(param_1 + 0x20) + lVar6);
          lVar8 = *(long *)(*(long *)(param_1 + 0x18) + lVar6);
          if ((long)(uVar7 * 8) < 0) {
            dVar14 = (double)((uVar7 & 0x1fffffffffffffff) << 2);
            dVar14 = dVar14 + dVar14;
          }
          else {
            dVar14 = (double)(long)(uVar7 * 8);
          }
          uVar7 = uVar7 * lVar8 - ((lVar8 + -1) * lVar8) / 2;
          lVar8 = uVar7 * 8;
          if (lVar8 < 0) {
            dVar13 = (double)((uVar7 & 0x1fffffffffffffff) << 2);
            dVar13 = dVar13 + dVar13;
          }
          else {
            dVar13 = (double)lVar8;
          }
          dVar12 = dVar12 + dVar14 + dVar13;
          if (dVar11 * dVar5 <= dVar12) {
            *(long *)(*(long *)(param_2 + 0x50) + lVar9) = lVar10;
            *(undefined8 *)(*(long *)(param_2 + 0x68) + lVar9) =
                 *(undefined8 *)(*(long *)(param_1 + 0x10) + lVar6);
            break;
          }
          lVar10 = lVar10 + 1;
          lVar6 = lVar6 + 8;
        } while (lVar10 <= lVar4);
      }
      lVar9 = lVar9 + 8;
    } while (lVar9 != lVar1 * 8 + 8);
  }
  return;
}


