/* driver: TOFQBpEPtB_XSPAIPP addr=0095b680 size=da */
#include "vsdlss_ref.h"


void TOFQBpEPtB_XSPAIPP(long param_1,size_t param_2,long param_3,long param_4,long param_5,
                       FILE *param_6)

{
  long lVar1;
  void *__ptr;
  size_t sVar2;
  long lVar3;
  
  __ptr = tc_malloc(param_2 * 8 + 8);
  lVar3 = 1;
  if (0 < param_1) {
    do {
      if (*(char *)(param_4 + lVar3) == '\0') {
        lVar1 = *(long *)(param_3 + lVar3 * 8);
        if ((long)param_2 < lVar1) {
                    /* WARNING: Subroutine does not return */
          CyQyIcOOLOXSPAIPP(9999,"main.c",0x31a,"Internal data structure integrity is lost\n");
        }
        *(undefined8 *)((long)__ptr + lVar1 * 8) = *(undefined8 *)(param_5 + lVar3 * 8);
      }
      lVar3 = lVar3 + 1;
    } while (lVar3 <= param_1);
  }
  sVar2 = fwrite((void *)((long)__ptr + 8),8,param_2,param_6);
  if (sVar2 != param_2) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x324,"Write failed: Writing %ld but written %ld\n",param_2,
                      sVar2);
  }
  tc_cfree(__ptr);
  return;
}


