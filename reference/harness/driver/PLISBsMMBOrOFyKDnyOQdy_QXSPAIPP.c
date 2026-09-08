/* driver: PLISBsMMBOrOFyKDnyOQdy_QXSPAIPP addr=009a2920 size=23c */
#include "vsdlss_ref.h"


void PLISBsMMBOrOFyKDnyOQdy_QXSPAIPP(char *param_1,long param_2,long param_3,undefined8 param_4)

{
  long lVar1;
  long lVar2;
  double dVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  uint uVar9;
  uint uVar10;
  long lVar11;
  undefined8 *puVar12;
  undefined4 *puVar13;
  uint *puVar14;
  uint *puVar15;
  long lVar16;
  long lVar17;
  double dVar18;
  uint local_c38 [256];
  uint local_838 [256];
  uint local_438 [258];
  
  lVar17 = *(long *)(param_3 + 8);
  lVar4 = *(long *)(param_3 + 0x18);
  lVar5 = *(long *)(param_3 + 0x20);
  lVar6 = *(long *)(param_3 + 0x10);
  lVar11 = param_2 + lVar17;
  puVar12 = (undefined8 *)KBTtB_QLOXSPAIPP(0,lVar11,0);
  *puVar12 = 0;
  strcpy((char *)local_c38,param_1);
  puVar15 = local_c38;
  do {
    puVar14 = puVar15;
    uVar9 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar10 = uVar9 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar10 == 0);
  if ((uVar9 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar10 = uVar10 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar10,(byte)uVar10)));
  *(undefined2 *)(puVar13 + 1) = 0x6c;
  *puVar13 = 0x6f73692e;
  OByAtB_dOLJdFIBXSPAIPP(local_c38,lVar17,puVar12);
  strcpy((char *)local_838,param_1);
  puVar15 = local_838;
  do {
    puVar14 = puVar15;
    uVar9 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar10 = uVar9 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar10 == 0);
  if ((uVar9 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar10 = uVar10 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar10,(byte)uVar10)));
  *puVar13 = 0x726f702e;
  *(undefined2 *)(puVar13 + 1) = 0x7674;
  *(undefined1 *)((long)puVar13 + 6) = 0;
  OByAtB_dOLJdFIBXSPAIPP(local_838,param_2,puVar12 + lVar17);
  if (0 < lVar17) {
    do {
      lVar7 = *(long *)(lVar6 + lVar17 * 8);
      lVar8 = *(long *)(lVar5 + lVar17 * 8);
      dVar18 = 0.0;
      if (1 < lVar7) {
        dVar18 = 0.0;
        lVar16 = 2;
        do {
          lVar1 = lVar16 * 8;
          lVar2 = lVar16 * 8;
          lVar16 = lVar16 + 1;
          dVar18 = dVar18 + (double)puVar12[*(long *)(*(long *)(lVar4 + lVar17 * 8) + lVar1)] *
                            *(double *)(lVar8 + lVar2);
        } while (lVar16 != lVar7 + 1);
      }
      dVar3 = (double)puVar12[lVar17];
      puVar12[lVar17] = dVar3 - dVar18;
      puVar12[lVar17] = (dVar3 - dVar18) / *(double *)(lVar8 + 8);
      lVar17 = lVar17 + -1;
    } while (lVar17 != 0);
  }
  MBOJRQBbtB_QLOgKnIy_BXSPAIPP(param_4,lVar11,puVar12);
  strcpy((char *)local_438,param_1);
  puVar15 = local_438;
  do {
    puVar14 = puVar15;
    uVar9 = *puVar14 + 0xfefefeff & ~*puVar14;
    uVar10 = uVar9 & 0x80808080;
    puVar15 = puVar14 + 1;
  } while (uVar10 == 0);
  if ((uVar9 & 0x8080) == 0) {
    puVar15 = (uint *)((long)puVar14 + 6);
    uVar10 = uVar10 >> 0x10;
  }
  puVar13 = (undefined4 *)((long)puVar15 + (-3 - (ulong)CARRY1((byte)uVar10,(byte)uVar10)));
  *puVar13 = 0x6c73722e;
  *(undefined1 *)(puVar13 + 1) = 0;
  TOFQBtB_rLdFIBXSPAIPP(local_438,lVar11,puVar12);
  _yPFdOBBXSPAIPP(puVar12);
  return;
}


