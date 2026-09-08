/* VSDLSS function (decompiled by Ghidra) */
/* name: writeNodalVoltages_vsdlss  addr: 009d2750  size: 2f1 */
#include "vsdlss_ref.h"

/* signature: void writeNodalVoltages_vsdlss(char *param_1,undefined8 param_2,uint param_3,long param_4); */

void writeNodalVoltages_vsdlss(char *param_1,undefined8 param_2,uint param_3,long param_4)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  size_t sVar4;
  uint *__dest;
  void *__ptr;
  void *__ptr_00;
  void *__ptr_01;
  FILE *pFVar5;
  long lVar6;
  uint *puVar7;
  uint *puVar8;
  undefined4 *puVar9;
  size_t __size;
  ulong uVar10;
  
  sVar4 = strlen(param_1);
  __dest = tc_malloc(sVar4 + 5);
  iVar1 = param_3 + 1;
  __size = (size_t)iVar1;
  __ptr = tc_malloc(__size);
  __ptr_00 = tc_malloc(__size * 8);
  __ptr_01 = tc_malloc(__size * 4);
  pFVar5 = (FILE *)openFileMainR_vsdlss(param_2);
  sVar4 = fread(__ptr,1,__size,pFVar5);
  if (iVar1 != (int)sVar4) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x23c,"Read failed: Reading %d but read %d\n",iVar1,
                      sVar4 & 0xffffffff);
  }
  sVar4 = fread(__ptr_00,8,__size,pFVar5);
  if (iVar1 != (int)sVar4) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x245,"Read failed: Reading %d but read %d\n",iVar1,
                      sVar4 & 0xffffffff);
  }
  sVar4 = fread(__ptr_01,4,__size,pFVar5);
  if (iVar1 != (int)sVar4) {
                    /* WARNING: Subroutine does not return */
    fatalError_vsdlss(9999,"main.c",0x24e,"Read failed: Reading %d but read %d\n",iVar1,
                      sVar4 & 0xffffffff);
  }
  fclose(pFVar5);
  if ((int)param_3 < 1) {
    strcpy((char *)__dest,param_1);
    puVar8 = __dest;
    do {
      puVar7 = puVar8;
      uVar2 = *puVar7 + 0xfefefeff & ~*puVar7;
      uVar3 = uVar2 & 0x80808080;
      puVar8 = puVar7 + 1;
    } while (uVar3 == 0);
    if ((uVar2 & 0x8080) == 0) {
      puVar8 = (uint *)((long)puVar7 + 6);
      uVar3 = uVar3 >> 0x10;
    }
    puVar9 = (undefined4 *)((long)puVar8 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
    *puVar9 = 0x74756f2e;
    *(undefined1 *)(puVar9 + 1) = 0;
    pFVar5 = (FILE *)openFileMainW_vsdlss(__dest);
    fprintf(pFVar5,"J       %s\n",param_1);
    fprintf(pFVar5,"%d\n",(ulong)param_3);
  }
  else {
    lVar6 = 0;
    do {
      if (*(char *)((long)__ptr + lVar6 + 1) == '\0') {
        *(undefined8 *)((long)__ptr_00 + lVar6 * 8 + 8) =
             *(undefined8 *)(param_4 + (long)*(int *)((long)__ptr_01 + lVar6 * 4 + 4) * 8);
      }
      lVar6 = lVar6 + 1;
    } while (lVar6 != (ulong)(param_3 - 1) + 1);
    strcpy((char *)__dest,param_1);
    puVar8 = __dest;
    do {
      puVar7 = puVar8;
      uVar2 = *puVar7 + 0xfefefeff & ~*puVar7;
      uVar3 = uVar2 & 0x80808080;
      puVar8 = puVar7 + 1;
    } while (uVar3 == 0);
    if ((uVar2 & 0x8080) == 0) {
      puVar8 = (uint *)((long)puVar7 + 6);
      uVar3 = uVar3 >> 0x10;
    }
    puVar9 = (undefined4 *)((long)puVar8 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
    *puVar9 = 0x74756f2e;
    *(undefined1 *)(puVar9 + 1) = 0;
    pFVar5 = (FILE *)openFileMainW_vsdlss(__dest);
    fprintf(pFVar5,"J       %s\n",param_1);
    fprintf(pFVar5,"%d\n",(ulong)param_3);
    uVar10 = 0;
    do {
      fprintf(pFVar5,"%d       %18.15g\n",*(undefined8 *)((long)__ptr_00 + uVar10 * 8 + 8),
              uVar10 & 0xffffffff);
      iVar1 = (int)uVar10;
      uVar10 = uVar10 + 1;
    } while (iVar1 + 2 <= (int)param_3);
  }
  fclose(pFVar5);
  tc_cfree(__dest);
  tc_cfree(__ptr);
  tc_cfree(__ptr_00);
  tc_cfree(__ptr_01);
  return;
}


