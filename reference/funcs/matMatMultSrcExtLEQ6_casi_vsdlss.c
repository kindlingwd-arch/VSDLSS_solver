/* VSDLSS function (decompiled by Ghidra) */
/* name: matMatMultSrcExtLEQ6_casi_vsdlss  addr: 009ec860  size: 99d */
#include "vsdlss_ref.h"

/* signature: void matMatMultSrcExtLEQ6_casi_vsdlss(long param_1,int param_2,int param_3,long param_4,int param_5,double *param_6); */

void matMatMultSrcExtLEQ6_casi_vsdlss
               (long param_1,int param_2,int param_3,long param_4,int param_5,double *param_6)

{
  long lVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  long lVar6;
  int iVar7;
  long lVar8;
  long lVar9;
  double *pdVar10;
  int iVar11;
  long lVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  double dVar17;
  int local_54;
  
  iVar7 = param_5 % 3;
  if (0 < param_2) {
    lVar6 = (long)param_3;
    pdVar10 = (double *)(param_4 + 8);
    lVar1 = lVar6 * 8;
    local_54 = 1;
    dVar17 = 0.0;
    do {
      lVar8 = param_1;
      if (0 < param_5 - iVar7) {
        lVar12 = lVar6 * -8;
        iVar11 = 1;
        lVar9 = param_1;
        do {
          lVar9 = lVar9 + lVar6 * 0x18;
          switch(param_3) {
          default:
            nrerror_vsdlss(1,"factora.c",0x8a9);
            dVar14 = dVar17;
            dVar15 = dVar17;
            dVar16 = dVar17;
            break;
          case 1:
            dVar14 = *pdVar10;
            dVar16 = *(double *)(lVar8 + 8) * dVar14;
            dVar15 = *(double *)(lVar8 + 8 + lVar1) * dVar14;
            dVar14 = dVar14 * *(double *)(lVar9 + 8 + lVar12);
            break;
          case 2:
            dVar14 = *pdVar10;
            dVar13 = pdVar10[1];
            dVar16 = *(double *)(lVar8 + 0x10) * dVar13 + *(double *)(lVar8 + 8) * dVar14;
            dVar15 = *(double *)(lVar8 + 0x10 + lVar1) * dVar13 +
                     *(double *)(lVar8 + 8 + lVar1) * dVar14;
            dVar14 = dVar13 * *(double *)(lVar9 + 0x10 + lVar12) +
                     *(double *)(lVar9 + 8 + lVar12) * dVar14;
            break;
          case 3:
            dVar14 = *pdVar10;
            dVar13 = pdVar10[1];
            dVar2 = pdVar10[2];
            dVar16 = *(double *)(lVar8 + 0x18) * dVar2 +
                     *(double *)(lVar8 + 0x10) * dVar13 + *(double *)(lVar8 + 8) * dVar14;
            dVar15 = *(double *)(lVar8 + 0x18 + lVar1) * dVar2 +
                     *(double *)(lVar8 + 0x10 + lVar1) * dVar13 +
                     *(double *)(lVar8 + 8 + lVar1) * dVar14;
            dVar14 = dVar2 * *(double *)(lVar9 + 0x18 + lVar12) +
                     dVar13 * *(double *)(lVar9 + 0x10 + lVar12) +
                     dVar14 * *(double *)(lVar9 + 8 + lVar12);
            break;
          case 4:
            dVar14 = *pdVar10;
            dVar13 = pdVar10[1];
            dVar2 = pdVar10[2];
            dVar3 = pdVar10[3];
            dVar16 = *(double *)(lVar8 + 0x20) * dVar3 +
                     *(double *)(lVar8 + 0x18) * dVar2 +
                     *(double *)(lVar8 + 0x10) * dVar13 + *(double *)(lVar8 + 8) * dVar14;
            dVar15 = *(double *)(lVar8 + 0x20 + lVar1) * dVar3 +
                     *(double *)(lVar8 + 0x18 + lVar1) * dVar2 +
                     *(double *)(lVar8 + 0x10 + lVar1) * dVar13 +
                     *(double *)(lVar8 + 8 + lVar1) * dVar14;
            dVar14 = dVar3 * *(double *)(lVar9 + 0x20 + lVar12) +
                     dVar2 * *(double *)(lVar9 + 0x18 + lVar12) +
                     dVar13 * *(double *)(lVar9 + 0x10 + lVar12) +
                     *(double *)(lVar9 + 8 + lVar12) * dVar14;
            break;
          case 5:
            dVar14 = *pdVar10;
            dVar13 = pdVar10[1];
            dVar2 = pdVar10[2];
            dVar3 = pdVar10[3];
            dVar4 = pdVar10[4];
            dVar16 = *(double *)(lVar8 + 0x28) * dVar4 +
                     *(double *)(lVar8 + 0x20) * dVar3 +
                     *(double *)(lVar8 + 0x18) * dVar2 +
                     *(double *)(lVar8 + 0x10) * dVar13 + *(double *)(lVar8 + 8) * dVar14;
            dVar15 = *(double *)(lVar8 + 0x28 + lVar1) * dVar4 +
                     *(double *)(lVar8 + 0x20 + lVar1) * dVar3 +
                     *(double *)(lVar8 + 0x18 + lVar1) * dVar2 +
                     *(double *)(lVar8 + 0x10 + lVar1) * dVar13 +
                     *(double *)(lVar8 + 8 + lVar1) * dVar14;
            dVar14 = dVar4 * *(double *)(lVar9 + 0x28 + lVar12) +
                     dVar3 * *(double *)(lVar9 + 0x20 + lVar12) +
                     dVar2 * *(double *)(lVar9 + 0x18 + lVar12) +
                     dVar13 * *(double *)(lVar9 + 0x10 + lVar12) +
                     dVar14 * *(double *)(lVar9 + 8 + lVar12);
            break;
          case 6:
            dVar14 = *pdVar10;
            dVar13 = pdVar10[1];
            dVar2 = pdVar10[2];
            dVar3 = pdVar10[3];
            dVar4 = pdVar10[4];
            dVar5 = pdVar10[5];
            dVar16 = *(double *)(lVar8 + 0x30) * dVar5 +
                     *(double *)(lVar8 + 0x28) * dVar4 +
                     *(double *)(lVar8 + 0x20) * dVar3 +
                     *(double *)(lVar8 + 0x18) * dVar2 +
                     *(double *)(lVar8 + 0x10) * dVar13 + *(double *)(lVar8 + 8) * dVar14;
            dVar15 = *(double *)(lVar8 + 0x30 + lVar1) * dVar5 +
                     *(double *)(lVar8 + 0x28 + lVar1) * dVar4 +
                     *(double *)(lVar8 + 0x20 + lVar1) * dVar3 +
                     *(double *)(lVar8 + 0x18 + lVar1) * dVar2 +
                     *(double *)(lVar8 + 0x10 + lVar1) * dVar13 +
                     *(double *)(lVar8 + 8 + lVar1) * dVar14;
            dVar14 = dVar5 * *(double *)(lVar9 + 0x30 + lVar12) +
                     dVar4 * *(double *)(lVar9 + 0x28 + lVar12) +
                     dVar3 * *(double *)(lVar9 + 0x20 + lVar12) +
                     dVar2 * *(double *)(lVar9 + 0x18 + lVar12) +
                     dVar13 * *(double *)(lVar9 + 0x10 + lVar12) +
                     *(double *)(lVar9 + 8 + lVar12) * dVar14;
          }
          iVar11 = iVar11 + 3;
          param_6[1] = dVar16;
          lVar8 = lVar8 + lVar6 * 0x18;
          param_6[2] = dVar15;
          param_6 = param_6 + 3;
          *param_6 = dVar14;
        } while (iVar11 <= param_5 - iVar7);
      }
      if (iVar7 == 1) {
        switch(param_3) {
        default:
          nrerror_vsdlss(1,"factora.c",0x8df);
          dVar14 = 0.0;
          break;
        case 1:
          dVar14 = *pdVar10 * *(double *)(lVar8 + 8);
          break;
        case 2:
          dVar14 = pdVar10[1] * *(double *)(lVar8 + 0x10) + *pdVar10 * *(double *)(lVar8 + 8);
          break;
        case 3:
          dVar14 = pdVar10[2] * *(double *)(lVar8 + 0x18) +
                   pdVar10[1] * *(double *)(lVar8 + 0x10) + *pdVar10 * *(double *)(lVar8 + 8);
          break;
        case 4:
          dVar14 = pdVar10[3] * *(double *)(lVar8 + 0x20) +
                   pdVar10[2] * *(double *)(lVar8 + 0x18) +
                   pdVar10[1] * *(double *)(lVar8 + 0x10) + *pdVar10 * *(double *)(lVar8 + 8);
          break;
        case 5:
          dVar14 = pdVar10[4] * *(double *)(lVar8 + 0x28) +
                   pdVar10[3] * *(double *)(lVar8 + 0x20) +
                   pdVar10[2] * *(double *)(lVar8 + 0x18) +
                   pdVar10[1] * *(double *)(lVar8 + 0x10) + *pdVar10 * *(double *)(lVar8 + 8);
          break;
        case 6:
          dVar14 = pdVar10[5] * *(double *)(lVar8 + 0x30) +
                   pdVar10[4] * *(double *)(lVar8 + 0x28) +
                   pdVar10[3] * *(double *)(lVar8 + 0x20) +
                   pdVar10[2] * *(double *)(lVar8 + 0x18) +
                   pdVar10[1] * *(double *)(lVar8 + 0x10) + *pdVar10 * *(double *)(lVar8 + 8);
        }
        param_6[1] = dVar14;
        param_6 = param_6 + 1;
      }
      else if (iVar7 == 2) {
        lVar9 = lVar8 + lVar1;
        switch(param_3) {
        default:
          nrerror_vsdlss(1,"factora.c",0x923);
          dVar13 = 0.0;
          dVar14 = dVar13;
          break;
        case 1:
          dVar14 = *(double *)(lVar8 + 8) * *pdVar10;
          dVar13 = *pdVar10 * *(double *)(lVar9 + 8);
          break;
        case 2:
          dVar14 = *(double *)(lVar8 + 0x10) * pdVar10[1] + *(double *)(lVar8 + 8) * *pdVar10;
          dVar13 = pdVar10[1] * *(double *)(lVar9 + 0x10) + *(double *)(lVar9 + 8) * *pdVar10;
          break;
        case 3:
          dVar14 = *(double *)(lVar8 + 0x18) * pdVar10[2] +
                   *(double *)(lVar8 + 0x10) * pdVar10[1] + *(double *)(lVar8 + 8) * *pdVar10;
          dVar13 = pdVar10[2] * *(double *)(lVar9 + 0x18) +
                   pdVar10[1] * *(double *)(lVar9 + 0x10) + *pdVar10 * *(double *)(lVar9 + 8);
          break;
        case 4:
          dVar14 = *(double *)(lVar8 + 0x20) * pdVar10[3] +
                   *(double *)(lVar8 + 0x18) * pdVar10[2] +
                   *(double *)(lVar8 + 0x10) * pdVar10[1] + *(double *)(lVar8 + 8) * *pdVar10;
          dVar13 = pdVar10[3] * *(double *)(lVar9 + 0x20) +
                   pdVar10[2] * *(double *)(lVar9 + 0x18) +
                   pdVar10[1] * *(double *)(lVar9 + 0x10) + *(double *)(lVar9 + 8) * *pdVar10;
          break;
        case 5:
          dVar14 = *(double *)(lVar8 + 0x28) * pdVar10[4] +
                   *(double *)(lVar8 + 0x20) * pdVar10[3] +
                   *(double *)(lVar8 + 0x18) * pdVar10[2] +
                   *(double *)(lVar8 + 0x10) * pdVar10[1] + *(double *)(lVar8 + 8) * *pdVar10;
          dVar13 = pdVar10[4] * *(double *)(lVar9 + 0x28) +
                   pdVar10[3] * *(double *)(lVar9 + 0x20) +
                   pdVar10[2] * *(double *)(lVar9 + 0x18) +
                   pdVar10[1] * *(double *)(lVar9 + 0x10) + *pdVar10 * *(double *)(lVar9 + 8);
          break;
        case 6:
          dVar14 = *(double *)(lVar8 + 0x30) * pdVar10[5] +
                   *(double *)(lVar8 + 0x28) * pdVar10[4] +
                   *(double *)(lVar8 + 0x20) * pdVar10[3] +
                   *(double *)(lVar8 + 0x18) * pdVar10[2] +
                   *(double *)(lVar8 + 0x10) * pdVar10[1] + *(double *)(lVar8 + 8) * *pdVar10;
          dVar13 = pdVar10[5] * *(double *)(lVar9 + 0x30) +
                   pdVar10[4] * *(double *)(lVar9 + 0x28) +
                   pdVar10[3] * *(double *)(lVar9 + 0x20) +
                   pdVar10[2] * *(double *)(lVar9 + 0x18) +
                   pdVar10[1] * *(double *)(lVar9 + 0x10) + *(double *)(lVar9 + 8) * *pdVar10;
        }
        param_6[1] = dVar14;
        param_6 = param_6 + 2;
        *param_6 = dVar13;
      }
      local_54 = local_54 + 1;
      pdVar10 = pdVar10 + lVar6;
    } while (local_54 <= param_2);
  }
  return;
}


