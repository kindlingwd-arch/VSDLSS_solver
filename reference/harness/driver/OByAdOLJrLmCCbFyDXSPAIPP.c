/* driver: OByAdOLJrLmCCbFyDXSPAIPP addr=00987720 size=e9 */
#include "vsdlss_ref.h"


void OByAdOLJrLmCCbFyDXSPAIPP(void)

{
  size_t sVar1;
  size_t __n;
  
  __n = CQOTaERKHqFWBpByA;
  if ((long)CQOTlRJgQBJPjBCQ <= (long)CQOTaERKHqFWBpByA) {
    __n = CQOTlRJgQBJPjBCQ;
  }
  sVar1 = fread((void *)(CQOTdOLJlLABtB_pByA + 8),8,__n,(FILE *)CQOTpByAdM._8_8_);
  if (__n != sVar1) {
    KOBOOLOXSPAIPP(0x10,"fromtoreadwrite.c",0xd2,"readFromToOffDiag:CQOTdOLJlLABtB_pByA",__n,sVar1);
  }
  sVar1 = fread((void *)(CQOTrLlLABtB_pByA + 8),8,__n,(FILE *)CQOTpByAdM._16_8_);
  if (__n != sVar1) {
    KOBOOLOXSPAIPP(0x10,"fromtoreadwrite.c",0xd6,"readFromToOffDiag:CQOTrLlLABtB_pByA",__n,sVar1);
  }
  sVar1 = fread((void *)(CQOTmCCbFyDtB_pByA + 8),8,__n,(FILE *)CQOTpByAdM._24_8_);
  if (__n != sVar1) {
    KOBOOLOXSPAIPP(0x10,"fromtoreadwrite.c",0xde,"readFromToOffDiag:CQOTmCCbFyDtB_pByA",__n,sVar1);
    return;
  }
  return;
}


