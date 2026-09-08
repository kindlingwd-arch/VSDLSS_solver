/* VSDLSS function (decompiled by Ghidra) */
/* name: printNetMallocCalls_vsdlss  addr: 009e76c0  size: 82 */
#include "vsdlss_ref.h"

/* signature: void printNetMallocCalls_vsdlss(void); */

void printNetMallocCalls_vsdlss(void)

{
  puts("=================================================");
  printf("Net malloc calls %d\n",0);
  printf("malloc calls %d\n",0);
  printf("free calls %d\n",0);
  printf("Maximum difference %d\n",0);
  printf("allocSizeThresh %d\n",(ulong)DAT_00ead8c0);
  printf("num_small_allocs %d\n",0);
  printf("sizeOfSmallAllocs %ld\n",0);
  puts("=================================================");
  return;
}


