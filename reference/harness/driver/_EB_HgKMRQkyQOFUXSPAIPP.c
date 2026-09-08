/* driver: _EB_HgKMRQkyQOFUXSPAIPP addr=0098a720 size=215 */
#include "vsdlss_ref.h"


void _EB_HgKMRQkyQOFUXSPAIPP(long *param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  bool bVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  double dVar10;
  double local_50;
  double local_48;
  
  lVar1 = param_1[1];
  lVar2 = *param_1;
  lVar3 = param_1[5];
  lVar4 = param_1[7];
  lVar5 = param_1[8];
  lVar9 = param_1[9];
  if (lVar1 < 1) {
    local_50 = 0.0;
  }
  else {
    lVar8 = 1;
    local_50 = 0.0;
    do {
      dVar10 = *(double *)(lVar9 + lVar8 * 8);
      if (dVar10 < 0.0) {
        KOBOOLOXSPAIPP(0x15,"interface.c",0x194);
        dVar10 = *(double *)(lVar9 + lVar8 * 8);
      }
      local_50 = dVar10 + local_50;
      lVar8 = lVar8 + 1;
    } while (lVar8 <= lVar1);
  }
  if (lVar2 < 1) {
    local_48 = 0.0;
  }
  else {
    bVar6 = false;
    lVar9 = 1;
    local_48 = 0.0;
    do {
      lVar8 = *(long *)(lVar3 + lVar9 * 8);
      lVar7 = *(long *)(lVar4 + lVar9 * 8);
      if (lVar8 == lVar7) {
        KOBOOLOXSPAIPP(0x1d,"interface.c",0x19d,lVar8,lVar8,lVar1);
        lVar8 = *(long *)(lVar3 + lVar9 * 8);
        lVar7 = *(long *)(lVar4 + lVar9 * 8);
      }
      if ((lVar8 < 1) || (lVar1 < lVar8)) {
        KOBOOLOXSPAIPP(0x1d,"interface.c",0x1a4,lVar8,lVar7,lVar1);
        lVar7 = *(long *)(lVar4 + lVar9 * 8);
      }
      if ((lVar7 < 1) || (lVar1 < lVar7)) {
        KOBOOLOXSPAIPP(0x1d,"interface.c",0x1ab,*(undefined8 *)(lVar3 + lVar9 * 8),lVar7,lVar1);
      }
      dVar10 = *(double *)(lVar5 + lVar9 * 8);
      if (0.0 < dVar10) {
        if (bVar6) {
          bVar6 = true;
        }
        else {
          bVar6 = true;
          _yPFuyOKFKDXSPAIPP(0x21,"interface.c",0x1b3,*(undefined8 *)(lVar3 + lVar9 * 8),
                             *(undefined8 *)(lVar4 + lVar9 * 8));
          dVar10 = *(double *)(lVar5 + lVar9 * 8);
        }
      }
      lVar9 = lVar9 + 1;
      local_48 = dVar10 + dVar10 + local_48;
    } while (lVar9 <= lVar2);
  }
  if (0.0 < local_50 + local_48) {
    return;
  }
  KOBOOLOXSPAIPP(0x1c,"interface.c",0x1c4);
  return;
}


