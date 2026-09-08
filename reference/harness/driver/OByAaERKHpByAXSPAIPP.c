/* driver: OByAaERKHpByAXSPAIPP addr=00975190 size=13f */
#include "vsdlss_ref.h"


long OByAaERKHpByAXSPAIPP(void)

{
  size_t sVar1;
  size_t sVar2;
  long local_10;
  
  sVar1 = fread(&local_10,8,1,_JRaERKHBApByAdM);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"chunkmatrixutils.c",0xcc,"readChunkRead:n",1,sVar1);
  }
  if (0 < local_10) {
    sVar1 = local_10 + 1;
    sVar2 = fread(_JRdOLJlLABtB_pByA,8,sVar1,_JRaERKHBApByAdM);
    if (sVar2 != sVar1) {
      KOBOOLOXSPAIPP(0x10,"chunkmatrixutils.c",0xd1,"readChunkRead:_JRdOLJlLABtB_pByA",sVar1,sVar2);
    }
    sVar1 = local_10 + 1;
    sVar2 = fread(_JRrLlLABtB_pByA,8,sVar1,_JRaERKHBApByAdM);
    if (sVar2 != sVar1) {
      KOBOOLOXSPAIPP(0x10,"chunkmatrixutils.c",0xd5,"readChunkRead:_JRrLlLABtB_pByA",sVar1,sVar2);
    }
    sVar1 = local_10 + 1;
    sVar2 = fread(_JRmCCbFyDtB_pByA,8,sVar1,_JRaERKHBApByAdM);
    if (sVar2 != sVar1) {
      KOBOOLOXSPAIPP(0x10,"chunkmatrixutils.c",0xdd,"readChunkRead:_JRmCCbFyDtB_pByA",sVar1,sVar2);
    }
  }
  return local_10;
}


