/* driver: TOFQBlLAyItLIQyDBPXSPAIPP addr=0095af50 size=2d9 */
#include "vsdlss_ref.h"


void TOFQBlLAyItLIQyDBPXSPAIPP(char *param_1,undefined8 param_2,long param_3,long param_4)

{
  size_t __size;
  long lVar1;
  long lVar2;
  uint uVar3;
  uint uVar4;
  size_t sVar5;
  uint *__dest;
  void *__ptr;
  void *__ptr_00;
  void *__ptr_01;
  FILE *pFVar6;
  uint *puVar7;
  uint *puVar8;
  undefined4 *puVar9;
  long lVar10;
  
  __size = param_3 + 1;
  sVar5 = strlen(param_1);
  __dest = tc_malloc(sVar5 + 5);
  __ptr = tc_malloc(__size);
  __ptr_00 = tc_malloc(__size * 8);
  __ptr_01 = tc_malloc(__size * 8);
  pFVar6 = (FILE *)LMBKdFIBkyFKpXSPAIPP(param_2);
  sVar5 = fread(__ptr,1,__size,pFVar6);
  if (__size != sVar5) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x23c,"Read failed: Reading %ld but read %ld\n",__size,sVar5);
  }
  sVar5 = fread(__ptr_00,8,__size,pFVar6);
  if (__size != sVar5) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x245,"Read failed: Reading %ld but read %ld\n",__size,sVar5);
  }
  sVar5 = fread(__ptr_01,8,__size,pFVar6);
  if (__size != sVar5) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x24e,"Read failed: Reading %ld but read %ld\n",__size,sVar5);
  }
  fclose(pFVar6);
  if (param_3 < 1) {
    strcpy((char *)__dest,param_1);
    puVar8 = __dest;
    do {
      puVar7 = puVar8;
      uVar3 = *puVar7 + 0xfefefeff & ~*puVar7;
      uVar4 = uVar3 & 0x80808080;
      puVar8 = puVar7 + 1;
    } while (uVar4 == 0);
    if ((uVar3 & 0x8080) == 0) {
      puVar8 = (uint *)((long)puVar7 + 6);
      uVar4 = uVar4 >> 0x10;
    }
    puVar9 = (undefined4 *)((long)puVar8 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
    *puVar9 = 0x74756f2e;
    *(undefined1 *)(puVar9 + 1) = 0;
    pFVar6 = (FILE *)LMBKdFIBkyFKuXSPAIPP(__dest);
    fprintf(pFVar6,"J       %s\n",param_1);
    fprintf(pFVar6,"%ld\n",param_3);
  }
  else {
    sVar5 = 1;
    do {
      if (*(char *)((long)__ptr + sVar5) == '\0') {
        *(undefined8 *)((long)__ptr_00 + sVar5 * 8) =
             *(undefined8 *)(param_4 + *(long *)((long)__ptr_01 + sVar5 * 8) * 8);
      }
      sVar5 = sVar5 + 1;
    } while (sVar5 != __size);
    strcpy((char *)__dest,param_1);
    puVar8 = __dest;
    do {
      puVar7 = puVar8;
      uVar3 = *puVar7 + 0xfefefeff & ~*puVar7;
      uVar4 = uVar3 & 0x80808080;
      puVar8 = puVar7 + 1;
    } while (uVar4 == 0);
    lVar10 = 1;
    if ((uVar3 & 0x8080) == 0) {
      puVar8 = (uint *)((long)puVar7 + 6);
      uVar4 = uVar4 >> 0x10;
    }
    puVar9 = (undefined4 *)((long)puVar8 + (-3 - (ulong)CARRY1((byte)uVar4,(byte)uVar4)));
    *puVar9 = 0x74756f2e;
    *(undefined1 *)(puVar9 + 1) = 0;
    pFVar6 = (FILE *)LMBKdFIBkyFKuXSPAIPP(__dest);
    fprintf(pFVar6,"J       %s\n",param_1);
    fprintf(pFVar6,"%ld\n",param_3);
    do {
      lVar2 = lVar10 * 8;
      lVar1 = lVar10 + -1;
      lVar10 = lVar10 + 1;
      fprintf(pFVar6,"%ld       %18.15g\n",*(undefined8 *)((long)__ptr_00 + lVar2),lVar1);
    } while (lVar10 <= param_3);
  }
  fclose(pFVar6);
  tc_cfree(__dest);
  tc_cfree(__ptr);
  tc_cfree(__ptr_00);
  tc_cfree(__ptr_01);
  return;
}


