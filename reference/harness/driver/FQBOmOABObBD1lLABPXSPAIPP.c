/* driver: FQBOmOABObBD1lLABPXSPAIPP addr=009aa080 size=2d3 */
#include "vsdlss_ref.h"


void FQBOmOABObBD1lLABPXSPAIPP
               (long param_1,long *param_2,long *param_3,long param_4,long param_5,long param_6)

{
  ulong *puVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  ulong uVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long local_80;
  
  lVar15 = *param_2;
  lVar2 = *(long *)(param_1 + 0x10);
  lVar3 = *(long *)(param_1 + 8);
  lVar4 = *(long *)(param_1 + 0x18);
  lVar10 = KBTgtB_QLOXSPAIPP(lVar3,0,0);
  if (lVar3 < 1) {
    local_80 = 0;
  }
  else {
    local_80 = 0;
    lVar13 = 1;
    lVar16 = 1;
    do {
      while ((*(char *)(param_6 + lVar13) == '\0' &&
             (uVar11 = *(ulong *)(param_5 + lVar13 * 8), uVar11 < 2))) {
        local_80 = local_80 + (ulong)(uVar11 == 0);
        if (*(long *)(lVar4 + lVar13 * 8) == 1) {
          local_80 = local_80 +
                     (ulong)(*(long *)(param_5 + *(long *)(*(long *)(lVar2 + lVar13 * 8) + 8) * 8)
                            == -1);
        }
        if (lVar3 < lVar16) {
          KOBOOLOXSPAIPP(1,"reduceproblem.c",0xd0);
        }
        *(long *)(lVar10 + lVar16 * 8) = lVar13;
        *(undefined8 *)(param_5 + lVar13 * 8) = 0xffffffffffffffff;
        lVar13 = lVar13 + 1;
        lVar16 = lVar16 + 1;
        if (lVar3 < lVar13) goto LAB_009aa15f;
      }
      lVar13 = lVar13 + 1;
    } while (lVar13 <= lVar3);
LAB_009aa15f:
    if (lVar16 != 1) {
      lVar13 = 1;
      do {
        lVar5 = *(long *)(lVar10 + lVar13 * 8);
        lVar15 = lVar15 + 1;
        lVar13 = lVar13 + 1;
        *(long *)(param_4 + lVar15 * 8) = lVar5;
        lVar6 = *(long *)(lVar4 + lVar5 * 8);
        lVar5 = *(long *)(lVar2 + lVar5 * 8);
        if (0 < lVar6) {
          lVar14 = 1;
LAB_009aa1cd:
          do {
            lVar7 = *(long *)(lVar5 + lVar14 * 8);
            puVar1 = (ulong *)(param_5 + lVar7 * 8);
            if (*puVar1 != 0xffffffffffffffff) {
              uVar11 = *puVar1 - 1;
              *puVar1 = uVar11;
              if ((*(char *)(param_6 + lVar7) == '\0') && (uVar11 < 2)) {
                lVar8 = *(long *)(lVar4 + lVar7 * 8);
                lVar9 = *(long *)(lVar2 + lVar7 * 8);
                if (lVar8 < 1) {
LAB_009aa235:
                  local_80 = local_80 + 1;
                }
                else if (*(long *)(param_5 + *(long *)(lVar9 + 8) * 8) == -1) {
                  lVar12 = 1;
                  do {
                    lVar12 = lVar12 + 1;
                    if (lVar8 < lVar12) goto LAB_009aa235;
                  } while (*(long *)(param_5 + *(long *)(lVar9 + lVar12 * 8) * 8) == -1);
                }
                if (lVar3 < lVar16) {
                  KOBOOLOXSPAIPP(1,"reduceproblem.c",0xf9);
                }
                lVar14 = lVar14 + 1;
                *(long *)(lVar10 + lVar16 * 8) = lVar7;
                lVar16 = lVar16 + 1;
                *puVar1 = 0xffffffffffffffff;
                if (lVar6 < lVar14) break;
                goto LAB_009aa1cd;
              }
            }
            lVar14 = lVar14 + 1;
          } while (lVar14 <= lVar6);
        }
      } while (lVar13 != lVar16);
    }
  }
  if (lVar10 != 0) {
    _yPFdOBBXSPAIPP(lVar10);
  }
  OBJLSBcIFJlLABPdOLJZR_HBQP(param_5,param_1);
  *param_2 = lVar15;
  *param_3 = local_80;
  return;
}


