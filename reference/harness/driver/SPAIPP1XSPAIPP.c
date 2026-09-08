/* driver: SPAIPP1XSPAIPP addr=00965610 size=d47 */
#include "vsdlss_ref.h"


undefined8
SPAIPP1XSPAIPP(double param_1,long param_2,undefined8 param_3,undefined8 param_4,undefined8 param_5,
              undefined8 param_6,undefined8 param_7,undefined8 param_8)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  FILE *pFVar7;
  char *pcVar8;
  long lVar9;
  undefined8 uVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  undefined8 uVar15;
  long local_70;
  long local_68;
  long local_60;
  undefined8 local_58;
  long local_50;
  long local_48;
  time_t local_40 [2];
  
  if (param_2 == 0) {
LAB_00965639:
    SPAIPPqQyQRPXSPAIPP = 0;
  }
  else {
    BIBJPgKnOLzXSPAIPP = 0;
    RMMBOYlxXSPAIPP = 0;
    SyOPgKnOLzXSPAIPP = param_2;
    KLABPgKnOLzXSPAIPP = param_2;
    uVar1 = KBTrFJBXSPAIPP();
    uVar2 = KBTrFJBXSPAIPP();
    uVar3 = KBTrFJBXSPAIPP();
    uVar4 = KBTrFJBXSPAIPP();
    uVar5 = KBTrFJBXSPAIPP();
    uVar6 = KBTrFJBXSPAIPP();
    ABzRDnOFKQXSPAIPP("********** Starting vsdlss1 **********\n");
    _yPFrFJBPXSPAIPP();
    if (kYrpgvXafYlecbXSPAIPP == '\0') {
      lVar11 = OByAaERKHgKCLXSPAIPP();
      lVar9 = OByAZIL_HgKCLXSPAIPP();
    }
    else {
      local_40[0] = time((time_t *)0x0);
      pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
      pcVar8 = ctime(local_40);
      fprintf(pFVar7,"Time Stamp at %s : %s","Start of Reordering",pcVar8);
      fflush(pFVar7);
      _yPFrFJBPXSPAIPP(uVar1);
      local_70 = DBQeOyMEdOLJcIBJYOOXSPAIPP(param_2,param_3,param_7);
      lVar13 = *(long *)(local_70 + 8);
      TOFQBeOyMEXSPAIPP(local_70);
      ABzRDnOFKQXSPAIPP("Graph has been obtained and saved\n");
      _yPFrFJBPXSPAIPP();
      local_40[0] = time((time_t *)0x0);
      pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
      pcVar8 = ctime(local_40);
      fprintf(pFVar7,"Time Stamp at %s : %s","Before Minimum Degree",pcVar8);
      fflush(pFVar7);
      JFKbBDOBBmOABOXSPAIPP(&local_70,&local_68,&local_60,&local_58);
      local_70 = ABIBQBeOyMEXSPAIPP();
      local_40[0] = time((time_t *)0x0);
      pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
      pcVar8 = ctime(local_40);
      fprintf(pFVar7,"Time Stamp at %s : %s","After  Minimum Degree",pcVar8);
      fflush(pFVar7);
      _yPFrFJBPXSPAIPP(uVar6);
      AFCCrFJBPXSPAIPP(uVar5,uVar6,QXJFKbBDOBBXSPAIPP);
      lVar9 = DBQpLLQPgKcIFJrOBBXSPAIPP(local_68,param_2);
      if (lVar9 == 0) {
        KOBOOLOXSPAIPP(1,"vsdlss1.c",0x70);
      }
      uVar10 = _LKSBOQaEFIArLnyOBKQrLeOyMEXSPAIPP(local_68,param_2);
      if (local_68 != 0) {
        _yPFdOBBXSPAIPP();
      }
      local_68 = 0;
      ABzRDnOFKQXSPAIPP("Elimination tree vector is converted to graph\n");
      ySyFIkBJXSPAIPP = param_1 * DAT_00abc298;
      lVar11 = 0x12;
      if (param_2 < 0x13) {
        lVar11 = param_2;
      }
      Cy_QLOZIL_HkBJXSPAIPP = _LJMRQBdy_QLOZIL_HkBJXSPAIPP(param_2,local_60,lVar11);
      ABzRDnOFKQXSPAIPP("Start breakupIndistNodeSets\n");
      zOByHRMgKAFPQlLABqBQPXSPAIPP(Cy_QLOZIL_HkBJXSPAIPP,lVar11,local_60,local_58);
      ABzRDnOFKQXSPAIPP("End   breakupIndistNodeSets\n");
      uVar15 = _LJMRQBmSBOEByAkBJXSPAIPP(local_58,local_60);
      Cy_QLOaERKHkBJXSPAIPP =
           _LJMRQBdy_QLOaERKHkBJXSPAIPP(ySyFIkBJXSPAIPP,Cy_QLOZIL_HkBJXSPAIPP,uVar15,param_2);
      ABzRDnOFKQXSPAIPP();
      local_40[0] = time((time_t *)0x0);
      pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
      pcVar8 = ctime(local_40);
      fprintf(pFVar7,"Time Stamp at %s : %s","Before Divide into Chunks",pcVar8);
      fflush(pFVar7);
      _yPFrFJBPXSPAIPP(uVar5);
      lVar11 = AFSFABgKQLaERKHPXSPAIPP
                         (Cy_QLOaERKHkBJXSPAIPP,param_4,param_5,uVar10,lVar9,local_60,local_58,
                          &local_50,&local_48);
      _yPFrFJBPXSPAIPP(uVar6);
      AFCCrFJBPXSPAIPP(uVar5,uVar6,QXAFSFABgKQLaERKHPXSPAIPP);
      local_40[0] = time((time_t *)0x0);
      pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
      pcVar8 = ctime(local_40);
      fprintf(pFVar7,"Time Stamp at %s : %s","After  Divide into Chunks",pcVar8);
      fflush(pFVar7);
      _ERKHPgKnOLzXSPAIPP = *(undefined8 *)(lVar11 + 0x10);
      ABIBQBeOyMEXSPAIPP(uVar10);
      ABIBQBcKQFOBacADBIFPQXSPAIPP(lVar9);
      lVar9 = DBQZIL_HgKCLdOLJgKPXSPAIPP(param_2,local_50,&local_58);
      zIL_HPgKnOLzXSPAIPP = *(undefined8 *)(lVar9 + 8);
      local_58 = ABIBQBgKAFPQlLABqBQPXSPAIPP(local_58);
      ABzRDnOFKQXSPAIPP("BlockInfo has been obtained\n");
      _yPFrFJBPXSPAIPP(uVar2);
      AFCCrFJBPXSPAIPP(uVar1,uVar2,QXOBLOABOXSPAIPP);
      local_40[0] = time((time_t *)0x0);
      pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
      pcVar8 = ctime(local_40);
      fprintf(pFVar7,"Time Stamp at %s : %s","End of Reordering;Start Read Graph",pcVar8);
      fflush(pFVar7);
      local_70 = OByAeOyMEXSPAIPP();
      ABzRDnOFKQXSPAIPP();
      local_40[0] = time((time_t *)0x0);
      pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
      pcVar8 = ctime(local_40);
      fprintf(pFVar7,"Time Stamp at %s : %s","End   Read Graph;Start Symbolic Elimination",pcVar8);
      fflush(pFVar7);
      _yPFrFJBPXSPAIPP(uVar1);
      uVar10 = PVJzLIF_cIFJFKyQFLKXSPAIPP(&local_70,lVar9,lVar11,local_60,local_50,local_48);
      _yPFrFJBPXSPAIPP(uVar2);
      AFCCrFJBPXSPAIPP(uVar1,uVar2,QXPVJcIFJXSPAIPP);
      local_40[0] = time((time_t *)0x0);
      pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
      pcVar8 = ctime(local_40);
      fprintf(pFVar7,"Time Stamp at %s : %s","End   Symbolic Elimination",pcVar8);
      fflush(pFVar7);
      if (local_48 != 0) {
        _yPFdOBBXSPAIPP();
      }
      local_48 = 0;
      if (local_60 != 0) {
        _yPFdOBBXSPAIPP();
      }
      local_60 = 0;
      local_70 = ABIBQBeOyMEXSPAIPP(local_70);
      _yPFpBJLSBXSPAIPP(0xb);
      CIRPEaBIdOBBqQOR_QROBPXSPAIPP();
      ABzRDnOFKQXSPAIPP("Preprocessing for reordering complete\n");
      DBQdy_QLOqQyQPXSPAIPP
                (lVar11,lVar9,&JyUaERKHqFWBXSPAIPP,&ySDaERKHqFWBXSPAIPP,&Cy_QLOgKABUqFWBXSPAIPP,
                 &Cy_QLOtyIRBqFWBXSPAIPP,&Cy_QLOqFWBXSPAIPP,&Cy_QLOlxXSPAIPP,&LMPrLdy_QLOXSPAIPP,
                 &LMPdLOrOFyKDqLISBPXSPAIPP);
      ABzRDnOFKQXSPAIPP("Factor Stats obtained\n");
      lVar12 = BKNmOABOmKIVXSPAIPP();
      lVar14 = local_50;
      if (lVar12 != 0) {
        _yPFrFJBPXSPAIPP(uVar4);
        AFCCrFJBPXSPAIPP(uVar3,uVar4,QXOBAR_BAqLISBXSPAIPP);
        lVar9 = KBTtB_QLOXSPAIPP(0,param_2,1);
        TOFQBtB_rLdFIBXSPAIPP(param_6,param_2,lVar9);
        if (lVar9 != 0) {
          _yPFdOBBXSPAIPP(lVar9);
        }
        OBJLSBYIIaERKHdFIBPXSPAIPP(lVar11);
        goto LAB_00965639;
      }
      local_50 = 0;
      TOFQBpBLOABOtB_XSPAIPP(lVar14,param_2);
      local_40[0] = time((time_t *)0x0);
      pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
      pcVar8 = ctime(local_40);
      fprintf(pFVar7,"Time Stamp at %s : %s","Start Matrix Permutation",pcVar8);
      fflush(pFVar7);
      _yPFrFJBPXSPAIPP(uVar1);
      lVar12 = OBKRJzBOcIBJYOOYKAuOFQBaERKHdFIBPXSPAIPP(param_2,param_3,param_7,lVar14,lVar11);
      if (lVar12 * 2 != lVar13) {
        KOBOOLOXSPAIPP(1,"vsdlss1.c",0x108);
      }
      RMMBOYlxXSPAIPP = param_2 + lVar12;
      BIBJPgKnOLzXSPAIPP = lVar12;
      if (lVar14 != 0) {
        _yPFdOBBXSPAIPP(lVar14);
      }
      _yPFrFJBPXSPAIPP(uVar2);
      AFCCrFJBPXSPAIPP(uVar1,uVar2,QXMBOJRQBYXSPAIPP);
      local_40[0] = time((time_t *)0x0);
      pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
      pcVar8 = ctime(local_40);
      fprintf(pFVar7,"Time Stamp at %s : %s","End   Matrix Permutation",pcVar8);
      fflush(pFVar7);
      ABzRDnOFKQXSPAIPP("Matrix permuted\n");
      _LJMRQBqMIFQZIL_HdLOaERKHPXSPAIPP(lVar9,lVar11);
      ABzRDnOFKQXSPAIPP();
      local_40[0] = time((time_t *)0x0);
      pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
      pcVar8 = ctime(local_40);
      fprintf(pFVar7,"Time Stamp at %s : %s","Start Factorization",pcVar8);
      fflush(pFVar7);
      _yPFrFJBPXSPAIPP(uVar1);
      Cy_QLOYXSPAIPP(lVar11,lVar9,uVar10);
      _yPFrFJBPXSPAIPP(uVar2);
      AFCCrFJBPXSPAIPP(uVar1,uVar2,QXCy_QLOXSPAIPP);
      local_40[0] = time((time_t *)0x0);
      pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
      pcVar8 = ctime(local_40);
      fprintf(pFVar7,"Time Stamp at %s : %s","End   Factorization",pcVar8);
      fflush(pFVar7);
      ABIBQBYCCB_QBAZVaERKHXSPAIPP(uVar10);
      ABzRDnOFKQXSPAIPP("End   factorization\n");
      if (qglejcXqmjtcXmljwXSPAIPP == '\0') {
        TOFQBaERKHgKCLXSPAIPP(lVar11);
        TOFQBZIL_HgKCLXSPAIPP(lVar9);
      }
    }
    lVar13 = OByApBLOABOtB_XSPAIPP();
    lVar14 = KBTtB_QLOXSPAIPP(0,param_2,0);
    OByAbtB_QLOdlRJXSPAIPP(param_8,param_2,lVar14);
    MBOJRQBbtB_QLOgKnIy_BXSPAIPP(lVar13,param_2,lVar14);
    ABzRDnOFKQXSPAIPP();
    local_40[0] = time((time_t *)0x0);
    pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar8 = ctime(local_40);
    fprintf(pFVar7,"Time Stamp at %s : %s","Start Forward Substitution",pcVar8);
    fflush(pFVar7);
    _yPFrFJBPXSPAIPP(uVar1);
    CLOTyOAqRzPQFQRQBXSPAIPP(lVar11,lVar9,lVar14);
    _yPFrFJBPXSPAIPP(uVar2);
    y__RJbFCCrFJBPXSPAIPP(uVar1,uVar2,QXILTBOqLISBXSPAIPP);
    local_40[0] = time((time_t *)0x0);
    pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar8 = ctime(local_40);
    fprintf(pFVar7,"Time Stamp at %s : %s","End   Forward Substitution",pcVar8);
    fflush(pFVar7);
    ABzRDnOFKQXSPAIPP("End   forwardSubstitute\n");
    ABzRDnOFKQXSPAIPP();
    local_40[0] = time((time_t *)0x0);
    pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar8 = ctime(local_40);
    fprintf(pFVar7,"Time Stamp at %s : %s","Start Back Substitution",pcVar8);
    fflush(pFVar7);
    _yPFrFJBPXSPAIPP(uVar1);
    zy_HqRzPQFQRQBXSPAIPP(lVar11,lVar9,lVar14);
    _yPFrFJBPXSPAIPP(uVar2);
    y__RJbFCCrFJBPXSPAIPP(uVar1,uVar2,QXRMMBOqLISBXSPAIPP);
    local_40[0] = time((time_t *)0x0);
    pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar8 = ctime(local_40);
    fprintf(pFVar7,"Time Stamp at %s : %s","End   Back Substitution",pcVar8);
    fflush(pFVar7);
    ABzRDnOFKQXSPAIPP("End   backSubstitute\n");
    if (qglejcXqmjtcXmljwXSPAIPP != '\0') {
      OBJLSBYIIaERKHdFIBPXSPAIPP(lVar11);
    }
    ABIBQBaERKHgKCLXSPAIPP(lVar11);
    ABIBQBZIL_HgKCLXSPAIPP(lVar9);
    FKSnBOJRQBbtB_QLOgKnIy_BXSPAIPP(lVar13,param_2,lVar14);
    if (lVar13 != 0) {
      _yPFdOBBXSPAIPP(lVar13);
    }
    ABzRDnOFKQXSPAIPP();
    local_40[0] = time((time_t *)0x0);
    pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar8 = ctime(local_40);
    fprintf(pFVar7,"Time Stamp at %s : %s","Start Write Reduced Results",pcVar8);
    fflush(pFVar7);
    TOFQBtB_rLdFIBXSPAIPP(param_6,param_2,lVar14);
    if (lVar14 != 0) {
      _yPFdOBBXSPAIPP();
    }
    local_40[0] = time((time_t *)0x0);
    pFVar7 = (FILE *)DBQdFIBnQOXSPAIPP(9);
    pcVar8 = ctime(local_40);
    fprintf(pFVar7,"Time Stamp at %s : %s","End   Write Reduced Results",pcVar8);
    fflush(pFVar7);
    ABzRDnOFKQXSPAIPP("End   writing result file\n");
    _yPFrFJBPXSPAIPP(uVar4);
    AFCCrFJBPXSPAIPP(uVar3,uVar4,QXOBAR_BAqLISBXSPAIPP);
    ABIBQBrFJBXSPAIPP(uVar1);
    ABIBQBrFJBXSPAIPP(uVar2);
    ABIBQBrFJBXSPAIPP(uVar3);
    ABIBQBrFJBXSPAIPP(uVar4);
    ABIBQBrFJBXSPAIPP(uVar5);
    ABIBQBrFJBXSPAIPP(uVar6);
    SPAIPPqQyQRPXSPAIPP = 0;
    ABzRDnOFKQXSPAIPP("********** vsdlss1 ended with %ld status code **********\n",0);
  }
  return SPAIPPqQyQRPXSPAIPP;
}


