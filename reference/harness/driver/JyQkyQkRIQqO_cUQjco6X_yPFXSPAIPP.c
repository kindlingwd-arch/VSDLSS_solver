/* driver: JyQkyQkRIQqO_cUQjco6X_yPFXSPAIPP addr=0097eeb0 size=9a7 */
#include "vsdlss_ref.h"


void JyQkyQkRIQqO_cUQjco6X_yPFXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,double *param_6)

{
  long lVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  double *pdVar9;
  long lVar10;
  long lVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  long local_60;
  
  lVar6 = param_5 % 3;
  if (0 < param_2) {
    lVar1 = param_3 * 8;
    pdVar9 = (double *)(param_4 + 8);
    dVar16 = 0.0;
    local_60 = 1;
    do {
      lVar7 = param_1;
      if (0 < param_5 - lVar6) {
        lVar10 = param_3 * -8;
        lVar11 = 1;
        lVar8 = param_1;
        do {
          lVar8 = lVar8 + param_3 * 0x18;
          switch(param_3) {
          default:
            KOBOOLOXSPAIPP(1,"factora.c",0x8a9);
            dVar13 = dVar16;
            dVar14 = dVar16;
            dVar15 = dVar16;
            break;
          case 1:
            dVar13 = *pdVar9;
            dVar15 = *(double *)(lVar7 + 8) * dVar13;
            dVar14 = *(double *)(lVar7 + 8 + lVar1) * dVar13;
            dVar13 = dVar13 * *(double *)(lVar8 + 8 + lVar10);
            break;
          case 2:
            dVar13 = *pdVar9;
            dVar12 = pdVar9[1];
            dVar15 = *(double *)(lVar7 + 0x10) * dVar12 + *(double *)(lVar7 + 8) * dVar13;
            dVar14 = *(double *)(lVar7 + 0x10 + lVar1) * dVar12 +
                     *(double *)(lVar7 + 8 + lVar1) * dVar13;
            dVar13 = dVar12 * *(double *)(lVar8 + 0x10 + lVar10) +
                     *(double *)(lVar8 + 8 + lVar10) * dVar13;
            break;
          case 3:
            dVar13 = *pdVar9;
            dVar12 = pdVar9[1];
            dVar2 = pdVar9[2];
            dVar15 = *(double *)(lVar7 + 0x18) * dVar2 +
                     *(double *)(lVar7 + 0x10) * dVar12 + *(double *)(lVar7 + 8) * dVar13;
            dVar14 = *(double *)(lVar7 + 0x18 + lVar1) * dVar2 +
                     *(double *)(lVar7 + 0x10 + lVar1) * dVar12 +
                     *(double *)(lVar7 + 8 + lVar1) * dVar13;
            dVar13 = dVar2 * *(double *)(lVar8 + 0x18 + lVar10) +
                     dVar12 * *(double *)(lVar8 + 0x10 + lVar10) +
                     dVar13 * *(double *)(lVar8 + 8 + lVar10);
            break;
          case 4:
            dVar13 = *pdVar9;
            dVar12 = pdVar9[1];
            dVar2 = pdVar9[2];
            dVar3 = pdVar9[3];
            dVar15 = *(double *)(lVar7 + 0x20) * dVar3 +
                     *(double *)(lVar7 + 0x18) * dVar2 +
                     *(double *)(lVar7 + 0x10) * dVar12 + *(double *)(lVar7 + 8) * dVar13;
            dVar14 = *(double *)(lVar7 + 0x20 + lVar1) * dVar3 +
                     *(double *)(lVar7 + 0x18 + lVar1) * dVar2 +
                     *(double *)(lVar7 + 0x10 + lVar1) * dVar12 +
                     *(double *)(lVar7 + 8 + lVar1) * dVar13;
            dVar13 = dVar3 * *(double *)(lVar8 + 0x20 + lVar10) +
                     dVar2 * *(double *)(lVar8 + 0x18 + lVar10) +
                     dVar12 * *(double *)(lVar8 + 0x10 + lVar10) +
                     *(double *)(lVar8 + 8 + lVar10) * dVar13;
            break;
          case 5:
            dVar13 = *pdVar9;
            dVar12 = pdVar9[1];
            dVar2 = pdVar9[2];
            dVar3 = pdVar9[3];
            dVar4 = pdVar9[4];
            dVar15 = *(double *)(lVar7 + 0x28) * dVar4 +
                     *(double *)(lVar7 + 0x20) * dVar3 +
                     *(double *)(lVar7 + 0x18) * dVar2 +
                     *(double *)(lVar7 + 0x10) * dVar12 + *(double *)(lVar7 + 8) * dVar13;
            dVar14 = *(double *)(lVar7 + 0x28 + lVar1) * dVar4 +
                     *(double *)(lVar7 + 0x20 + lVar1) * dVar3 +
                     *(double *)(lVar7 + 0x18 + lVar1) * dVar2 +
                     *(double *)(lVar7 + 0x10 + lVar1) * dVar12 +
                     *(double *)(lVar7 + 8 + lVar1) * dVar13;
            dVar13 = dVar4 * *(double *)(lVar8 + 0x28 + lVar10) +
                     dVar3 * *(double *)(lVar8 + 0x20 + lVar10) +
                     dVar2 * *(double *)(lVar8 + 0x18 + lVar10) +
                     dVar12 * *(double *)(lVar8 + 0x10 + lVar10) +
                     dVar13 * *(double *)(lVar8 + 8 + lVar10);
            break;
          case 6:
            dVar13 = *pdVar9;
            dVar12 = pdVar9[1];
            dVar2 = pdVar9[2];
            dVar3 = pdVar9[3];
            dVar4 = pdVar9[4];
            dVar5 = pdVar9[5];
            dVar15 = *(double *)(lVar7 + 0x30) * dVar5 +
                     *(double *)(lVar7 + 0x28) * dVar4 +
                     *(double *)(lVar7 + 0x20) * dVar3 +
                     *(double *)(lVar7 + 0x18) * dVar2 +
                     *(double *)(lVar7 + 0x10) * dVar12 + *(double *)(lVar7 + 8) * dVar13;
            dVar14 = *(double *)(lVar7 + 0x30 + lVar1) * dVar5 +
                     *(double *)(lVar7 + 0x28 + lVar1) * dVar4 +
                     *(double *)(lVar7 + 0x20 + lVar1) * dVar3 +
                     *(double *)(lVar7 + 0x18 + lVar1) * dVar2 +
                     *(double *)(lVar7 + 0x10 + lVar1) * dVar12 +
                     *(double *)(lVar7 + 8 + lVar1) * dVar13;
            dVar13 = dVar5 * *(double *)(lVar8 + 0x30 + lVar10) +
                     dVar4 * *(double *)(lVar8 + 0x28 + lVar10) +
                     dVar3 * *(double *)(lVar8 + 0x20 + lVar10) +
                     dVar2 * *(double *)(lVar8 + 0x18 + lVar10) +
                     dVar12 * *(double *)(lVar8 + 0x10 + lVar10) +
                     *(double *)(lVar8 + 8 + lVar10) * dVar13;
          }
          lVar11 = lVar11 + 3;
          param_6[1] = dVar15;
          lVar7 = lVar7 + param_3 * 0x18;
          param_6[2] = dVar14;
          param_6 = param_6 + 3;
          *param_6 = dVar13;
        } while (lVar11 <= param_5 - lVar6);
      }
      if (lVar6 == 1) {
        switch(param_3) {
        default:
          KOBOOLOXSPAIPP(1,"factora.c",0x8df);
          dVar13 = 0.0;
          break;
        case 1:
          dVar13 = *pdVar9 * *(double *)(lVar7 + 8);
          break;
        case 2:
          dVar13 = pdVar9[1] * *(double *)(lVar7 + 0x10) + *pdVar9 * *(double *)(lVar7 + 8);
          break;
        case 3:
          dVar13 = pdVar9[2] * *(double *)(lVar7 + 0x18) +
                   pdVar9[1] * *(double *)(lVar7 + 0x10) + *pdVar9 * *(double *)(lVar7 + 8);
          break;
        case 4:
          dVar13 = pdVar9[3] * *(double *)(lVar7 + 0x20) +
                   pdVar9[2] * *(double *)(lVar7 + 0x18) +
                   pdVar9[1] * *(double *)(lVar7 + 0x10) + *pdVar9 * *(double *)(lVar7 + 8);
          break;
        case 5:
          dVar13 = pdVar9[4] * *(double *)(lVar7 + 0x28) +
                   pdVar9[3] * *(double *)(lVar7 + 0x20) +
                   pdVar9[2] * *(double *)(lVar7 + 0x18) +
                   pdVar9[1] * *(double *)(lVar7 + 0x10) + *pdVar9 * *(double *)(lVar7 + 8);
          break;
        case 6:
          dVar13 = pdVar9[5] * *(double *)(lVar7 + 0x30) +
                   pdVar9[4] * *(double *)(lVar7 + 0x28) +
                   pdVar9[3] * *(double *)(lVar7 + 0x20) +
                   pdVar9[2] * *(double *)(lVar7 + 0x18) +
                   pdVar9[1] * *(double *)(lVar7 + 0x10) + *pdVar9 * *(double *)(lVar7 + 8);
        }
        param_6[1] = dVar13;
        param_6 = param_6 + 1;
      }
      else if (lVar6 == 2) {
        lVar8 = lVar7 + lVar1;
        switch(param_3) {
        default:
          KOBOOLOXSPAIPP(1,"factora.c",0x923);
          dVar12 = 0.0;
          dVar13 = dVar12;
          break;
        case 1:
          dVar13 = *(double *)(lVar7 + 8) * *pdVar9;
          dVar12 = *pdVar9 * *(double *)(lVar8 + 8);
          break;
        case 2:
          dVar13 = *(double *)(lVar7 + 0x10) * pdVar9[1] + *(double *)(lVar7 + 8) * *pdVar9;
          dVar12 = pdVar9[1] * *(double *)(lVar8 + 0x10) + *(double *)(lVar8 + 8) * *pdVar9;
          break;
        case 3:
          dVar13 = *(double *)(lVar7 + 0x18) * pdVar9[2] +
                   *(double *)(lVar7 + 0x10) * pdVar9[1] + *(double *)(lVar7 + 8) * *pdVar9;
          dVar12 = pdVar9[2] * *(double *)(lVar8 + 0x18) +
                   pdVar9[1] * *(double *)(lVar8 + 0x10) + *pdVar9 * *(double *)(lVar8 + 8);
          break;
        case 4:
          dVar13 = *(double *)(lVar7 + 0x20) * pdVar9[3] +
                   *(double *)(lVar7 + 0x18) * pdVar9[2] +
                   *(double *)(lVar7 + 0x10) * pdVar9[1] + *(double *)(lVar7 + 8) * *pdVar9;
          dVar12 = pdVar9[3] * *(double *)(lVar8 + 0x20) +
                   pdVar9[2] * *(double *)(lVar8 + 0x18) +
                   pdVar9[1] * *(double *)(lVar8 + 0x10) + *(double *)(lVar8 + 8) * *pdVar9;
          break;
        case 5:
          dVar13 = *(double *)(lVar7 + 0x28) * pdVar9[4] +
                   *(double *)(lVar7 + 0x20) * pdVar9[3] +
                   *(double *)(lVar7 + 0x18) * pdVar9[2] +
                   *(double *)(lVar7 + 0x10) * pdVar9[1] + *(double *)(lVar7 + 8) * *pdVar9;
          dVar12 = pdVar9[4] * *(double *)(lVar8 + 0x28) +
                   pdVar9[3] * *(double *)(lVar8 + 0x20) +
                   pdVar9[2] * *(double *)(lVar8 + 0x18) +
                   pdVar9[1] * *(double *)(lVar8 + 0x10) + *pdVar9 * *(double *)(lVar8 + 8);
          break;
        case 6:
          dVar13 = *(double *)(lVar7 + 0x30) * pdVar9[5] +
                   *(double *)(lVar7 + 0x28) * pdVar9[4] +
                   *(double *)(lVar7 + 0x20) * pdVar9[3] +
                   *(double *)(lVar7 + 0x18) * pdVar9[2] +
                   *(double *)(lVar7 + 0x10) * pdVar9[1] + *(double *)(lVar7 + 8) * *pdVar9;
          dVar12 = pdVar9[5] * *(double *)(lVar8 + 0x30) +
                   pdVar9[4] * *(double *)(lVar8 + 0x28) +
                   pdVar9[3] * *(double *)(lVar8 + 0x20) +
                   pdVar9[2] * *(double *)(lVar8 + 0x18) +
                   pdVar9[1] * *(double *)(lVar8 + 0x10) + *(double *)(lVar8 + 8) * *pdVar9;
        }
        param_6[1] = dVar13;
        param_6 = param_6 + 2;
        *param_6 = dVar12;
      }
      local_60 = local_60 + 1;
      pdVar9 = pdVar9 + param_3;
    } while (local_60 <= param_2);
  }
  return;
}


