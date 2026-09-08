/* driver: kjbgKFQnyOQFQFLKXSPAIPP addr=0099ac20 size=22f */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void kjbgKFQnyOQFQFLKXSPAIPP(undefined8 param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  int iVar4;
  undefined8 uVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  double dVar13;
  double dVar14;
  undefined1 local_78 [8];
  long local_70;
  long local_68;
  undefined1 local_58 [8];
  long local_50;
  long local_48;
  
  lVar2 = *(long *)(param_3 + 0x10);
  lVar10 = *(long *)(param_3 + 0x50);
  uVar5 = QyDlBTgtB_QLOXSPAIPP(param_1,lVar2,1,0);
  if (((DAT_00e8e648 == 0) || (*(long *)(param_2 + 8) + *(long *)(param_2 + 0x10) < DAT_00e8e640))
     || (lVar8 = lVar2, 0x1e < lVar2)) {
    lVar8 = DAT_00e8e630;
    if (DAT_00e8e610 < lVar2) {
      lVar8 = DAT_00e8e638;
    }
    if (lVar2 < lVar8) {
      lVar8 = lVar2;
    }
  }
  dVar14 = (double)*(long *)(param_2 + 8);
  if (lVar8 < 1) {
    lVar8 = 1;
  }
  dVar13 = (double)*(long *)(param_2 + 0x10);
  local_50 = (long)(DAT_00e8e618 * dVar14);
  if (local_50 < 1) {
    local_50 = 1;
  }
  local_48 = (long)(DAT_00e8e618 * dVar13);
  if (local_48 < 1) {
    local_48 = 1;
  }
  local_70 = (long)(dVar14 * (_DAT_00ab67f8 / DAT_00e8e618));
  if (local_70 < 1) {
    local_70 = 1;
  }
  lVar9 = (long)((_DAT_00ab67f8 / DAT_00e8e618) * dVar13);
  local_68 = 1;
  if (0 < lVar9) {
    local_68 = lVar9;
  }
  if (lVar2 < 1) {
    lVar9 = 1;
  }
  else {
    lVar12 = 1;
    lVar9 = 0;
    do {
      lVar11 = *(long *)(lVar10 + lVar12 * 8);
      if (*(long *)(param_3 + 0x70) == 0) {
        lVar7 = 0;
      }
      else {
        lVar7 = *(long *)(*(long *)(param_3 + 0x70) + lVar12 * 8);
      }
      if (0 < lVar11) {
        lVar11 = lVar11 + 1;
        lVar6 = 1;
        do {
          while (lVar7 != 0) {
            lVar1 = lVar6 * 8;
            lVar6 = lVar6 + 1;
            lVar9 = lVar9 + *(long *)(lVar7 + lVar1);
            if (lVar6 == lVar11) goto LAB_0099ad90;
          }
          lVar6 = lVar6 + 1;
          lVar9 = lVar9 + 1;
        } while (lVar6 != lVar11);
      }
LAB_0099ad90:
      lVar12 = lVar12 + 1;
    } while (lVar12 != lVar2 + 1);
    lVar9 = lVar9 + 1;
  }
  lVar10 = 1;
  do {
    while( true ) {
      iVar4 = rand();
      lVar12 = kjbeOLTpBDFLKZyIyK_BpBCFKBXSPAIPP
                         ((long)(iVar4 >> 3) % lVar2 + 1,param_3,param_2,local_58,local_78,param_4);
      if (lVar9 <= lVar12) break;
      lVar10 = lVar10 + 1;
      _LMVgtB_QLOXSPAIPP(*(undefined8 *)(param_4 + 0xa0),lVar2,uVar5);
      lVar9 = lVar12;
      if (lVar8 < lVar10) goto LAB_0099ae0f;
    }
    lVar10 = lVar10 + 1;
  } while (lVar10 <= lVar8);
LAB_0099ae0f:
  uVar3 = *(undefined8 *)(param_4 + 0xa0);
  *(long *)(param_3 + 0x28) = lVar9;
  _LMVgtB_QLOXSPAIPP(uVar5,lVar2,uVar3);
  return;
}


