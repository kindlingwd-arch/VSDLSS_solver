/* driver: OByAbtB_QLOdlRJnBOJRQBXSPAIPP addr=00961dc0 size=10c */
#include "vsdlss_ref.h"


void OByAbtB_QLOdlRJnBOJRQBXSPAIPP(long param_1,undefined8 param_2,ulong param_3,long param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  FILE *__stream;
  size_t sVar4;
  long lVar5;
  ulong __n;
  long lVar6;
  
  lVar6 = 0;
  lVar3 = KBTtB_QLOXSPAIPP(0,pcYbXZsddXqgxc,0);
  __stream = (FILE *)LMBKdFIBpXSPAIPP(param_2);
  for (; 0 < (long)param_3; param_3 = param_3 - __n) {
    __n = pcYbXZsddXqgxc;
    if (param_3 <= pcYbXZsddXqgxc) {
      __n = param_3;
    }
    sVar4 = fread((void *)(lVar3 + 8),8,__n,__stream);
    if (sVar4 != __n) {
      KOBOOLOXSPAIPP(0x10,"utils.c",0x6fc,"readDVectorFNumPermute:v",__n,sVar4);
    }
    if (0 < (long)__n) {
      lVar5 = 1;
      do {
        lVar1 = lVar5 * 8;
        lVar2 = lVar5 * 8;
        lVar5 = lVar5 + 1;
        *(undefined8 *)(param_4 + *(long *)(param_1 + lVar6 * 8 + lVar1) * 8) =
             *(undefined8 *)(lVar3 + lVar2);
      } while (lVar5 != __n + 1);
    }
    lVar6 = lVar6 + __n;
  }
  if (lVar3 != 0) {
    _yPFdOBBXSPAIPP(lVar3);
  }
  _yPFdaILPBXSPAIPP(param_2);
  return;
}


