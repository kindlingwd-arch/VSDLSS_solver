/* driver: kjbeOLTpBDFLKZyIyK_BpBCFKBXSPAIPP addr=0099a940 size=2ce */
#include "vsdlss_ref.h"


void kjbeOLTpBDFLKZyIyK_BpBCFKBXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,long param_6)

{
  char *pcVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  undefined8 uVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  int iVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  
  lVar13 = *(long *)(param_6 + 0xa0);
  lVar2 = *(long *)(param_2 + 0x10);
  lVar3 = *(long *)(param_2 + 0x50);
  lVar4 = *(long *)(param_2 + 0x80);
  lVar5 = *(long *)(param_6 + 0x48);
  *(undefined8 *)(param_6 + 0x48) = 0;
  FKFQatB_QLOXSPAIPP(lVar5,lVar2,0);
  lVar17 = 2;
  *(undefined8 *)(param_2 + 0x38) = 0;
  *(long *)(param_2 + 0x40) = *(long *)(param_3 + 8) + *(long *)(param_3 + 0x10);
  FKFQgtB_QLOXSPAIPP(lVar13,lVar2,2);
  lVar6 = *(long *)(param_6 + 0x30);
  uVar7 = *(undefined8 *)(param_6 + 0x60);
  *(long *)(lVar6 + 0x10) = lVar2;
  *(undefined8 *)(lVar6 + 0x18) = uVar7;
  FKFQoXSPAIPP(lVar6);
  lVar8 = *(long *)(lVar6 + 0x18);
  lVar16 = 1;
  lVar9 = *(long *)(lVar6 + 0x10);
  *(long *)(lVar8 + 8) = param_1;
  *(undefined1 *)(lVar5 + param_1) = 1;
  do {
    lVar10 = *(long *)(lVar8 + lVar16 * 8);
    lVar16 = lVar16 + 1;
    if (lVar4 == 0) {
      lVar14 = 1;
    }
    else {
      lVar14 = *(long *)(lVar4 + lVar10 * 8);
    }
    if ((*(long *)(param_2 + 0x38) < 1) ||
       (*(long *)(param_5 + 0x10) <= *(long *)(param_2 + 0x40) - lVar14)) {
      *(undefined8 *)(lVar13 + lVar10 * 8) = 1;
      *(long *)(param_2 + 0x38) = *(long *)(param_2 + 0x38) + lVar14;
      lVar14 = *(long *)(param_2 + 0x40) - lVar14;
      *(long *)(param_2 + 0x40) = lVar14;
      if (lVar14 <= *(long *)(param_4 + 0x10)) break;
      lVar14 = *(long *)(lVar3 + lVar10 * 8);
      lVar10 = *(long *)(*(long *)(param_2 + 0x60) + lVar10 * 8);
      if (0 < lVar14) {
        lVar15 = 1;
        do {
          lVar11 = *(long *)(lVar10 + lVar15 * 8);
          pcVar1 = (char *)(lVar5 + lVar11);
          if (*pcVar1 == '\0') {
            if (lVar9 < lVar17) {
              KOBOOLOXSPAIPP(1,"mldorder.c",0xaea);
            }
            *(long *)(lVar8 + lVar17 * 8) = lVar11;
            lVar17 = lVar17 + 1;
            *pcVar1 = '\x01';
          }
          lVar15 = lVar15 + 1;
        } while (lVar15 <= lVar14);
      }
    }
  } while (lVar16 < lVar17);
  PBQorLlRIIXSPAIPP(lVar6);
  lVar3 = *(long *)(param_2 + 0x40);
  *(long *)(param_6 + 0x48) = lVar5;
  if (lVar3 == 0) {
    iVar12 = rand();
    lVar2 = (long)(iVar12 >> 3) % lVar2 + 1;
    *(undefined8 *)(lVar13 + lVar2 * 8) = 2;
    if (lVar4 == 0) {
      lVar13 = 1;
    }
    else {
      lVar13 = *(long *)(lVar4 + lVar2 * 8);
    }
    *(long *)(param_2 + 0x40) = *(long *)(param_2 + 0x40) + lVar13;
    *(long *)(param_2 + 0x38) = *(long *)(param_2 + 0x38) - lVar13;
  }
  _LJMRQBgKQBOtB_PdLOZyIyK_BpBCFKBXSPAIPP(param_3,param_2,param_6);
  kjbZyIyK_BXSPAIPP(param_3,param_2,param_6);
  kjbdkpBCFKBXE(param_3,param_2,param_6);
  return;
}


