/* VSDLSS function (decompiled by Ghidra) */
/* name: factorAOneChunk_vsdlss  addr: 00a194b0  size: 5 */
#include "vsdlss_ref.h"

/* signature: void factorAOneChunk_vsdlss(int param_1,int param_2,long param_3); */

void factorAOneChunk_vsdlss(int param_1,int param_2,long param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar4 = 1;
  piVar5 = *(int **)(param_3 + 0x20);
  iVar1 = 1;
  iVar3 = *(int *)(param_3 + 0x14) - param_2;
  if (param_1 == 0) {
    iVar3 = *(int *)(param_3 + 0x14);
  }
  if (0 < iVar3) {
    do {
      piVar5 = piVar5 + 1;
      iVar2 = *piVar5;
      switch(iVar2) {
      default:
        iVar7 = iVar1;
        for (iVar8 = iVar2; 0 < iVar8; iVar8 = iVar8 - iVar6) {
          iVar6 = 6;
          if (iVar8 < 7) {
            iVar6 = iVar8;
          }
          switch(iVar6) {
          default:
            sub1RowContribFromRest_vsdlss(iVar7,param_3);
            break;
          case 2:
            sub2RowContribFromRest_vsdlss(iVar7,param_3);
            break;
          case 3:
            sub3RowContribFromRest_vsdlss(iVar7,param_3);
            break;
          case 4:
            sub4RowContribFromRest(iVar7,param_3);
            break;
          case 5:
            sub5RowContribFromRest(iVar7,param_3);
            break;
          case 6:
            sub6RowContribFromRest(iVar7,param_3);
          }
          iVar7 = iVar7 + iVar6;
        }
        break;
      case 1:
        sub1RowContribFromRest_vsdlss(iVar1,param_3);
        break;
      case 2:
        sub2RowContribFromRest_vsdlss(iVar1,param_3);
        break;
      case 3:
        sub3RowContribFromRest_vsdlss(iVar1,param_3);
        break;
      case 4:
        sub4RowContribFromRest(iVar1,param_3);
        break;
      case 5:
        sub5RowContribFromRest(iVar1,param_3);
        break;
      case 6:
        sub6RowContribFromRest(iVar1,param_3);
      }
      iVar1 = iVar1 + iVar2;
      iVar4 = iVar4 + 1;
    } while (iVar4 <= iVar3);
  }
  return;
}


