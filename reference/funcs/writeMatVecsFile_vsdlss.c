/* VSDLSS function (decompiled by Ghidra) */
/* name: writeMatVecsFile_vsdlss  addr: 009d3c20  size: 79a */
#include "vsdlss_ref.h"

/* signature: int writeMatVecsFile_vsdlss(int param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,int param_9,int param_10,void *param_11,void *param_12,void *param_13,void *param_14,int param_15,long param_16,long param_17,long param_18,long param_19,undefined8 param_20,int *param_21,int *param_22,void *param_23,void *param_24,void *param_25); */

int writeMatVecsFile_vsdlss
              (int param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
              undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
              int param_9,int param_10,void *param_11,void *param_12,void *param_13,void *param_14,
              int param_15,long param_16,long param_17,long param_18,long param_19,
              undefined8 param_20,int *param_21,int *param_22,void *param_23,void *param_24,
              void *param_25)

{
  double *pdVar1;
  double dVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  size_t sVar6;
  void *__ptr;
  long lVar7;
  long lVar8;
  char *pcVar9;
  FILE *pFVar10;
  size_t sVar11;
  void *__ptr_00;
  long lVar12;
  long lVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  bool bVar17;
  
  iVar4 = param_9 + 1;
  sVar6 = (size_t)iVar4;
  __ptr = tc_malloc(sVar6 * 4);
  initIVector_vsdlss(__ptr,(long)param_9,0);
  bVar17 = param_1 != 0;
  __ptr_00 = (void *)0x0;
  bVar3 = bVar17 || 0 < param_15;
  if ((bVar3) && (__ptr_00 = tc_malloc(sVar6 * 4), 0 < param_15)) {
    lVar8 = 0;
    do {
      iVar14 = *(int *)(param_16 + 4 + lVar8 * 4);
      if (iVar14 != 0) {
        *(undefined4 *)((long)__ptr_00 + (long)iVar14 * 4) = 1;
      }
      iVar14 = *(int *)(param_17 + 4 + lVar8 * 4);
      if (iVar14 != 0) {
        *(undefined4 *)((long)__ptr_00 + (long)iVar14 * 4) = 1;
      }
      iVar14 = *(int *)(param_18 + 4 + lVar8 * 4);
      if (iVar14 != 0) {
        *(undefined4 *)((long)__ptr_00 + (long)iVar14 * 4) = 1;
      }
      iVar14 = *(int *)(param_19 + 4 + lVar8 * 4);
      if (iVar14 != 0) {
        *(undefined4 *)((long)__ptr_00 + (long)iVar14 * 4) = 1;
      }
      iVar14 = (int)lVar8;
      lVar8 = lVar8 + 1;
    } while (iVar14 + 2 <= param_15);
  }
  iVar14 = 0;
  if (0 < param_10) {
    iVar14 = 0;
    lVar8 = (ulong)(param_10 - 1) * 4 + 4;
    lVar7 = 0;
    do {
      while( true ) {
        lVar12 = (long)*(int *)((long)param_11 + lVar7 + 4);
        lVar13 = (long)*(int *)((long)param_12 + lVar7 + 4);
        dVar2 = *(double *)((long)param_13 + lVar7 * 2 + 8);
        if (*(char *)((long)param_23 + lVar12) == '\0') break;
        if (*(char *)((long)param_23 + lVar13) == '\0') {
          pdVar1 = (double *)((long)param_25 + lVar13 * 8);
          iVar14 = iVar14 + 1;
          *pdVar1 = *(double *)((long)param_24 + lVar12 * 8) * dVar2 + *pdVar1;
          if (*(char *)((long)param_23 + lVar12) == '\0') break;
        }
LAB_009d3d77:
        lVar7 = lVar7 + 4;
        if (lVar7 == lVar8) goto LAB_009d3dd0;
      }
      if (*(char *)((long)param_23 + lVar13) == '\0') goto LAB_009d3d77;
      pdVar1 = (double *)((long)param_25 + lVar12 * 8);
      lVar7 = lVar7 + 4;
      iVar14 = iVar14 + 1;
      *pdVar1 = dVar2 * *(double *)((long)param_24 + lVar13 * 8) + *pdVar1;
    } while (lVar7 != lVar8);
  }
LAB_009d3dd0:
  if (0 < param_9) {
    lVar8 = 0;
    do {
      if (*(char *)((long)param_23 + lVar8 + 1) != '\0') {
        *(undefined8 *)((long)param_25 + lVar8 * 8 + 8) = 0;
      }
      lVar8 = lVar8 + 1;
    } while (lVar8 != (ulong)(param_9 - 1) + 1);
  }
  iVar5 = 0;
  if ((!bVar17) && (iVar5 = param_10 - iVar14, param_15 != 0)) {
    if (param_10 < 1) {
      iVar5 = 0;
    }
    else {
      iVar5 = 0;
      lVar8 = 0;
      do {
        lVar12 = (long)*(int *)((long)param_11 + lVar8 + 4);
        lVar7 = (long)*(int *)((long)param_12 + lVar8 + 4);
        if (((*(char *)((long)param_23 + lVar12) == '\0') &&
            (*(char *)((long)param_23 + lVar7) == '\0')) &&
           (*(int *)((long)__ptr_00 + lVar12 * 4) == 0)) {
          iVar5 = iVar5 + (uint)(*(int *)((long)__ptr_00 + lVar7 * 4) == 0);
        }
        lVar8 = lVar8 + 4;
      } while (lVar8 != (ulong)(param_10 - 1) * 4 + 4);
    }
  }
  if (param_9 < 1) {
    iVar14 = 0;
    iVar16 = 0;
    iVar15 = 0;
  }
  else {
    pcVar9 = (char *)((long)param_23 + 1);
    lVar8 = 4;
    iVar16 = 0;
    do {
      if ((*pcVar9 == '\0') && ((param_15 < 1 || (*(int *)((long)__ptr_00 + lVar8) == 0)))) {
        iVar16 = iVar16 + 1;
        *(int *)((long)__ptr + lVar8) = iVar16;
      }
      pcVar9 = pcVar9 + 1;
      lVar8 = lVar8 + 4;
    } while (pcVar9 != (char *)((long)param_23 + (ulong)(param_9 - 1) + 2));
    lVar8 = 0;
    iVar15 = iVar16;
    do {
      if (((*(char *)((long)param_23 + lVar8 + 1) == '\0') && (0 < param_15)) &&
         (*(int *)((long)__ptr_00 + lVar8 * 4 + 4) != 0)) {
        iVar15 = iVar15 + 1;
        *(int *)((long)__ptr + lVar8 * 4 + 4) = iVar15;
      }
      lVar8 = lVar8 + 1;
    } while (lVar8 != (ulong)(param_9 - 1) + 1);
    iVar14 = iVar15 - iVar16;
  }
  if (bVar17) {
    iVar16 = 0;
    iVar14 = iVar15;
  }
  else {
    pFVar10 = (FILE *)openFileMainW_vsdlss(param_2);
    writeNewOffDiagVec_vsdlss
              (param_9,param_10,iVar16,iVar5,__ptr,param_23,param_11,param_12,param_13,param_14,
               pFVar10);
    fclose(pFVar10);
  }
  if (0 < iVar14) {
    formAndWriteUnsymmFiles
              (param_1,param_9,param_10,iVar16,iVar14,__ptr,param_23,param_11,param_12,param_13,
               param_14,param_5,param_6,param_15,param_16,param_17,param_18,param_19,param_20);
  }
  tc_cfree(param_13);
  tc_cfree(param_14);
  tc_cfree(param_11);
  tc_cfree(param_12);
  if (param_1 == 0 && param_15 == 0) {
    pFVar10 = (FILE *)openFileMainW_vsdlss(param_3);
    writeRhsVec_vsdlss(param_9,iVar15,__ptr,param_23,param_25,pFVar10);
    fclose(pFVar10);
  }
  else {
    writeRhsVecsUnsymm(param_1,param_9,iVar15,iVar16,iVar14,__ptr,param_23,param_25,param_7,param_8)
    ;
  }
  tc_cfree(param_25);
  pFVar10 = (FILE *)openFileMainW_vsdlss(param_4);
  sVar11 = fwrite(param_23,1,sVar6,pFVar10);
  if (iVar4 != (int)sVar11) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x5a1,"Write failed: Writing %d but written %d\n",iVar4,
                      sVar11 & 0xffffffff);
  }
  sVar11 = fwrite(param_24,8,sVar6,pFVar10);
  if (iVar4 != (int)sVar11) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x5aa,"Write failed: Writing %d but written %d\n",iVar4,
                      sVar11 & 0xffffffff);
  }
  sVar6 = fwrite(__ptr,4,sVar6,pFVar10);
  if (iVar4 != (int)sVar6) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x5b3,"Write failed: Writing %d but written %d\n",iVar4,
                      sVar6 & 0xffffffff);
  }
  tc_cfree(param_23);
  tc_cfree(param_24);
  fclose(pFVar10);
  if (bVar3) {
    tc_cfree(__ptr_00);
  }
  tc_cfree(__ptr);
  *param_21 = iVar16;
  *param_22 = iVar14;
  return iVar5;
}


