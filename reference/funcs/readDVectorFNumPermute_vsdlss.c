/* VSDLSS function (decompiled by Ghidra) */
/* name: readDVectorFNumPermute_vsdlss  addr: 009d9580  size: 115 */
#include "vsdlss_ref.h"

/* signature: void readDVectorFNumPermute_vsdlss(long param_1,undefined4 param_2,int param_3,long param_4); */

void readDVectorFNumPermute_vsdlss(long param_1,undefined4 param_2,int param_3,long param_4)

{
  long lVar1;
  int *piVar2;
  long lVar3;
  FILE *__stream;
  ulong __n;
  size_t sVar4;
  long lVar5;
  int iVar6;
  int iVar7;
  
  iVar6 = 0;
  lVar3 = newVector_vsdlss(0,READ_BUFF_SIZE,0);
  __stream = (FILE *)openFileR_vsdlss(param_2);
  for (; 0 < param_3; param_3 = param_3 - iVar7) {
    __n = READ_BUFF_SIZE;
    if ((ulong)(long)param_3 <= READ_BUFF_SIZE) {
      __n = (long)param_3;
    }
    sVar4 = fread((void *)(lVar3 + 8),8,__n,__stream);
    iVar7 = (int)__n;
    if ((int)sVar4 != iVar7) {
      nrerror_vsdlss(0x10,"utils.c",0x6fc,"readDVectorFNumPermute:v",__n & 0xffffffff,
                     sVar4 & 0xffffffff);
    }
    if (0 < iVar7) {
      lVar5 = 0;
      do {
        piVar2 = (int *)(param_1 + (long)iVar6 * 4 + 4 + lVar5);
        lVar1 = lVar5 * 2;
        lVar5 = lVar5 + 4;
        *(undefined8 *)(param_4 + (long)*piVar2 * 8) = *(undefined8 *)(lVar3 + 8 + lVar1);
      } while (lVar5 != (ulong)(iVar7 - 1) * 4 + 4);
    }
    iVar6 = iVar6 + iVar7;
  }
  if (lVar3 != 0) {
    casiFree_vsdlss(lVar3);
  }
  casiFClose_vsdlss(param_2);
  return;
}


