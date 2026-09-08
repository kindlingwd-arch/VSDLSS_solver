/* driver: TOFQBlBTmCCbFyDtB_XSPAIPP addr=0095b230 size=44f */
#include "vsdlss_ref.h"


void TOFQBlBTmCCbFyDtB_XSPAIPP
               (long param_1,long param_2,size_t param_3,size_t param_4,long param_5,long param_6,
               long param_7,long param_8,long param_9,long param_10,FILE *param_11)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  size_t sVar4;
  size_t sVar5;
  void *pvVar6;
  long lVar7;
  size_t __n;
  size_t local_48;
  size_t local_40 [2];
  
  local_48 = param_4;
  local_40[0] = param_3;
  sVar4 = fwrite(local_40,8,1,param_11);
  if (sVar4 != 1) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x285,"Write failed: Writing %ld but written %ld\n",1,sVar4);
  }
  sVar5 = fwrite(&local_48,8,1,param_11);
  sVar4 = local_40[0];
  if (sVar5 != 1) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x28f,"Write failed: Writing %ld but written %ld\n",1,sVar5);
  }
  pvVar6 = tc_malloc(local_40[0] * 8 + 8);
  lVar7 = 1;
  if (0 < param_1) {
    do {
      if ((*(char *)(param_6 + lVar7) == '\0') &&
         (lVar1 = *(long *)(param_5 + lVar7 * 8), lVar1 <= (long)sVar4)) {
        *(undefined8 *)((long)pvVar6 + lVar1 * 8) = *(undefined8 *)(param_10 + lVar7 * 8);
      }
      lVar7 = lVar7 + 1;
    } while (lVar7 != param_1 + 1);
  }
  sVar4 = fwrite((void *)((long)pvVar6 + 8),8,sVar4,param_11);
  if (sVar4 != local_40[0]) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x2a4,"Write failed: Writing %ld but written %ld\n",local_40[0],
                      sVar4);
  }
  tc_cfree(pvVar6);
  sVar5 = local_48;
  pvVar6 = tc_malloc(local_48 * 8 + 8);
  uVar3 = DAT_00abd200;
  if (param_2 < 1) {
    __n = 0;
  }
  else {
    __n = 0;
    lVar7 = 1;
    do {
      lVar1 = *(long *)(param_7 + lVar7 * 8);
      lVar2 = *(long *)(param_8 + lVar7 * 8);
      if ((((*(char *)(param_6 + lVar1) == '\0') && (*(char *)(param_6 + lVar2) == '\0')) &&
          (*(long *)(param_5 + lVar1 * 8) <= (long)sVar4)) &&
         (*(long *)(param_5 + lVar2 * 8) <= (long)sVar4)) {
        __n = __n + 1;
        *(ulong *)((long)pvVar6 + __n * 8) = *(ulong *)(param_9 + lVar7 * 8) ^ uVar3;
      }
      lVar7 = lVar7 + 1;
    } while (lVar7 != param_2 + 1);
  }
  if (__n != sVar5) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",700,"Internal data structure integrity is lost\n");
  }
  sVar4 = fwrite((void *)((long)pvVar6 + 8),8,__n,param_11);
  if (sVar4 != local_48) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x2c4,"Write failed: Writing %ld but written %ld\n",local_48,
                      sVar4);
  }
  tc_cfree(pvVar6);
  pvVar6 = tc_malloc(sVar4 * 8 + 8);
  if (param_2 < 1) {
    sVar5 = 0;
  }
  else {
    sVar5 = 0;
    lVar7 = 1;
    do {
      lVar1 = *(long *)(param_7 + lVar7 * 8);
      lVar2 = *(long *)(param_8 + lVar7 * 8);
      if ((*(char *)(param_6 + lVar1) == '\0') && (*(char *)(param_6 + lVar2) == '\0')) {
        lVar1 = *(long *)(param_5 + lVar1 * 8);
        if ((lVar1 <= (long)local_40[0]) && (*(long *)(param_5 + lVar2 * 8) <= (long)local_40[0])) {
          sVar5 = sVar5 + 1;
          *(long *)((long)pvVar6 + sVar5 * 8) = lVar1;
        }
      }
      lVar7 = lVar7 + 1;
    } while (lVar7 != param_2 + 1);
  }
  if (sVar4 != sVar5) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x2dd,"Internal data structure integrity is lost\n");
  }
  sVar4 = fwrite((void *)((long)pvVar6 + 8),8,sVar4,param_11);
  if (sVar4 != local_48) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x2e3,"Write failed: Writing %ld but written %ld\n",local_48,
                      sVar4);
  }
  sVar5 = 0;
  if (0 < param_2) {
    sVar5 = 0;
    lVar7 = 1;
    do {
      lVar1 = *(long *)(param_7 + lVar7 * 8);
      lVar2 = *(long *)(param_8 + lVar7 * 8);
      if (((*(char *)(param_6 + lVar1) == '\0') && (*(char *)(param_6 + lVar2) == '\0')) &&
         ((lVar2 = *(long *)(param_5 + lVar2 * 8),
          *(long *)(param_5 + lVar1 * 8) <= (long)local_40[0] && (lVar2 <= (long)local_40[0])))) {
        sVar5 = sVar5 + 1;
        *(long *)((long)pvVar6 + sVar5 * 8) = lVar2;
      }
      lVar7 = lVar7 + 1;
    } while (lVar7 != param_2 + 1);
  }
  if (sVar5 == sVar4) {
    sVar4 = fwrite((void *)((long)pvVar6 + 8),8,sVar5,param_11);
    if (sVar4 == local_48) {
      tc_cfree(pvVar6);
      return;
    }
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x2ff,"Write failed: Writing %ld but written %ld\n",local_48,
                      sVar4);
  }
                    /* WARNING: Subroutine does not return */
  CyQyIcOOLOXSPAIPP(9999,"main.c",0x2f9,"Internal data structure integrity is lost\n");
}


