/* driver: kjbkFKaLSBOXSPAIPP addr=0099b9b0 size=4ef */
#include "vsdlss_ref.h"


undefined8 kjbkFKaLSBOXSPAIPP(long param_1,long param_2,long param_3,undefined8 param_4)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  undefined8 uVar19;
  long lVar20;
  long lVar21;
  long lVar22;
  long local_e8;
  
  uVar19 = *(undefined8 *)(param_3 + 0x60);
  lVar2 = *(long *)(param_2 + 0x50);
  lVar3 = *(long *)(param_1 + 8);
  lVar4 = *(long *)(param_1 + 0x10);
  lVar5 = *(long *)(param_3 + 0x30);
  *(long *)(lVar5 + 0x10) = lVar4;
  *(undefined8 *)(lVar5 + 0x18) = uVar19;
  FKFQoXSPAIPP(lVar5);
  lVar6 = *(long *)(param_3 + 0x70);
  lVar7 = *(long *)(lVar5 + 0x18);
  lVar8 = *(long *)(param_3 + 0x68);
  lVar9 = *(long *)(param_3 + 0x48);
  lVar10 = *(long *)(param_3 + 0x78);
  lVar11 = *(long *)(lVar5 + 0x10);
  *(undefined8 *)(param_3 + 0x48) = 0;
  *(undefined8 *)(param_3 + 0x68) = 0;
  lVar12 = *(long *)(param_3 + 0x58);
  *(undefined8 *)(param_3 + 0x70) = 0;
  *(undefined8 *)(param_3 + 0x78) = 0;
  *(undefined8 *)(param_3 + 0x58) = 0;
  FKFQgtB_QLOXSPAIPP(lVar8,lVar4,0);
  if (0 < lVar3) {
    lVar21 = 1;
    lVar20 = *(long *)(param_2 + 0x60);
    do {
      while( true ) {
        lVar22 = *(long *)(lVar2 + lVar21 * 8);
        lVar13 = *(long *)(lVar20 + lVar21 * 8);
        if (lVar22 < 1) break;
        lVar14 = *(long *)(lVar13 + 8);
        lVar18 = 1;
        plVar1 = (long *)(lVar8 + lVar14 * 8);
        lVar15 = *plVar1;
        while (lVar15 != 0) {
          lVar18 = lVar18 + 1;
          if (lVar22 < lVar18) goto LAB_0099bafb;
          lVar14 = *(long *)(lVar13 + lVar18 * 8);
          plVar1 = (long *)(lVar8 + lVar14 * 8);
          lVar15 = *plVar1;
        }
        *(long *)(lVar8 + lVar21 * 8) = lVar14;
        *plVar1 = lVar21;
        lVar21 = lVar21 + 1;
        if (lVar21 == lVar3 + 1) goto LAB_0099bb04;
      }
LAB_0099bafb:
      lVar21 = lVar21 + 1;
    } while (lVar21 != lVar3 + 1);
  }
LAB_0099bb04:
  FKFQgtB_QLOXSPAIPP(lVar6,lVar4,0);
  FKFQgtB_QLOXSPAIPP(lVar12,lVar4,0);
  FKFQatB_QLOXSPAIPP(lVar9,lVar4,0);
  if (0 < lVar3) {
    while( true ) {
      lVar20 = 1;
      local_e8 = 1;
      do {
        if (*(long *)(lVar8 + lVar20 * 8) == 0) {
          if (lVar11 < local_e8) {
            KOBOOLOXSPAIPP(1,"mldorder.c",0xd6f);
          }
          *(long *)(lVar7 + local_e8 * 8) = lVar20;
          local_e8 = local_e8 + 1;
          *(undefined8 *)(lVar6 + lVar20 * 8) = 1;
        }
        lVar20 = lVar20 + 1;
      } while (lVar20 <= lVar3);
      if (local_e8 == 1) break;
      lVar22 = 1;
      lVar21 = 0;
      lVar20 = lVar4 + 1;
      do {
        lVar13 = *(long *)(lVar7 + lVar22 * 8);
        lVar22 = lVar22 + 1;
        plVar1 = (long *)(lVar6 + lVar13 * 8);
        if (*plVar1 <= lVar20) {
          *(undefined1 *)(lVar9 + lVar13) = 1;
          lVar14 = *(long *)(lVar2 + lVar13 * 8);
          lVar15 = *(long *)(*(long *)(param_2 + 0x60) + lVar13 * 8);
          if (0 < lVar14) {
            lVar18 = 1;
            do {
              while( true ) {
                lVar16 = *(long *)(lVar15 + lVar18 * 8);
                if (*(char *)(lVar9 + lVar16) == '\0') break;
LAB_0099bd00:
                lVar18 = lVar18 + 1;
                if (lVar14 < lVar18) goto LAB_0099bd88;
              }
              *(char *)(lVar9 + lVar16) = '\x01';
              *(long *)(lVar12 + lVar16 * 8) = lVar13;
              lVar17 = *(long *)(lVar8 + lVar16 * 8);
              if (lVar17 == 0) {
                lVar21 = lVar21 + 1;
                lVar20 = *plVar1;
                *(long *)(lVar10 + lVar21 * 8) = lVar16;
                goto LAB_0099bd00;
              }
              if (lVar20 <= *plVar1) goto LAB_0099bd00;
              if (*(char *)(lVar9 + lVar17) != '\0') {
                KOBOOLOXSPAIPP(1,"mldorder.c",0xd8c);
              }
              *(long *)(lVar12 + lVar17 * 8) = lVar16;
              if (lVar11 < local_e8) {
                KOBOOLOXSPAIPP(1,"mldorder.c",0xd93);
              }
              lVar18 = lVar18 + 1;
              lVar16 = local_e8 + 1;
              *(long *)(lVar7 + local_e8 * 8) = lVar17;
              *(long *)(lVar6 + lVar17 * 8) = *plVar1 + 1;
              local_e8 = lVar16;
            } while (lVar18 <= lVar14);
          }
        }
LAB_0099bd88:
      } while (lVar22 < local_e8);
      if (lVar21 == 0) break;
      lVar20 = 1;
      do {
        lVar22 = lVar20 * 8;
        lVar20 = lVar20 + 1;
        kjbkFKaLSBOYRDJBKQXSPAIPP(*(undefined8 *)(lVar10 + lVar22),lVar12,lVar9,lVar8);
      } while (lVar20 <= lVar21);
      FKFQgtB_QLOXSPAIPP(lVar6,lVar4,0);
      FKFQgtB_QLOXSPAIPP(lVar12,lVar4,0);
      FKFQatB_QLOXSPAIPP(lVar9,lVar4,0);
    }
  }
  uVar19 = kjbkFKaLSBObB_LJMLPBXSPAIPP(param_1,param_2,lVar8,param_4,lVar6,lVar5);
  *(long *)(param_3 + 0x78) = lVar10;
  *(long *)(param_3 + 0x48) = lVar9;
  *(long *)(param_3 + 0x68) = lVar8;
  *(long *)(param_3 + 0x70) = lVar6;
  *(long *)(param_3 + 0x58) = lVar12;
  PBQorLlRIIXSPAIPP(lVar5);
  return uVar19;
}


