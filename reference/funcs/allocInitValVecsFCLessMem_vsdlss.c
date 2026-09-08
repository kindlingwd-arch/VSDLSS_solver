/* VSDLSS function (decompiled by Ghidra) */
/* name: allocInitValVecsFCLessMem_vsdlss  addr: 00a16d60  size: 4c4 */
#include "vsdlss_ref.h"

/* signature: void allocInitValVecsFCLessMem_vsdlss(int *param_1,undefined4 *param_2); */

void allocInitValVecsFCLessMem_vsdlss(int *param_1,undefined4 *param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  undefined8 *puVar8;
  long lVar9;
  long lVar10;
  int iVar11;
  long lVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  int iVar19;
  int iVar20;
  long local_a0;
  long local_98;
  int local_7c;
  long local_68;
  int local_54;
  int local_44;
  
  uVar1 = *param_2;
  lVar4 = *(long *)(param_2 + 0xc);
  iVar2 = param_2[2];
  lVar5 = *(long *)(param_2 + 10);
  if (iVar2 < 0x400) {
    local_a0 = *(long *)(param_1 + 0xe);
    lVar17 = *(long *)(param_1 + 0xc);
    lVar9 = *(long *)(param_1 + 10);
    local_98 = *(long *)(param_1 + 8);
    local_7c = *param_1;
    local_44 = getBreakPointInd(param_1);
    local_68 = tagNewFPVector_vsdlss(uVar1,(long)iVar2,1,0);
    *(long *)(param_2 + 0xe) = local_68;
    if (iVar2 < 1) {
LAB_00a17108:
      if (local_7c != 0) {
        nrerror_vsdlss(1,"vsdlss1memutils.c",0x454);
      }
      if (*(long *)(param_1 + 10) != 0) {
        casiFree_vsdlss();
      }
      param_1[10] = 0;
      param_1[0xb] = 0;
      if (*(long *)(param_1 + 0xc) != 0) {
        casiFree_vsdlss();
      }
      param_1[0xc] = 0;
      param_1[0xd] = 0;
      if (*(long *)(param_1 + 8) != 0) {
        casiFree_vsdlss();
      }
      param_1[8] = 0;
      param_1[9] = 0;
      if (*(long *)(param_1 + 0xe) != 0) {
        casiFree_vsdlss();
      }
      param_1[0xe] = 0;
      param_1[0xf] = 0;
      return;
    }
    local_54 = 1;
  }
  else {
    local_54 = iVar2 >> 10;
    local_a0 = *(long *)(param_1 + 0xe);
    lVar17 = *(long *)(param_1 + 0xc);
    lVar9 = *(long *)(param_1 + 10);
    local_98 = *(long *)(param_1 + 8);
    local_7c = *param_1;
    local_44 = getBreakPointInd(param_1);
    local_68 = tagNewFPVector_vsdlss(uVar1,(long)iVar2,1,0);
    *(long *)(param_2 + 0xe) = local_68;
  }
  lVar12 = 0;
  iVar20 = 0;
  lVar18 = 0;
  iVar13 = 1;
  do {
    iVar3 = *(int *)(lVar5 + 4 + lVar12 * 4);
    lVar6 = lVar18;
    if (iVar20 < iVar3) {
      iVar19 = iVar3 + 1;
      if (iVar3 < local_54) {
        iVar19 = local_54;
      }
      iVar20 = iVar19 + -1;
      lVar6 = newBlock_vsdlss(uVar1,(long)iVar19 << 3);
    }
    iVar20 = iVar20 - iVar3;
    lVar18 = lVar6 + (long)iVar3 * 8;
    *(long *)(local_68 + 8 + lVar12 * 8) = lVar6;
    lVar16 = *(long *)(lVar4 + 8 + lVar12 * 8);
    iVar19 = *(int *)(local_98 + 4);
    *(undefined8 *)(lVar6 + 8) = *(undefined8 *)(local_a0 + 8);
    if ((iVar19 < 1) || (iVar3 < 2)) {
      iVar14 = 2;
      iVar11 = 1;
    }
    else {
      lVar7 = 0;
      iVar11 = 1;
      iVar15 = 2;
      do {
        lVar10 = (long)iVar11;
        if (*(int *)(lVar9 + lVar10 * 4) == *(int *)(lVar16 + 8 + lVar7)) {
          iVar11 = iVar11 + 1;
          *(undefined8 *)(lVar6 + 0x10 + lVar7 * 2) = *(undefined8 *)(lVar17 + lVar10 * 8);
        }
        else {
          *(undefined8 *)(lVar6 + 0x10 + lVar7 * 2) = 0;
        }
        iVar14 = iVar15 + 1;
        if (iVar19 < iVar11) break;
        iVar14 = iVar15 + 1;
        lVar7 = lVar7 + 4;
        iVar15 = iVar14;
      } while (iVar14 <= iVar3);
    }
    if (iVar11 <= iVar19) {
      nrerror_vsdlss(1,"vsdlss1memutils.c",0x42d);
    }
    if (iVar14 <= iVar3) {
      puVar8 = (undefined8 *)(lVar6 + (long)iVar14 * 8);
      do {
        *puVar8 = 0;
        puVar8 = puVar8 + 1;
      } while (puVar8 != (undefined8 *)
                         (lVar6 + 8 + ((long)iVar14 + (ulong)(uint)(iVar3 - iVar14)) * 8));
    }
    local_7c = local_7c - iVar19;
    local_98 = local_98 + 4;
    lVar9 = lVar9 + (long)iVar19 * 4;
    lVar17 = lVar17 + (long)iVar19 * 8;
    local_a0 = local_a0 + 8;
    if ((iVar13 < iVar2) && (iVar13 == local_44)) {
      lVar16 = (long)local_7c;
      lVar6 = newIVector_vsdlss(lVar16,0,0);
      copyIVector_vsdlss(lVar9,lVar16,lVar6);
      if (*(long *)(param_1 + 10) != 0) {
        casiFree_vsdlss();
      }
      *(long *)(param_1 + 10) = lVar6;
      lVar7 = newFVector_vsdlss(0,lVar16,0);
      copyFVector_vsdlss(lVar17,lVar16,lVar7);
      if (*(long *)(param_1 + 0xc) != 0) {
        casiFree_vsdlss();
      }
      lVar17 = ((long)iVar2 + -1) - lVar12;
      *(long *)(param_1 + 0xc) = lVar7;
      lVar9 = newIVector_vsdlss(lVar17,0,0);
      copyIVector_vsdlss(local_98,lVar17,lVar9);
      if (*(long *)(param_1 + 8) != 0) {
        casiFree_vsdlss();
      }
      *(long *)(param_1 + 8) = lVar9;
      lVar16 = newFVector_vsdlss(0,lVar17,0);
      copyFVector_vsdlss(local_a0,lVar17,lVar16);
      if (*(long *)(param_1 + 0xe) != 0) {
        casiFree_vsdlss();
      }
      *(long *)(param_1 + 0xe) = lVar16;
      local_98 = lVar9;
      lVar9 = lVar6;
      lVar17 = lVar7;
      local_a0 = lVar16;
    }
    if (iVar2 < (int)lVar12 + 2) goto LAB_00a17108;
    iVar13 = (int)lVar12 + 2;
    lVar12 = lVar12 + 1;
  } while( true );
}


