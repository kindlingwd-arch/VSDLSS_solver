/* VSDLSS function (decompiled by Ghidra) */
/* name: readSpiceAndWriteMatVecs_vsdlss  addr: 009d43e0  size: 877 */
#include "vsdlss_ref.h"

/* signature: undefined4 readSpiceAndWriteMatVecs_vsdlss(undefined4 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,int param_9,int param_10,int param_11,int param_12,int param_13,undefined4 *param_14,undefined4 *param_15); */

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
readSpiceAndWriteMatVecs_vsdlss
          (undefined4 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
          undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,int param_9,
          int param_10,int param_11,int param_12,int param_13,undefined4 *param_14,
          undefined4 *param_15)

{
  size_t sVar1;
  double *pdVar2;
  double *pdVar3;
  double dVar4;
  FILE *pFVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  void *pvVar9;
  void *pvVar10;
  void *pvVar11;
  void *pvVar12;
  void *pvVar13;
  void *pvVar14;
  void *pvVar15;
  void *pvVar16;
  void *pvVar17;
  void *pvVar18;
  void *pvVar19;
  void *pvVar20;
  long lVar21;
  int *piVar22;
  int iVar23;
  int iVar24;
  double dVar25;
  int local_152c;
  int local_14d8;
  undefined4 local_14a0;
  undefined4 local_149c;
  int local_1498;
  int local_1494 [3];
  int local_1488;
  int local_1484 [3];
  int local_1478;
  int local_1474 [3];
  double local_1468;
  double local_1460;
  undefined8 local_1458;
  undefined8 local_1450;
  int local_1448 [4];
  int local_1438 [256];
  undefined1 local_1038 [1024];
  undefined1 local_c38 [1024];
  undefined1 local_838 [1024];
  undefined1 local_438 [1032];
  
  local_14a0 = 0;
  local_149c = 0;
  sVar1 = (long)(param_10 + 1) * 4;
  pvVar9 = tc_malloc(sVar1);
  pvVar10 = tc_malloc(sVar1);
  sVar1 = (long)(param_13 + 1) * 4;
  pvVar11 = tc_malloc(sVar1);
  pvVar12 = tc_malloc(sVar1);
  pvVar13 = tc_malloc(sVar1);
  pvVar14 = tc_malloc(sVar1);
  sVar1 = (long)(param_9 + 1) * 8;
  pvVar15 = tc_malloc(sVar1);
  pvVar16 = tc_malloc((long)(param_10 + 1) * 8);
  pvVar17 = tc_malloc((long)(param_13 + 1) * 8);
  pvVar18 = tc_malloc((long)(param_9 + 1));
  pvVar19 = tc_malloc(sVar1);
  pvVar20 = tc_malloc(sVar1);
  openNetlistFile_vsdlss();
  if (0 < param_9) {
    lVar21 = 0;
    do {
      *(undefined8 *)((long)pvVar15 + lVar21 * 8 + 8) = 0;
      *(undefined8 *)((long)pvVar20 + lVar21 * 8 + 8) = 0;
      *(undefined1 *)((long)pvVar18 + lVar21 + 1) = 0;
      lVar21 = lVar21 + 1;
    } while (lVar21 != (ulong)(param_9 - 1) + 1);
  }
  pFVar5 = DAT_00e96448;
  *(undefined1 *)((long)pvVar18 + 1) = 1;
  iVar23 = 0;
  iVar24 = 0;
  *(undefined8 *)((long)pvVar19 + 8) = 0;
  iVar7 = getALineWithFirstCharInSet_vsdlss(pFVar5,"rivgRIVG",local_1438);
  local_14d8 = 0;
  local_152c = 1;
  if (iVar7 == 0) {
    iVar23 = 0;
    do {
      switch((undefined1)local_1438[0]) {
      case 0x47:
      case 0x67:
        __isoc99_sscanf(local_1438,"%s %d %d %d %d %lg",local_438,local_1448,local_1448 + 1,
                        local_1448 + 2,local_1448 + 3,&local_1450);
        piVar22 = local_1448;
        do {
          *piVar22 = *piVar22 + 1;
          piVar22 = piVar22 + 1;
        } while (piVar22 != local_1438);
        piVar22 = local_1448;
        do {
          if (param_9 < *piVar22) {
                    /* WARNING: Subroutine does not return */
            fatalError_vsdlss(9999,"main.c",0x667,
                              "One of the end point nodes for vccs %s is: %d\n%s\n",local_438,
                              *piVar22 + -1,"4:Node out of range for a vccs");
          }
          piVar22 = piVar22 + 1;
        } while (local_1438 != piVar22);
        iVar23 = iVar23 + 1;
        lVar21 = (long)iVar23;
        *(int *)((long)pvVar11 + lVar21 * 4) = local_1448[0];
        *(int *)((long)pvVar12 + lVar21 * 4) = local_1448[1];
        *(int *)((long)pvVar13 + lVar21 * 4) = local_1448[2];
        *(int *)((long)pvVar14 + lVar21 * 4) = local_1448[3];
        *(undefined8 *)((long)pvVar17 + lVar21 * 8) = local_1450;
        break;
      default:
                    /* WARNING: Subroutine does not return */
        fatalError_vsdlss(9999,"main.c",0x678,"5:This case is erroneous");
      case 0x49:
      case 0x69:
        __isoc99_sscanf(local_1438,"%s %d %d %lg",local_c38,&local_1488,local_1484,&local_1460);
        iVar6 = local_1484[0];
        iVar7 = local_1488;
        local_1488 = local_1488 + 1;
        local_1484[0] = local_1484[0] + 1;
        if ((param_9 < local_1488) || (param_9 < local_1484[0])) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x635,
                            "The end point nodes for current source %s are: %d %d\n%s\n",local_c38,
                            iVar7,iVar6,"2:Node out of range for a current source.");
        }
        pdVar2 = (double *)((long)pvVar20 + (long)local_1488 * 8);
        local_14d8 = local_14d8 + 1;
        pdVar3 = (double *)((long)pvVar20 + (long)local_1484[0] * 8);
        *pdVar2 = *pdVar2 - local_1460;
        *pdVar3 = local_1460 + *pdVar3;
        break;
      case 0x52:
      case 0x72:
        __isoc99_sscanf(local_1438,"%s %d %d %lg",local_1038,&local_1498,local_1494,&local_1468);
        iVar6 = local_1494[0];
        iVar7 = local_1498;
        local_1498 = local_1498 + 1;
        local_1494[0] = local_1494[0] + 1;
        if ((param_9 < local_1498) || (param_9 < local_1494[0])) {
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x620,
                            "The end point nodes for resistor %s are: %d %d\n%s\n",local_1038,iVar7,
                            iVar6,"1:Node out of range for a resistor");
        }
        iVar24 = iVar24 + 1;
        lVar21 = (long)iVar24;
        dVar25 = _DAT_00ab67f8 / local_1468;
        *(int *)((long)pvVar9 + lVar21 * 4) = local_1498;
        pdVar2 = (double *)((long)pvVar15 + (long)local_1498 * 8);
        *(int *)((long)pvVar10 + lVar21 * 4) = local_1494[0];
        pdVar3 = (double *)((long)pvVar15 + (long)local_1494[0] * 8);
        dVar4 = *pdVar2;
        *(double *)((long)pvVar16 + lVar21 * 8) = dVar25;
        *pdVar2 = dVar4 + dVar25;
        *pdVar3 = dVar25 + *pdVar3;
        break;
      case 0x56:
      case 0x76:
        __isoc99_sscanf(local_1438,"%s %d %d %lg",local_838,&local_1478,local_1474,&local_1458);
        iVar6 = local_1474[0];
        iVar7 = local_1478;
        local_1478 = local_1478 + 1;
        local_1474[0] = local_1474[0] + 1;
        if ((param_9 < local_1478) || (param_9 < local_1474[0])) {
LAB_009d4af9:
                    /* WARNING: Subroutine does not return */
          fatalError_vsdlss(9999,"main.c",0x64a,
                            "The end point nodes for voltage source %s are: %d %d\n%s\n",local_838,
                            iVar7,iVar6,
                            "3:Node out of range for a voltage source or a floating voltage.");
        }
        if (local_1478 == 1) {
          if (local_1474[0] == 1) goto LAB_009d4af9;
        }
        else if (local_1474[0] != 1) goto LAB_009d4af9;
        local_152c = local_152c + 1;
        if (local_1478 <= local_1474[0]) {
          local_1478 = local_1474[0];
        }
        *(undefined1 *)((long)pvVar18 + (long)local_1478) = 1;
        *(undefined8 *)((long)pvVar19 + (long)local_1478 * 8) = local_1458;
      }
      iVar7 = getALineWithFirstCharInSet_vsdlss(DAT_00e96448,"rivgRIVG",local_1438);
    } while (iVar7 == 0);
  }
  if (param_10 != iVar24) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x682,"Number of resistors: header %d : netlist %d \n%s\n",
                      param_10,iVar24,"5:Inconsistent input.");
  }
  if (param_12 != local_14d8) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x68a,"Number of current sources: header %d : netlist %d \n%s\n"
                      ,param_12,local_14d8,"6:Inconsistent input.");
  }
  if (param_11 != local_152c) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x693,"Number of voltage sources: header %d : netlist %d \n%s\n"
                      ,param_11,local_152c,"7:Inconsistent input.");
  }
  if (param_13 == iVar23) {
    uVar8 = writeMatVecsFile_vsdlss
                      (param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9,
                       param_10,pvVar9,pvVar10,pvVar16,pvVar15,param_13,pvVar11,pvVar12,pvVar13,
                       pvVar14,pvVar17,&local_14a0,&local_149c,pvVar18,pvVar19,pvVar20);
    fclose(DAT_00e96448);
    *param_14 = local_14a0;
    *param_15 = local_149c;
    return uVar8;
  }
                    /* WARNING: Subroutine does not return */
  fatalError_vsdlss(9999,"main.c",0x69b,"Number of VCC sources: header %d : netlist %d \n%s\n",
                    param_13,iVar23,"8:Inconsistent input.");
}


