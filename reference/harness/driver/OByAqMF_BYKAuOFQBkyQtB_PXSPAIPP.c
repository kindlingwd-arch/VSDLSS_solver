/* driver: OByAqMF_BYKAuOFQBkyQtB_PXSPAIPP addr=0095cb60 size=883 */
#include "vsdlss_ref.h"


/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
OByAqMF_BYKAuOFQBkyQtB_PXSPAIPP
          (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
          undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,long param_9,
          long param_10,long param_11,long param_12,long param_13,undefined8 *param_14,
          undefined8 *param_15)

{
  size_t __size;
  size_t __size_00;
  size_t __size_01;
  double *pdVar1;
  double *pdVar2;
  double dVar3;
  FILE *pFVar4;
  long lVar5;
  void *pvVar6;
  void *pvVar7;
  void *pvVar8;
  void *pvVar9;
  void *pvVar10;
  void *pvVar11;
  void *pvVar12;
  void *pvVar13;
  void *pvVar14;
  void *pvVar15;
  void *pvVar16;
  void *pvVar17;
  size_t sVar18;
  long lVar19;
  undefined8 uVar20;
  long *plVar21;
  long lVar22;
  long lVar23;
  double dVar24;
  long local_1548;
  long local_1540;
  double local_14b8;
  double local_14b0;
  undefined8 local_14a8;
  undefined8 local_14a0;
  undefined8 local_1498;
  undefined8 local_1490;
  long local_1488;
  long local_1480;
  long local_1478;
  long local_1470;
  long local_1468;
  long local_1460;
  long local_1458 [4];
  long local_1438 [128];
  undefined1 local_1038 [1024];
  undefined1 local_c38 [1024];
  undefined1 local_838 [1024];
  undefined1 local_438 [1032];
  
  sVar18 = param_10 * 8 + 8;
  local_1498 = 0;
  local_1490 = 0;
  pvVar6 = tc_malloc(sVar18);
  __size = param_9 + 1;
  pvVar7 = tc_malloc(sVar18);
  __size_01 = __size * 8;
  __size_00 = param_13 * 8 + 8;
  pvVar8 = tc_malloc(__size_00);
  pvVar9 = tc_malloc(__size_00);
  pvVar10 = tc_malloc(__size_00);
  pvVar11 = tc_malloc(__size_00);
  pvVar12 = tc_malloc(__size_01);
  pvVar13 = tc_malloc(sVar18);
  pvVar14 = tc_malloc(__size_00);
  pvVar15 = tc_malloc(__size);
  pvVar16 = tc_malloc(__size_01);
  pvVar17 = tc_malloc(__size_01);
  LMBKlBQIFPQdFIBXSPAIPP();
  sVar18 = 1;
  if (0 < param_9) {
    do {
      *(undefined8 *)((long)pvVar12 + sVar18 * 8) = 0;
      *(undefined8 *)((long)pvVar17 + sVar18 * 8) = 0;
      *(undefined1 *)((long)pvVar15 + sVar18) = 0;
      sVar18 = sVar18 + 1;
    } while (sVar18 != __size);
  }
  pFVar4 = DAT_00e56348;
  *(undefined1 *)((long)pvVar15 + 1) = 1;
  lVar22 = 0;
  lVar23 = 0;
  *(undefined8 *)((long)pvVar16 + 8) = 0;
  lVar19 = DBQYjFKBuFQEdFOPQaEyOgKqBQXSPAIPP(pFVar4,"rivgRIVG",local_1438);
  local_1540 = 0;
  local_1548 = 1;
  if (lVar19 == 0) {
    do {
      switch((undefined1)local_1438[0]) {
      case 0x47:
      case 0x67:
        __isoc99_sscanf(local_1438,"%s %ld %ld %ld %ld %lg",local_438,local_1458,local_1458 + 1,
                        local_1458 + 2,local_1458 + 3,&local_14a0);
        plVar21 = local_1458;
        do {
          *plVar21 = *plVar21 + 1;
          plVar21 = plVar21 + 1;
        } while (plVar21 != local_1438);
        plVar21 = local_1458;
        do {
          if (param_9 < *plVar21) {
                    /* WARNING: Subroutine does not return */
            CyQyIcOOLOXSPAIPP(9999,"main.c",0x667,
                              "One of the end point nodes for vccs %s is: %ld\n%s\n",local_438,
                              *plVar21 + -1,"4:Node out of range for a vccs");
          }
          plVar21 = plVar21 + 1;
        } while (local_1438 != plVar21);
        lVar22 = lVar22 + 1;
        *(long *)((long)pvVar8 + lVar22 * 8) = local_1458[0];
        *(long *)((long)pvVar9 + lVar22 * 8) = local_1458[1];
        *(long *)((long)pvVar10 + lVar22 * 8) = local_1458[2];
        *(long *)((long)pvVar11 + lVar22 * 8) = local_1458[3];
        *(undefined8 *)((long)pvVar14 + lVar22 * 8) = local_14a0;
        break;
      default:
                    /* WARNING: Subroutine does not return */
        CyQyIcOOLOXSPAIPP(9999,"main.c",0x678,"5:This case is erroneous");
      case 0x49:
      case 0x69:
        __isoc99_sscanf(local_1438,"%s %ld %ld %lg",local_c38,&local_1478,&local_1470,&local_14b0);
        lVar5 = local_1470;
        lVar19 = local_1478;
        local_1478 = local_1478 + 1;
        local_1470 = local_1470 + 1;
        if ((param_9 < local_1478) || (param_9 < local_1470)) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x635,
                            "The end point nodes for current source %s are: %ld %ld\n%s\n",local_c38
                            ,lVar19,lVar5,"2:Node out of range for a current source.");
        }
        pdVar1 = (double *)((long)pvVar17 + local_1478 * 8);
        pdVar2 = (double *)((long)pvVar17 + local_1470 * 8);
        local_1540 = local_1540 + 1;
        *pdVar1 = *pdVar1 - local_14b0;
        *pdVar2 = local_14b0 + *pdVar2;
        break;
      case 0x52:
      case 0x72:
        __isoc99_sscanf(local_1438,"%s %ld %ld %lg",local_1038,&local_1488,&local_1480,&local_14b8);
        lVar5 = local_1480;
        lVar19 = local_1488;
        local_1488 = local_1488 + 1;
        local_1480 = local_1480 + 1;
        if ((param_9 < local_1488) || (param_9 < local_1480)) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x620,
                            "The end point nodes for resistor %s are: %ld %ld\n%s\n",local_1038,
                            lVar19,lVar5,"1:Node out of range for a resistor");
        }
        lVar23 = lVar23 + 1;
        dVar24 = _DAT_00ab67f8 / local_14b8;
        *(long *)((long)pvVar6 + lVar23 * 8) = local_1488;
        pdVar1 = (double *)((long)pvVar12 + local_1488 * 8);
        *(long *)((long)pvVar7 + lVar23 * 8) = local_1480;
        pdVar2 = (double *)((long)pvVar12 + local_1480 * 8);
        dVar3 = *pdVar1;
        *(double *)((long)pvVar13 + lVar23 * 8) = dVar24;
        *pdVar1 = dVar3 + dVar24;
        *pdVar2 = dVar24 + *pdVar2;
        break;
      case 0x56:
      case 0x76:
        __isoc99_sscanf(local_1438,"%s %ld %ld %lg",local_838,&local_1468,&local_1460,&local_14a8);
        lVar5 = local_1460;
        lVar19 = local_1468;
        local_1468 = local_1468 + 1;
        local_1460 = local_1460 + 1;
        if ((param_9 < local_1468) || (param_9 < local_1460)) {
LAB_0095d28a:
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x64a,
                            "The end point nodes for voltage source %s are: %ld %ld\n%s\n",local_838
                            ,lVar19,lVar5,
                            "3:Node out of range for a voltage source or a floating voltage.");
        }
        if (local_1468 == 1) {
          if (local_1460 == 1) goto LAB_0095d28a;
        }
        else if (local_1460 != 1) goto LAB_0095d28a;
        local_1548 = local_1548 + 1;
        if (local_1468 <= local_1460) {
          local_1468 = local_1460;
        }
        *(undefined1 *)((long)pvVar15 + local_1468) = 1;
        *(undefined8 *)((long)pvVar16 + local_1468 * 8) = local_14a8;
      }
      lVar19 = DBQYjFKBuFQEdFOPQaEyOgKqBQXSPAIPP(DAT_00e56348,"rivgRIVG",local_1438);
    } while (lVar19 == 0);
  }
  if (param_10 != lVar23) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x682,"Number of resistors: header %ld : netlist %ld \n%s\n",
                      param_10,lVar23,"5:Inconsistent input.");
  }
  if (param_12 != local_1540) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x68a,
                      "Number of current sources: header %ld : netlist %ld \n%s\n",param_12,
                      local_1540,"6:Inconsistent input.");
  }
  if (param_11 != local_1548) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x693,
                      "Number of voltage sources: header %ld : netlist %ld \n%s\n",param_11,
                      local_1548,"7:Inconsistent input.");
  }
  if (param_13 == lVar22) {
    uVar20 = TOFQBkyQtB_PdFIBXSPAIPP
                       (param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9,
                        param_10,pvVar6,pvVar7,pvVar13,pvVar12,param_13,pvVar8,pvVar9,pvVar10,
                        pvVar11,pvVar14,&local_1498,&local_1490,pvVar15,pvVar16,pvVar17);
    fclose(DAT_00e56348);
    *param_14 = local_1498;
    *param_15 = local_1490;
    return uVar20;
  }
                    /* WARNING: Subroutine does not return */
  CyQyIcOOLOXSPAIPP(9999,"main.c",0x69b,"Number of VCC sources: header %ld : netlist %ld \n%s\n",
                    param_13,lVar22,"8:Inconsistent input.");
}


