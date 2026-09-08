/* driver: OBKRJuOFQBpBAkyQXSPAIPP addr=0096de30 size=2d1 */
#include "vsdlss_ref.h"


void OBKRJuOFQBpBAkyQXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  undefined8 uVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long *plVar9;
  FILE *__s;
  size_t sVar10;
  undefined8 *puVar11;
  undefined8 *puVar12;
  undefined8 *puVar13;
  undefined8 *puVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  long local_50 [2];
  long local_40 [2];
  
  lVar3 = *(long *)(param_2 + 0x10);
  lVar4 = *(long *)(param_2 + 0x50);
  lVar5 = *(long *)(param_2 + 0x58);
  lVar6 = *(long *)(param_2 + 0x48);
  local_40[0] = 0;
  lVar17 = lVar3 - param_1;
  lVar18 = lVar17 + 1;
  lVar15 = lVar18;
  if (lVar18 <= lVar3) {
    do {
      lVar7 = *(long *)(lVar6 + lVar15 * 8);
      lVar8 = *(long *)(lVar4 + lVar15 * 8);
      *(long *)(lVar8 + 8) = *(long *)(lVar8 + 8) - lVar17;
      plVar9 = (long *)(lVar8 + 0x10);
      if (1 < lVar7) {
        do {
          local_40[0] = local_40[0] + 1;
          *plVar9 = *plVar9 - lVar17;
          plVar9 = plVar9 + 1;
        } while (plVar9 != (long *)(lVar8 + 8 + lVar7 * 8));
      }
      lVar15 = lVar15 + 1;
    } while (lVar15 != lVar3 + 1);
  }
  local_50[0] = param_1;
  __s = (FILE *)LMBKdFIBuXSPAIPP(0x20);
  sVar10 = fwrite(local_50,8,1,__s);
  if (sVar10 != 1) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xaea,"renumWriteRedMat:WSize",1,sVar10);
  }
  sVar10 = fwrite(local_40,8,1,__s);
  if (sVar10 != 1) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xaec,"renumWriteRedMat:nOffdiagsRed",1,sVar10);
  }
  _yPFdaILPBXSPAIPP(0x20);
  puVar11 = (undefined8 *)KBTtB_QLOXSPAIPP(0,local_50[0],0);
  *puVar11 = 0;
  puVar12 = (undefined8 *)KBTgtB_QLOXSPAIPP(local_40[0],0,0);
  *puVar12 = 0;
  puVar13 = (undefined8 *)KBTgtB_QLOXSPAIPP(local_40[0],0,0);
  *puVar13 = 0;
  puVar14 = (undefined8 *)KBTtB_QLOXSPAIPP(0,local_40[0],0);
  *puVar14 = 0;
  local_40[0] = 0;
  if (lVar18 <= lVar3) {
    do {
      lVar15 = *(long *)(lVar6 + lVar18 * 8);
      lVar7 = *(long *)(lVar4 + lVar18 * 8);
      lVar8 = *(long *)(lVar5 + lVar18 * 8);
      puVar11[lVar18 - lVar17] = *(undefined8 *)(lVar8 + 8);
      if (1 < lVar15) {
        lVar16 = 2;
        do {
          local_40[0] = local_40[0] + 1;
          uVar2 = *(undefined8 *)(lVar8 + lVar16 * 8);
          puVar12[local_40[0]] = lVar18 - lVar17;
          lVar1 = lVar16 * 8;
          lVar16 = lVar16 + 1;
          puVar13[local_40[0]] = *(undefined8 *)(lVar7 + lVar1);
          puVar14[local_40[0]] = uVar2;
        } while (lVar16 != lVar15 + 1);
      }
      lVar18 = lVar18 + 1;
    } while (lVar18 != lVar3 + 1);
  }
  TOFQBbtB_QLOdlRJXSPAIPP(0x21,local_50[0],puVar11);
  _yPFdOBBXSPAIPP(puVar11);
  TOFQBgtB_QLOdlRJXSPAIPP(0x22,local_40[0],puVar12);
  _yPFdOBBXSPAIPP(puVar12);
  TOFQBgtB_QLOdlRJXSPAIPP(0x23,local_40[0],puVar13);
  _yPFdOBBXSPAIPP(puVar13);
  TOFQBbtB_QLOdlRJXSPAIPP(0x24,local_40[0],puVar14);
  _yPFdOBBXSPAIPP(puVar14);
  return;
}


