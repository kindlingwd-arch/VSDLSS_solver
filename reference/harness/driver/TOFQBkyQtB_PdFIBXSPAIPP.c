/* driver: TOFQBkyQtB_PdFIBXSPAIPP addr=0095c3c0 size=77d */
#include "vsdlss_ref.h"


long TOFQBkyQtB_PdFIBXSPAIPP
               (long param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
               long param_9,long param_10,void *param_11,void *param_12,void *param_13,
               void *param_14,long param_15,long param_16,long param_17,long param_18,long param_19,
               undefined8 param_20,long *param_21,long *param_22,void *param_23,void *param_24,
               void *param_25)

{
  size_t __n;
  double *pdVar1;
  double dVar2;
  long lVar3;
  bool bVar4;
  void *__ptr;
  long lVar5;
  size_t sVar6;
  void *pvVar7;
  FILE *pFVar8;
  void *__ptr_00;
  long lVar9;
  long lVar10;
  bool bVar11;
  long local_90;
  
  __n = param_9 + 1;
  __ptr = tc_malloc(__n * 8);
  FKFQgtB_QLOXSPAIPP(__ptr,param_9,0);
  bVar11 = param_1 != 0;
  bVar4 = 0 < param_15;
  __ptr_00 = (void *)0x0;
  if ((bVar11 || bVar4) && (__ptr_00 = tc_malloc(__n * 8), 0 < param_15)) {
    lVar5 = 1;
    do {
      lVar9 = *(long *)(param_16 + lVar5 * 8);
      if (lVar9 != 0) {
        *(undefined8 *)((long)__ptr_00 + lVar9 * 8) = 1;
      }
      lVar9 = *(long *)(param_17 + lVar5 * 8);
      if (lVar9 != 0) {
        *(undefined8 *)((long)__ptr_00 + lVar9 * 8) = 1;
      }
      lVar9 = *(long *)(param_18 + lVar5 * 8);
      if (lVar9 != 0) {
        *(undefined8 *)((long)__ptr_00 + lVar9 * 8) = 1;
      }
      lVar9 = *(long *)(param_19 + lVar5 * 8);
      if (lVar9 != 0) {
        *(undefined8 *)((long)__ptr_00 + lVar9 * 8) = 1;
      }
      lVar5 = lVar5 + 1;
    } while (lVar5 <= param_15);
  }
  lVar9 = 0;
  lVar5 = 1;
  if (0 < param_10) {
    do {
      while( true ) {
        lVar10 = *(long *)((long)param_11 + lVar5 * 8);
        lVar3 = *(long *)((long)param_12 + lVar5 * 8);
        dVar2 = *(double *)((long)param_13 + lVar5 * 8);
        if (*(char *)((long)param_23 + lVar10) == '\0') break;
        if (*(char *)((long)param_23 + lVar3) == '\0') {
          pdVar1 = (double *)((long)param_25 + lVar3 * 8);
          lVar9 = lVar9 + 1;
          *pdVar1 = *(double *)((long)param_24 + lVar10 * 8) * dVar2 + *pdVar1;
          if (*(char *)((long)param_23 + lVar10) == '\0') break;
        }
LAB_0095c507:
        lVar5 = lVar5 + 1;
        if (lVar5 == param_10 + 1) goto LAB_0095c567;
      }
      if (*(char *)((long)param_23 + lVar3) == '\0') goto LAB_0095c507;
      pdVar1 = (double *)((long)param_25 + lVar10 * 8);
      lVar5 = lVar5 + 1;
      lVar9 = lVar9 + 1;
      *pdVar1 = dVar2 * *(double *)((long)param_24 + lVar3 * 8) + *pdVar1;
    } while (lVar5 != param_10 + 1);
  }
LAB_0095c567:
  sVar6 = 1;
  if (0 < param_9) {
    do {
      if (*(char *)((long)param_23 + sVar6) != '\0') {
        *(undefined8 *)((long)param_25 + sVar6 * 8) = 0;
      }
      sVar6 = sVar6 + 1;
    } while (sVar6 != __n);
  }
  local_90 = 0;
  if ((!bVar11) && (local_90 = param_10 - lVar9, param_15 != 0)) {
    lVar9 = 0;
    local_90 = 0;
    lVar5 = 1;
    if (0 < param_10) {
      do {
        lVar10 = *(long *)((long)param_11 + lVar5 * 8);
        lVar3 = *(long *)((long)param_12 + lVar5 * 8);
        if (((*(char *)((long)param_23 + lVar10) == '\0') &&
            (*(char *)((long)param_23 + lVar3) == '\0')) &&
           (*(long *)((long)__ptr_00 + lVar10 * 8) == 0)) {
          lVar9 = lVar9 + (ulong)(*(long *)((long)__ptr_00 + lVar3 * 8) == 0);
        }
        lVar5 = lVar5 + 1;
        local_90 = lVar9;
      } while (lVar5 != param_10 + 1);
    }
  }
  if (param_9 < 1) {
    lVar9 = 0;
    lVar10 = 0;
    lVar5 = 0;
  }
  else {
    lVar5 = 8;
    lVar10 = 0;
    pvVar7 = param_23;
    do {
      if ((*(char *)((long)pvVar7 + 1) == '\0') &&
         ((!bVar4 || (*(long *)((long)__ptr_00 + lVar5) == 0)))) {
        lVar10 = lVar10 + 1;
        *(long *)((long)__ptr + lVar5) = lVar10;
      }
      pvVar7 = (void *)((long)pvVar7 + 1);
      lVar5 = lVar5 + 8;
    } while (pvVar7 != (void *)((long)param_23 + param_9));
    sVar6 = 1;
    lVar5 = lVar10;
    do {
      if (((*(char *)((long)param_23 + sVar6) == '\0') && (bVar4)) &&
         (*(long *)((long)__ptr_00 + sVar6 * 8) != 0)) {
        lVar5 = lVar5 + 1;
        *(long *)((long)__ptr + sVar6 * 8) = lVar5;
      }
      sVar6 = sVar6 + 1;
    } while (sVar6 != __n);
    lVar9 = lVar5 - lVar10;
  }
  if (bVar11) {
    lVar10 = 0;
    lVar9 = lVar5;
  }
  else {
    pFVar8 = (FILE *)LMBKdFIBkyFKuXSPAIPP(param_2);
    TOFQBlBTmCCbFyDtB_XSPAIPP
              (param_9,param_10,lVar10,local_90,__ptr,param_23,param_11,param_12,param_13,param_14,
               pFVar8);
    fclose(pFVar8);
  }
  if (0 < lVar9) {
    CLOJYKAuOFQBsKPVJJdFIBP
              (param_1,param_9,param_10,lVar10,lVar9,__ptr,param_23,param_11,param_12,param_13,
               param_14,param_5,param_6,param_15,param_16,param_17,param_18,param_19,param_20);
  }
  tc_cfree(param_13);
  tc_cfree(param_14);
  tc_cfree(param_11);
  tc_cfree(param_12);
  if (param_1 == 0 && param_15 == 0) {
    pFVar8 = (FILE *)LMBKdFIBkyFKuXSPAIPP(param_3);
    TOFQBpEPtB_XSPAIPP(param_9,lVar5,__ptr,param_23,param_25,pFVar8);
    fclose(pFVar8);
  }
  else {
    TOFQBpEPtB_PsKPVJJ(param_1,param_9,lVar5,lVar10,lVar9,__ptr,param_23,param_25,param_7,param_8);
  }
  tc_cfree(param_25);
  pFVar8 = (FILE *)LMBKdFIBkyFKuXSPAIPP(param_4);
  sVar6 = fwrite(param_23,1,__n,pFVar8);
  if (__n != sVar6) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x5a1,"Write failed: Writing %ld but written %ld\n",__n,sVar6);
  }
  sVar6 = fwrite(param_24,8,__n,pFVar8);
  if (__n != sVar6) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x5aa,"Write failed: Writing %ld but written %ld\n",__n,sVar6);
  }
  sVar6 = fwrite(__ptr,8,__n,pFVar8);
  if (__n != sVar6) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x5b3,"Write failed: Writing %ld but written %ld\n",__n,sVar6);
  }
  tc_cfree(param_23);
  tc_cfree(param_24);
  fclose(pFVar8);
  if (bVar11 || bVar4) {
    tc_cfree(__ptr_00);
  }
  tc_cfree(__ptr);
  *param_21 = lVar10;
  *param_22 = lVar9;
  return local_90;
}


