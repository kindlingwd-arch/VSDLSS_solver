/* driver: PLISBjLTBOrOFyKDnyOQdy_QXSPAIPP addr=009a26d0 size=246 */
#include "vsdlss_ref.h"


void PLISBjLTBOrOFyKDnyOQdy_QXSPAIPP(char *param_1,long param_2,long param_3,undefined8 param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  ulong uVar10;
  uint uVar11;
  uint uVar12;
  undefined8 *puVar13;
  long lVar14;
  undefined4 *puVar15;
  uint *puVar16;
  uint *puVar17;
  long lVar18;
  double dVar19;
  uint local_c38 [256];
  uint local_838 [256];
  uint local_438 [258];
  
  lVar3 = *(long *)(param_3 + 8);
  lVar4 = *(long *)(param_3 + 0x10);
  lVar5 = *(long *)(param_3 + 0x18);
  lVar6 = *(long *)(param_3 + 0x20);
  lVar18 = lVar3 + param_2;
  puVar13 = (undefined8 *)KBTtB_QLOXSPAIPP(0,lVar18,0);
  *puVar13 = 0;
  strcpy((char *)local_c38,param_1);
  puVar17 = local_c38;
  do {
    puVar16 = puVar17;
    uVar11 = *puVar16 + 0xfefefeff & ~*puVar16;
    uVar12 = uVar11 & 0x80808080;
    puVar17 = puVar16 + 1;
  } while (uVar12 == 0);
  if ((uVar11 & 0x8080) == 0) {
    puVar17 = (uint *)((long)puVar16 + 6);
    uVar12 = uVar12 >> 0x10;
  }
  puVar15 = (undefined4 *)((long)puVar17 + (-3 - (ulong)CARRY1((byte)uVar12,(byte)uVar12)));
  *puVar15 = 0x7368722e;
  *(undefined1 *)(puVar15 + 1) = 0;
  OByAtB_dOLJdFIBXSPAIPP(local_c38,lVar18,puVar13);
  FKSnBOJRQBbtB_QLOgKnIy_BXSPAIPP(param_4,lVar18,puVar13);
  uVar10 = DAT_00abd200;
  if (0 < lVar3) {
    lVar18 = 1;
    do {
      lVar7 = *(long *)(lVar6 + lVar18 * 8);
      lVar8 = *(long *)(lVar4 + lVar18 * 8);
      lVar9 = *(long *)(lVar5 + lVar18 * 8);
      lVar14 = 2;
      dVar19 = (double)puVar13[lVar18] / *(double *)(lVar7 + 8);
      puVar13[lVar18] = dVar19;
      if (1 < lVar8) {
        do {
          lVar1 = lVar14 * 8;
          lVar2 = lVar14 * 8;
          lVar14 = lVar14 + 1;
          puVar13[*(long *)(lVar9 + lVar2)] =
               *(double *)(lVar7 + lVar1) * (double)((ulong)dVar19 ^ uVar10) +
               (double)puVar13[*(long *)(lVar9 + lVar2)];
        } while (lVar14 != lVar8 + 1);
      }
      lVar18 = lVar18 + 1;
    } while (lVar18 != lVar3 + 1);
  }
  strcpy((char *)local_838,param_1);
  puVar17 = local_838;
  do {
    puVar16 = puVar17;
    uVar11 = *puVar16 + 0xfefefeff & ~*puVar16;
    uVar12 = uVar11 & 0x80808080;
    puVar17 = puVar16 + 1;
  } while (uVar12 == 0);
  if ((uVar11 & 0x8080) == 0) {
    puVar17 = (uint *)((long)puVar16 + 6);
    uVar12 = uVar12 >> 0x10;
  }
  puVar15 = (undefined4 *)((long)puVar17 + (-3 - (ulong)CARRY1((byte)uVar12,(byte)uVar12)));
  *(undefined2 *)(puVar15 + 1) = 0x6c;
  *puVar15 = 0x6f73692e;
  TOFQBtB_rLdFIBXSPAIPP(local_838,lVar3,puVar13);
  strcpy((char *)local_438,param_1);
  puVar17 = local_438;
  do {
    puVar16 = puVar17;
    uVar11 = *puVar16 + 0xfefefeff & ~*puVar16;
    uVar12 = uVar11 & 0x80808080;
    puVar17 = puVar16 + 1;
  } while (uVar12 == 0);
  if ((uVar11 & 0x8080) == 0) {
    puVar17 = (uint *)((long)puVar16 + 6);
    uVar12 = uVar12 >> 0x10;
  }
  puVar15 = (undefined4 *)((long)puVar17 + (-3 - (ulong)CARRY1((byte)uVar12,(byte)uVar12)));
  *(undefined2 *)(puVar15 + 1) = 0x72;
  *puVar15 = 0x6465722e;
  TOFQBtB_rLdFIBXSPAIPP(local_438,param_2,puVar13 + lVar3);
  _yPFdOBBXSPAIPP(puVar13);
  return;
}


