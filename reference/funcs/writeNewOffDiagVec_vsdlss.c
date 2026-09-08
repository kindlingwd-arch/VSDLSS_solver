/* VSDLSS function (decompiled by Ghidra) */
/* name: writeNewOffDiagVec_vsdlss  addr: 009d2a50  size: 44d */
#include "vsdlss_ref.h"

/* signature: void writeNewOffDiagVec_vsdlss(int param_1,int param_2,int param_3,int param_4,long param_5,long param_6,long param_7,long param_8,long param_9,long param_10,FILE *param_11); */

void writeNewOffDiagVec_vsdlss
               (int param_1,int param_2,int param_3,int param_4,long param_5,long param_6,
               long param_7,long param_8,long param_9,long param_10,FILE *param_11)

{
  int iVar1;
  ulong uVar2;
  size_t sVar3;
  void *pvVar4;
  long lVar5;
  long lVar6;
  int iVar7;
  long lVar8;
  int local_40;
  int local_3c [3];
  
  local_40 = param_4;
  local_3c[0] = param_3;
  sVar3 = fwrite(local_3c,4,1,param_11);
  if ((int)sVar3 != 1) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x285,"Write failed: Writing %d but written %d\n",1,
                      sVar3 & 0xffffffff);
  }
  sVar3 = fwrite(&local_40,4,1,param_11);
  if ((int)sVar3 != 1) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x28f,"Write failed: Writing %d but written %d\n",1,
                      sVar3 & 0xffffffff);
  }
  pvVar4 = tc_malloc((long)(local_3c[0] + 1) << 3);
  if (0 < param_1) {
    lVar5 = 0;
    do {
      if ((*(char *)(param_6 + 1 + lVar5) == '\0') &&
         (iVar7 = *(int *)(param_5 + 4 + lVar5 * 4), iVar7 <= local_3c[0])) {
        *(undefined8 *)((long)pvVar4 + (long)iVar7 * 8) = *(undefined8 *)(param_10 + 8 + lVar5 * 8);
      }
      lVar5 = lVar5 + 1;
    } while (lVar5 != (ulong)(param_1 - 1) + 1);
  }
  sVar3 = fwrite((void *)((long)pvVar4 + 8),8,(long)local_3c[0],param_11);
  if ((int)sVar3 != local_3c[0]) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x2a4,"Write failed: Writing %d but written %d\n",local_3c[0],
                      sVar3 & 0xffffffff);
  }
  tc_cfree(pvVar4);
  pvVar4 = tc_malloc((long)(local_40 + 1) << 3);
  uVar2 = DAT_00abd200;
  if (param_2 < 1) {
    iVar7 = 0;
  }
  else {
    lVar5 = 0;
    iVar7 = 0;
    do {
      lVar6 = (long)*(int *)(param_7 + 4 + lVar5);
      lVar8 = (long)*(int *)(param_8 + 4 + lVar5);
      if ((((*(char *)(param_6 + lVar6) == '\0') && (*(char *)(param_6 + lVar8) == '\0')) &&
          (*(int *)(param_5 + lVar6 * 4) <= local_3c[0])) &&
         (*(int *)(param_5 + lVar8 * 4) <= local_3c[0])) {
        iVar7 = iVar7 + 1;
        *(ulong *)((long)pvVar4 + (long)iVar7 * 8) = *(ulong *)(param_9 + 8 + lVar5 * 2) ^ uVar2;
      }
      lVar5 = lVar5 + 4;
    } while (lVar5 != (ulong)(param_2 - 1) * 4 + 4);
  }
  if (iVar7 != local_40) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",700,"Internal data structure integrity is lost\n");
  }
  sVar3 = fwrite((void *)((long)pvVar4 + 8),8,(long)iVar7,param_11);
  if ((int)sVar3 != local_40) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x2c4,"Write failed: Writing %d but written %d\n",local_40,
                      sVar3 & 0xffffffff);
  }
  tc_cfree(pvVar4);
  pvVar4 = tc_malloc((long)((int)sVar3 + 1) << 2);
  if (param_2 < 1) {
    iVar7 = 0;
  }
  else {
    lVar5 = 0;
    iVar7 = 0;
    do {
      lVar6 = (long)*(int *)(param_7 + 4 + lVar5);
      lVar8 = (long)*(int *)(param_8 + 4 + lVar5);
      if ((*(char *)(param_6 + lVar6) == '\0') && (*(char *)(param_6 + lVar8) == '\0')) {
        iVar1 = *(int *)(param_5 + lVar6 * 4);
        if ((iVar1 <= local_3c[0]) && (*(int *)(param_5 + lVar8 * 4) <= local_3c[0])) {
          iVar7 = iVar7 + 1;
          *(int *)((long)pvVar4 + (long)iVar7 * 4) = iVar1;
        }
      }
      lVar5 = lVar5 + 4;
    } while (lVar5 != (ulong)(param_2 - 1) * 4 + 4);
  }
  if (iVar7 != local_40) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x2dd,"Internal data structure integrity is lost\n");
  }
  sVar3 = fwrite((void *)((long)pvVar4 + 4),4,(long)iVar7,param_11);
  if ((int)sVar3 != local_40) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x2e3,"Write failed: Writing %d but written %d\n",local_40,
                      sVar3 & 0xffffffff);
  }
  if (param_2 < 1) {
    iVar7 = 0;
  }
  else {
    lVar5 = 0;
    iVar7 = 0;
    do {
      lVar6 = (long)*(int *)(param_7 + 4 + lVar5);
      lVar8 = (long)*(int *)(param_8 + 4 + lVar5);
      if (((*(char *)(param_6 + lVar6) == '\0') && (*(char *)(param_6 + lVar8) == '\0')) &&
         ((iVar1 = *(int *)(param_5 + lVar8 * 4), *(int *)(param_5 + lVar6 * 4) <= local_3c[0] &&
          (iVar1 <= local_3c[0])))) {
        iVar7 = iVar7 + 1;
        *(int *)((long)pvVar4 + (long)iVar7 * 4) = iVar1;
      }
      lVar5 = lVar5 + 4;
    } while (lVar5 != (ulong)(param_2 - 1) * 4 + 4);
  }
  if (iVar7 == (int)sVar3) {
    sVar3 = fwrite((void *)((long)pvVar4 + 4),4,(long)iVar7,param_11);
    if ((int)sVar3 == local_40) {
      tc_cfree(pvVar4);
      return;
    }
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x2ff,"Write failed: Writing %d but written %d\n",local_40,
                      sVar3 & 0xffffffff);
  }
                    /* WARNING: Subroutine does not return */
  fatalError_vsdlss(9999,"main.c",0x2f9,"Internal data structure integrity is lost\n");
}


