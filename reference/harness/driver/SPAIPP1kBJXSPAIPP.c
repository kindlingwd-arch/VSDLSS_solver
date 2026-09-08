/* driver: SPAIPP1kBJXSPAIPP addr=00966a80 size=1944 */
#include "vsdlss_ref.h"


undefined8
SPAIPP1kBJXSPAIPP(double param_1,long param_2,long param_3,long param_4,long param_5,
                 undefined8 param_6,undefined8 param_7,long param_8,long param_9,long param_10,
                 undefined8 param_11,long param_12,undefined8 param_13,long param_14,
                 undefined8 param_15,long param_16,long param_17,undefined8 param_18,
                 undefined8 param_19,undefined8 *param_20,undefined8 param_21,long param_22,
                 long param_23)

{
  long lVar1;
  long lVar2;
  long lVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  long lVar10;
  long lVar11;
  undefined8 *puVar12;
  FILE *pFVar13;
  char *pcVar14;
  undefined8 *puVar15;
  undefined8 uVar16;
  undefined8 uVar17;
  undefined8 uVar18;
  undefined8 uVar19;
  undefined8 uVar20;
  undefined8 uVar21;
  undefined8 uVar22;
  long lVar23;
  bool bVar24;
  bool bVar25;
  bool bVar26;
  long lVar27;
  long local_188;
  long local_e0;
  undefined1 *local_d8;
  long local_d0;
  undefined8 local_c8;
  time_t local_c0;
  undefined1 local_b8 [16];
  undefined1 local_a8 [16];
  undefined1 local_98 [16];
  undefined1 local_88 [16];
  undefined1 local_78 [16];
  undefined1 local_68 [16];
  undefined1 local_58 [16];
  undefined1 local_48 [24];
  
  local_e0 = 0;
  local_d8 = (undefined1 *)0x0;
  lVar1 = BKNqLISBgKkBJXSPAIPP();
  local_d0 = 0;
  lVar2 = BKNrEOByAPgKnLLI();
  lVar3 = BKNeBKkyQqQyQPXSPAIPP();
  if (param_2 == 0) {
    SPAIPPqQyQRPXSPAIPP = 0;
    return 0;
  }
  BIBJPgKnOLzXSPAIPP = 0;
  RMMBOYlxXSPAIPP = 0;
  SyOPgKnOLzXSPAIPP = param_2;
  KLABPgKnOLzXSPAIPP = param_2;
  uVar4 = KBTrFJBXSPAIPP();
  uVar5 = KBTrFJBXSPAIPP();
  uVar6 = KBTrFJBXSPAIPP();
  uVar7 = KBTrFJBXSPAIPP();
  uVar8 = KBTrFJBXSPAIPP();
  uVar9 = KBTrFJBXSPAIPP();
  ABzRDnOFKQXSPAIPP("********** Starting vsdlss1mem **********\n");
  _yPFeBQrFJBmCbyV(local_98);
  _yPFrFJBPXSPAIPP(uVar6);
  if (kYrpgvXafYlecbXSPAIPP == '\0') {
    if (lVar1 != 0) {
      local_188 = DAT_00e5a510;
      puVar12 = DAT_00e5a518;
      if (param_8 != 0 || param_16 != 0) goto LAB_00967427;
      goto LAB_00966bdb;
    }
    puVar12 = (undefined8 *)OByAdy_QLOmKBaERKHXSPAIPP();
    local_188 = OByApBLOABOtB_XSPAIPP();
    if (param_8 != 0 || param_16 != 0) goto LAB_00967427;
LAB_009671c0:
    local_d0 = 0;
LAB_009671cc:
    lVar3 = KBTtB_QLOXSPAIPP(0,param_2,0);
LAB_009671e0:
    lVar2 = DBQpBAR_BnOLzIBJqFWBdXSPAIPP();
    if (((lVar2 == 0) && (lVar1 != 0)) && (param_14 != 0)) {
      if (_yPFnyOqLISBOd == 0) {
        MBOJRQBbtB_QLOXSPAIPP(local_188,param_2,param_14,lVar3);
      }
      else {
        MBOJRQBbtB_QLOnyO();
      }
    }
    else {
      OByAbtB_QLOdlRJnBOJRQBXSPAIPP(local_188,param_13,param_2,lVar3);
    }
  }
  else {
    bVar24 = lVar1 != 0;
    bVar25 = param_20 != (undefined8 *)0x0;
    if (((bVar24) && (bVar25)) && (lVar3 - 1U < 2)) {
      DBKkyQqQyQP(param_20[1],*param_20,param_20[9],param_20[5],param_20[7],param_20[8]);
    }
    lVar10 = BKNmOABOmKIVXSPAIPP();
    if ((lVar10 != 0) && (bVar24)) {
      lVar10 = DBQnCryDXSPAIPP();
      if (-1 < lVar10) {
        ABIBQBryDXSPAIPP(lVar10);
      }
      if (bVar25) {
        if (param_20[9] != 0) {
          _yPFdOBBXSPAIPP();
        }
        param_20[9] = 0;
        if (param_20[8] != 0) {
          _yPFdOBBXSPAIPP();
        }
        param_20[8] = 0;
      }
    }
    _yPFeBQrFJBmCbyV();
    local_c0 = time((time_t *)0x0);
    pFVar13 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar14 = ctime(&local_c0);
    fprintf(pFVar13,"Time Stamp at %s : %s","Start of Reordering",pcVar14);
    fflush(pFVar13);
    _yPFrFJBPXSPAIPP(uVar4);
    _yPFrFJBPXSPAIPP();
    local_c0 = time((time_t *)0x0);
    pFVar13 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar14 = ctime(&local_c0);
    fprintf(pFVar13,"Time Stamp at %s : %s","Before Minimum Degree (Array based)",pcVar14);
    fflush(pFVar13);
    puVar12 = (undefined8 *)KBTmKBdy_QLOaERKHfByABOXSPAIPP(param_2);
    if (bVar24) {
      PBQqBJFnBOJdXSPAIPP(*puVar12);
    }
    lVar10 = KBTgtB_QLOXSPAIPP(param_2,0,0);
    if ((param_8 == 0) || (param_17 != 0)) {
      lVar11 = BKNmOABOqBIB_QLOXSPAIPP();
      if (lVar11 != 0) {
        local_d8 = (undefined1 *)KBTatB_QLOXSPAIPP(param_2,1,0);
        *local_d8 = 0;
      }
    }
    else {
      DBQpPBQYKAkyOHtB_PXSPAIPP(param_9,param_2,&local_e0,&local_d8);
    }
    lVar11 = BKNmOABOqBIB_QLOXSPAIPP();
    if (lVar11 == 0) {
      kjbmOABOXSPAIPP(param_2,param_5,param_12,param_20,param_18,param_19,lVar10,puVar12,&local_d0);
      PBQdzPMF();
      if (lVar3 == 3) {
        DBKdzPMFmCCAFyDqQyQP(local_d0,puVar12);
      }
    }
    else {
      if (param_16 != 0) {
        KOBOOLOXSPAIPP(1,"vsdlss1mem.c",0x214);
      }
      uVar22 = DBQeOyMEYOOdOLJcIBJYOOXSPAIPP(param_2,param_5,param_12,param_20);
      ABzRDnOFKQXSPAIPP("Graph (Array based) has been obtained.\n");
      JFKbBDOBBmOABOYOOXSPAIPP(param_8,param_9,local_e0,local_d8,uVar22,lVar10,puVar12);
      QOyKPCBOeOyMErLdy_QLOaERKHqLOQXSPAIPP(lVar10,uVar22,puVar12);
      ABIBQBeOyMEXSPAIPP();
      PBQdzPMF();
      ILKDtB_PgKda = 0;
    }
    local_c0 = time((time_t *)0x0);
    pFVar13 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar14 = ctime(&local_c0);
    fprintf(pFVar13,"Time Stamp at %s : %s","After  Minimum Degree (Array based)",pcVar14);
    fflush(pFVar13);
    lVar11 = puVar12[8];
    if (bVar24) {
      uVar22 = QyDlBTgtB_QLOXSPAIPP(*puVar12,puVar12[2],0,0);
      puVar12[8] = uVar22;
    }
    else {
      uVar22 = KBTgtB_QLOXSPAIPP(puVar12[2],0,0);
      puVar12[8] = uVar22;
    }
    uVar22 = zOByHRMZIL_HPXSPAIPP(lVar11,puVar12[5],0x80,uVar22);
    bVar26 = param_16 != 0;
    puVar12[5] = uVar22;
    if ((param_17 != 0) && (bVar26)) {
      _LMVgtB_QLOXSPAIPP(puVar12[8],uVar22,lVar11);
      uVar22 = zOByHRMZIL_HYQnyOQdy_QZLRKAyOV(lVar11,puVar12[5],param_9,puVar12[8]);
      puVar12[5] = uVar22;
    }
    puVar12[7] = uVar22;
    if (lVar11 != 0) {
      _yPFdOBBXSPAIPP(lVar11);
    }
    _yPFrFJBPXSPAIPP(uVar9);
    AFCCrFJBPXSPAIPP(uVar8,uVar9,QXJFKbBDOBBXSPAIPP);
    ySyFIkBJXSPAIPP = param_1 * DAT_00abc298;
    local_c0 = time((time_t *)0x0);
    zIL_HPgKnOLzXSPAIPP = puVar12[5];
    _ERKHPgKnOLzXSPAIPP = 1;
    _yPFrFJBPXSPAIPP(uVar5);
    AFCCrFJBPXSPAIPP(uVar4,uVar5,QXOBLOABOXSPAIPP);
    local_c0 = time((time_t *)0x0);
    pFVar13 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar14 = ctime(&local_c0);
    fprintf(pFVar13,"Time Stamp at %s : %s","End of Reordering",pcVar14);
    fflush(pFVar13);
    _yPFeBQrFJBmCbyV(local_48);
    BIyMPBApBLOABOrFJB = _yPFbFCCrFJBqB_P(local_48,local_58);
    DBQdy_QLOqQyQPmKBaERKHXSPAIPP
              (puVar12,&JyUaERKHqFWBXSPAIPP,&ySDaERKHqFWBXSPAIPP,&Cy_QLOgKABUqFWBXSPAIPP,
               &Cy_QLOtyIRBqFWBXSPAIPP,&Cy_QLOqFWBXSPAIPP,&Cy_QLOlxXSPAIPP,&LMPrLdy_QLOXSPAIPP,
               &LMPdLOrOFyKDqLISBPXSPAIPP,&local_c8);
    ABzRDnOFKQXSPAIPP();
    DAT_00e5a4f8 = puVar12[2];
    DAT_00e5a4f0 = local_c8;
    DAT_00e5a4e8 = Cy_QLOlxXSPAIPP;
    if ((param_22 != 0) && (param_23 != 0)) {
      RMMBOYlxXSPAIPP = param_2 + param_5;
      BIBJPgKnOLzXSPAIPP = param_5;
      ABIBQBcIBJYOOlLfByABO(param_20);
      _yPFrFJBPXSPAIPP(uVar7);
      AFCCrFJBPXSPAIPP(uVar6,uVar7,QXOBAR_BAqLISBXSPAIPP);
LAB_009670f5:
      _yPFeBQrFJBmCbyV(local_88);
      BIyMPBApBAR_BAqLISBrFJB = _yPFbFCCrFJBqB_P(local_88,local_98);
      SPAIPPqQyQRPXSPAIPP = 0;
      return 0;
    }
    lVar11 = BKNmOABOmKIVXSPAIPP();
    if (lVar11 != 0) {
      if (bVar25) {
        ABIBQBcIBJYOOlLfByABO(param_20);
      }
      RMMBOYlxXSPAIPP = param_5 + param_2;
      BIBJPgKnOLzXSPAIPP = param_5;
      _yPFrFJBPXSPAIPP(uVar7);
      AFCCrFJBPXSPAIPP(uVar6,uVar7,QXOBAR_BAqLISBXSPAIPP);
      lVar1 = KBTtB_QLOXSPAIPP(0,param_2,1);
      TOFQBtB_rLdFIBXSPAIPP(param_11,param_2,lVar1);
      if (lVar1 != 0) {
        _yPFdOBBXSPAIPP(lVar1);
      }
      goto LAB_009670f5;
    }
    local_c0 = time((time_t *)0x0);
    pFVar13 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar14 = ctime(&local_c0);
    fprintf(pFVar13,"Time Stamp at %s : %s","Start Factor Initialization",pcVar14);
    fflush(pFVar13);
    _yPFrFJBPXSPAIPP(uVar4);
    if (bVar24) {
      lVar11 = DBQpBAR_BnOLzIBJqFWBdXSPAIPP();
      if (lVar11 == 0) {
        if (local_d0 != 0) {
          lVar23 = (param_2 - *(long *)(local_d0 + 0x10)) * lVar2;
          DAT_00e5a500 = QyDlBTbtB_QLOXSPAIPP(0,*puVar12,param_2 + lVar23,0);
          uVar16 = 0;
          lVar11 = DAT_00e5a500 + param_2 * 8;
          uVar17 = 0;
          uVar20 = 0;
          uVar22 = 0xffffffffffffffff;
          uVar19 = 0;
          uVar18 = 0;
          lVar27 = local_d0;
LAB_00967e2d:
          PBQnMMFgKCLhOQ(uVar22,uVar20,uVar19,uVar18,uVar17,uVar16,lVar11,lVar23,lVar27);
          PBQnCgKCLhOQXSPAIPP(0xffffffffffffffff,0,0,0);
        }
      }
      else if (local_d0 != 0) {
        lVar11 = param_3 + param_2 + param_4;
        MOL_BPPnCPdLOnyO(local_d0,lVar11,param_2,lVar10,param_16);
        lVar23 = (param_2 - *(long *)(local_d0 + 0x10)) * lVar2;
        uVar22 = DBQnMMFryDqySB();
        local_188 = param_14;
        if (param_14 == 0) {
          local_188 = QyDlBTbtB_QLOXSPAIPP(0,uVar22,lVar11 + lVar23,0);
        }
        if (param_16 == 0) {
          uVar16 = DBQnCbBD1bBD2nyOQpBPXSPAIPP();
          _LMVtB_QLOXSPAIPP(uVar16,lVar11,local_188);
        }
        PBQnMMFpBPRIQtB_qySB(local_188);
        PBQnMMFpBApEPpBPqySB(local_188 + (param_3 + param_4) * 8);
        if (param_14 == 0) {
          PBQdzPnyOuLOHtB_PYKAqFWB(local_188 + lVar11 * 8,lVar23);
        }
        else {
          PBQdzPnyOuLOHtB_PYKAqFWB(0,0);
        }
        uVar16 = DBQnCryDXSPAIPP();
        ABIBQBryDXSPAIPP(uVar16);
        PBQnCgKCLnQOPrLlRIIXSPAIPP();
        lVar27 = local_d0;
        lVar11 = DBQdzPnyOuLOHtB_P();
        uVar16 = DBQnMMFpBApEPpBPqySB();
        uVar17 = DBQnMMFpBPRIQtB_qySB();
        uVar18 = DBQnMMFpBLOABOtB_qySB();
        uVar19 = DBQaMCqySB();
        uVar20 = DBQnMMFqySB();
        goto LAB_00967e2d;
      }
    }
    local_188 = lVar10;
    if (bVar25) {
      OBKRJzBOqLOQcIFJbRMgKkBJ(param_20,param_2,param_5,param_12,lVar10);
      if (bVar24) {
        if ((((param_8 != 0) && (!bVar26)) || (lVar11 = DBQpBAR_BnOLzIBJqFWBdXSPAIPP(), lVar11 == 0)
            ) || (DAT_00e5a508 == 0)) goto LAB_00967cc7;
        if (lVar10 != 0) {
          _yPFdOBBXSPAIPP();
        }
        local_188 = 0;
      }
LAB_0096788d:
      yIIL_gKFQtyItB_PdajBPPkBJXSPAIPP(param_20,puVar12);
      ABIBQBcIBJYOOlLfByABO(param_20);
    }
    else {
      if (ILKDtB_PgKda == 0) {
        yIIL_tyIRBtB_Pdy_QLOmKBaERKHXSPAIPP(puVar12);
      }
      else {
        yIIL_jLKDtyItB_Pdy_QLOmKBaERKHXSPAIPP();
      }
      if (bVar24) {
LAB_00967cc7:
        local_188 = QyDlBTgtB_QLOXSPAIPP(*puVar12,param_2,0,0);
        _LMVgtB_QLOXSPAIPP(lVar10,param_2,local_188);
        if (lVar10 != 0) {
          _yPFdOBBXSPAIPP();
        }
        if (bVar25) goto LAB_0096788d;
      }
      FKFQdy_QLOmKBaERKHXSPAIPP(param_5,param_12,local_188,puVar12);
      _yPFpBJLSBXSPAIPP(*(undefined8 *)(param_12 + 8));
      _yPFpBJLSBXSPAIPP(*(undefined8 *)(param_12 + 0x10));
      _yPFpBJLSBXSPAIPP(*(undefined8 *)(param_12 + 0x18));
      _yPFpBJLSBXSPAIPP(*(undefined8 *)(param_12 + 0x20));
    }
    RMMBOYlxXSPAIPP = param_5 + param_2;
    BIBJPgKnOLzXSPAIPP = param_5;
    _yPFrFJBPXSPAIPP(uVar5);
    AFCCrFJBPXSPAIPP(uVar4,uVar5,QXMBOJRQBYXSPAIPP);
    local_c0 = time((time_t *)0x0);
    pFVar13 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar14 = ctime(&local_c0);
    fprintf(pFVar13,"Time Stamp at %s : %s","End   Factor Initialization",pcVar14);
    fflush(pFVar13);
    ABzRDnOFKQXSPAIPP("Factor initialized\n");
    if (local_d0 != 0) {
      CFKFPEYKAq_EBARIBnyOgKQBOSyIP(local_d0,puVar12);
    }
    ABzRDnOFKQXSPAIPP("Start in-memory factorization\n");
    _yPFeBQrFJBmCbyV();
    local_c0 = time((time_t *)0x0);
    pFVar13 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar14 = ctime(&local_c0);
    fprintf(pFVar13,"Time Stamp at %s : %s","Start in-memory Factorization",pcVar14);
    fflush(pFVar13);
    _yPFrFJBPXSPAIPP(uVar4);
    lVar10 = KBTgtB_QLOXSPAIPP(puVar12[2],0,0);
    DBQpLTrLZIL_HtB_XSPAIPP(puVar12[5],puVar12[8],lVar10);
    if (((bVar24) && (_yPFnyOqLISBOd != 0)) && ((local_d0 != 0 && (1 < *(long *)(local_d0 + 8))))) {
      Cy_QLOYmKBaERKHnyO(lVar10,param_8,param_9,puVar12,local_d0,param_21);
    }
    else {
      Cy_QLOYmKBaERKHlBTXSPAIPP(lVar10,param_8,param_9,puVar12);
    }
    if (lVar10 != 0) {
      _yPFdOBBXSPAIPP(lVar10);
    }
    _yPFrFJBPXSPAIPP(uVar5);
    AFCCrFJBPXSPAIPP(uVar4,uVar5,QXCy_QLOXSPAIPP);
    _yPFeBQrFJBmCbyV(local_68);
    BIyMPBAdy_QLOrFJB = _yPFbFCCrFJBqB_P(local_68,local_78);
    local_c0 = time((time_t *)0x0);
    pFVar13 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar14 = ctime(&local_c0);
    fprintf(pFVar13,"Time Stamp at %s : %s","End   in-memory Factorization",pcVar14);
    fflush(pFVar13);
    ABzRDnOFKQXSPAIPP("End   in-memory factorization\n");
    if (((bVar24) && (lVar10 = DBQpBAR_BnOLzIBJqFWBdXSPAIPP(), lVar10 != 0)) &&
       ((local_d0 != 0 && (param_14 != 0)))) {
      lVar23 = (param_2 - *(long *)(local_d0 + 0x10)) * lVar2;
      lVar11 = param_2 + param_3 + param_4;
      uVar22 = DBQnMMFryDqySB();
      lVar10 = QyDlBTbtB_QLOXSPAIPP(0,uVar22,lVar11 + lVar23,0);
      if (param_16 == 0) {
        _LMVtB_QLOXSPAIPP(param_14,lVar11,lVar10);
      }
      PBQnMMFpBPRIQtB_qySB(lVar10);
      PBQnMMFpBApEPpBPqySB(lVar10 + (param_3 + param_4) * 8);
      PBQdzPnyOuLOHtB_PYKAqFWB(lVar10 + lVar11 * 8,lVar23);
      lVar10 = local_d0;
      uVar16 = DBQdzPnyOuLOHtB_P();
      uVar17 = DBQnMMFpBApEPpBPqySB();
      uVar18 = DBQnMMFpBPRIQtB_qySB();
      uVar19 = DBQnMMFpBLOABOtB_qySB();
      uVar20 = DBQaMCqySB();
      uVar21 = DBQnMMFqySB();
      PBQnMMFgKCLhOQ(uVar22,uVar21,uVar20,uVar19,uVar18,uVar17,uVar16,lVar23,lVar10);
    }
    if ((param_8 == 0) || (param_16 != 0)) {
      if ((qglejcXqmjtcXmljwXSPAIPP == '\0') || (bVar26)) {
        if (bVar24) {
          DAT_00e3a0e0 = *puVar12;
          DAT_00e5a510 = local_188;
          DAT_00e5a518 = puVar12;
          PBQd_gKCLhOQXSPAIPP(DAT_00e3a0e0,puVar12,local_188,DAT_00e5a508);
        }
        else {
          TOFQBdy_QLOmKBaERKHXSPAIPP(puVar12);
          TOFQBpBLOABOtB_XSPAIPP(local_188,param_2);
        }
      }
    }
    else {
      JyHBnyOQdy_QrORBaELIBPHVXSPAIPP(param_9,puVar12);
      OBKRJuOFQBpBAkyQXSPAIPP(param_9,puVar12);
      TOFQBgKSkyMYPnBOJXSPAIPP(param_2,local_188);
      if (param_10 == 0) {
        TOFQBnyOQdy_QdOLJd_XSPAIPP(param_9,puVar12);
        puVar12 = (undefined8 *)ABIBQBdy_QLOaERKHXSPAIPP(puVar12);
      }
      else {
        puVar15 = (undefined8 *)KBTtB_QLOXSPAIPP(0,param_2,0);
        *puVar15 = 0;
        OByAbtB_QLOdlRJnBOJRQBXSPAIPP(local_188,param_13,param_2,puVar15);
        PLISBjLTBOrOFyKDdLOnyOQdy_QXSPAIPP(param_9,puVar12,puVar15);
        puVar12 = (undefined8 *)ABIBQBdy_QLOaERKHXSPAIPP(puVar12);
        TOFQBbtB_QLOdlRJXSPAIPP(0x25,param_9,puVar15 + (param_2 - param_9));
        _yPFdOBBXSPAIPP(puVar15);
      }
      if (local_e0 != 0) {
        _yPFdOBBXSPAIPP();
      }
      local_e0 = 0;
    }
    if (local_d8 != (undefined1 *)0x0) {
      _yPFdOBBXSPAIPP();
    }
    local_d8 = (undefined1 *)0x0;
    if (bVar24) {
      if ((ILKDtB_PgKda != 0) && (_LKSBOQd_rLjLKDtyItB_PXj(puVar12,local_d0), local_d0 != 0)) {
        OB_LJMRQBqQyOQcKAZIL_HPdLOgKQBOSyIP(local_d0,puVar12);
      }
      DBQpBAR_BnOLzIBJqFWBdXSPAIPP();
    }
    if (param_22 != 0) {
      SPAIPPqQyQRPXSPAIPP = 0;
      return 0;
    }
    if (lVar3 == 4) {
      DBKd_mCCAFyDtyIRBqQyQP(puVar12);
    }
    if (param_8 != 0 || param_16 != 0) goto LAB_00967427;
    if (!bVar24) goto LAB_009671c0;
LAB_00966bdb:
    local_d0 = DBQdzPMF();
    lVar3 = DBQpBAR_BnOLzIBJqFWBdXSPAIPP();
    if (lVar3 == 0) {
      if (local_d0 == 0) goto LAB_009671cc;
      lVar2 = (param_2 - *(long *)(local_d0 + 0x10)) * lVar2;
      lVar3 = DAT_00e5a500;
      if (lVar1 == 0) {
        lVar3 = KBTtB_QLOXSPAIPP(0,param_2 + lVar2,0);
      }
      PBQdzPnyOuLOHtB_PYKAqFWB(lVar3 + param_2 * 8,lVar2);
      goto LAB_009671e0;
    }
    lVar2 = DBQaMCqySB();
    if (lVar2 == 0) {
      lVar2 = DBQnCpBAR_BApEPXSPAIPP();
    }
    else {
      lVar2 = DBQnMMFpBApEPpBPqySB();
    }
    lVar3 = lVar2;
    if (DAT_00e5a508 == 0) {
      lVar3 = param_14;
      if (param_14 == 0) {
        lVar3 = KBTtB_QLOXSPAIPP(0,param_2,0);
      }
      if (_yPFnyOqLISBOd == 0) {
        MBOJRQBbtB_QLOXSPAIPP(local_188,param_2,lVar2,lVar3);
      }
      else {
        MBOJRQBbtB_QLOnyO();
      }
    }
  }
  ABzRDnOFKQXSPAIPP();
  local_c0 = time((time_t *)0x0);
  pFVar13 = (FILE *)DBQdFIBnQOXSPAIPP(9);
  pcVar14 = ctime(&local_c0);
  fprintf(pFVar13,"Time Stamp at %s : %s","Start solve one load case",pcVar14);
  fflush(pFVar13);
  _yPFrFJBPXSPAIPP(uVar4);
  _yPFeBQrFJBmCbyV(local_b8);
  if ((local_d0 == 0) || (lVar2 = DBQdzPnyOuLOHtB_P(), lVar2 == 0)) {
    PLISBjLyAayPBZIL_HBAXSPAIPP(puVar12,lVar3);
  }
  else {
    PLISBjLyAayPBZIL_HBAXJQ(puVar12,local_d0,lVar3);
  }
  _yPFeBQrFJBmCbyV(local_a8);
  BIyMPBAjLyAayPBqLISBrFJB = _yPFbFCCrFJBqB_P(local_a8,local_b8);
  _yPFrFJBPXSPAIPP(uVar5);
  y__RJbFCCrFJBPXSPAIPP(uVar4,uVar5,QXILTBOqLISBXSPAIPP);
  local_c0 = time((time_t *)0x0);
  pFVar13 = (FILE *)DBQdFIBnQOXSPAIPP(9);
  pcVar14 = ctime(&local_c0);
  fprintf(pFVar13,"Time Stamp at %s : %s","End   solve one load case",pcVar14);
  fflush(pFVar13);
  ABzRDnOFKQXSPAIPP("End   solve one load case\n");
  if (lVar1 == 0) {
    ABIBQBdy_QLOaERKHXSPAIPP(puVar12);
    ABzRDnOFKQXSPAIPP();
    local_c0 = time((time_t *)0x0);
    pFVar13 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar14 = ctime(&local_c0);
    fprintf(pFVar13,"Time Stamp at %s : %s","Start Write/Save Reduced Results",pcVar14);
    fflush(pFVar13);
LAB_009676c7:
    lVar2 = DBQpBAR_BnOLzIBJqFWBdXSPAIPP();
    if (((lVar2 == 0) && (lVar1 != 0)) && (param_14 != 0)) {
      if (_yPFnyOqLISBOd == 0) {
        FKSnBOJRQBbtB_QLOXSPAIPP(local_188,param_2,lVar3,param_14);
      }
      else {
        FKSnBOJRQBbtB_QLOnyO();
      }
    }
    else {
      FKSnBOJRQBbtB_QLOgKnIy_BXSPAIPP(local_188,param_2,lVar3);
      TOFQBtB_rLdFIBXSPAIPP(param_11,param_2,lVar3);
    }
    if (local_d0 == 0) {
      if (lVar3 != 0) {
        _yPFdOBBXSPAIPP();
      }
      if (lVar1 == 0) goto LAB_00967746;
    }
    else {
      PBQdzPnyOuLOHtB_PYKAqFWB(0,0);
      if (lVar1 == 0) {
        if (lVar3 != 0) {
          _yPFdOBBXSPAIPP(lVar3);
        }
LAB_00967746:
        if (local_188 != 0) {
          _yPFdOBBXSPAIPP();
        }
      }
    }
  }
  else {
    ABzRDnOFKQXSPAIPP();
    local_c0 = time((time_t *)0x0);
    pFVar13 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar14 = ctime(&local_c0);
    fprintf(pFVar13,"Time Stamp at %s : %s","Start Write/Save Reduced Results",pcVar14);
    fflush(pFVar13);
    lVar2 = DBQpBAR_BnOLzIBJqFWBdXSPAIPP();
    if (lVar2 == 0) goto LAB_009676c7;
    if (DAT_00e5a508 == 0) {
      lVar1 = DBQaMCqySB();
      if (lVar1 == 0) {
        uVar22 = DBQnCpBAR_BApBPRIQXSPAIPP();
      }
      else {
        uVar22 = DBQnMMFpBApEPpBPqySB();
      }
      if (_yPFnyOqLISBOd == 0) {
        FKSnBOJRQBbtB_QLOXSPAIPP(local_188,param_2,lVar3,uVar22);
      }
      else {
        FKSnBOJRQBbtB_QLOnyO();
      }
      if ((param_14 == 0) && (lVar3 != 0)) {
        _yPFdOBBXSPAIPP();
      }
    }
  }
  local_c0 = time((time_t *)0x0);
  pFVar13 = (FILE *)DBQdFIBnQOXSPAIPP(9);
  pcVar14 = ctime(&local_c0);
  fprintf(pFVar13,"Time Stamp at %s : %s","End   Write/Save Reduced Results",pcVar14);
  fflush(pFVar13);
  ABzRDnOFKQXSPAIPP("End   writing/saving result file\n");
LAB_00967427:
  _yPFrFJBPXSPAIPP(uVar7);
  AFCCrFJBPXSPAIPP(uVar6,uVar7,QXOBAR_BAqLISBXSPAIPP);
  ABIBQBrFJBXSPAIPP(uVar4);
  ABIBQBrFJBXSPAIPP(uVar5);
  ABIBQBrFJBXSPAIPP(uVar6);
  ABIBQBrFJBXSPAIPP(uVar7);
  ABIBQBrFJBXSPAIPP(uVar8);
  ABIBQBrFJBXSPAIPP(uVar9);
  _yPFeBQrFJBmCbyV(local_88);
  BIyMPBApBAR_BAqLISBrFJB = _yPFbFCCrFJBqB_P(local_88,local_98);
  SPAIPPqQyQRPXSPAIPP = 0;
  ABzRDnOFKQXSPAIPP("********** vsdlss1mem ended with %ld status code **********\n",0);
  return SPAIPPqQyQRPXSPAIPP;
}


