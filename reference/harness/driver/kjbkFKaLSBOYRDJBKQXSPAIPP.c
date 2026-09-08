/* driver: kjbkFKaLSBOYRDJBKQXSPAIPP addr=0099b490 size=106 */
#include "vsdlss_ref.h"


void kjbkFKaLSBOYRDJBKQXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  char *pcVar1;
  long *plVar2;
  char cVar3;
  long lVar4;
  long lVar5;
  long *plVar6;
  
  lVar5 = param_1;
  do {
    lVar4 = *(long *)(param_2 + lVar5 * 8);
    if (*(char *)(param_3 + lVar4) == '\x02') {
      lVar5 = *(long *)(param_2 + param_1 * 8);
      pcVar1 = (char *)(param_3 + lVar5);
      cVar3 = *pcVar1;
      while (cVar3 != '\x02') {
        *pcVar1 = '\x02';
        lVar5 = *(long *)(param_2 + *(long *)(param_2 + lVar5 * 8) * 8);
        pcVar1 = (char *)(param_3 + lVar5);
        cVar3 = *pcVar1;
      }
      return;
    }
    lVar5 = *(long *)(param_2 + lVar4 * 8);
  } while (lVar5 != 0);
  if (*(long *)(param_4 + lVar4 * 8) != 0) {
    KOBOOLOXSPAIPP(1,"mldorder.c",0xc70);
  }
  do {
    lVar5 = *(long *)(param_2 + param_1 * 8);
    plVar2 = (long *)(param_4 + lVar5 * 8);
    plVar6 = (long *)(lVar5 * 8 + param_2);
    if (*plVar2 != *plVar6) {
      KOBOOLOXSPAIPP(1,"mldorder.c",0xc81);
    }
    *(long *)(param_4 + param_1 * 8) = lVar5;
    *plVar2 = param_1;
    *(undefined1 *)(param_3 + lVar5) = 2;
    param_1 = *plVar6;
  } while (param_1 != 0);
  return;
}


