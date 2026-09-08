/* driver: TOFQBaERKHuOFQBXSPAIPP addr=00974e70 size=13c */
#include "vsdlss_ref.h"


void TOFQBaERKHuOFQBXSPAIPP(long param_1)

{
  size_t sVar1;
  size_t sVar2;
  long local_10;
  
  local_10 = param_1;
  sVar1 = fwrite(&local_10,8,1,_JRaERKHBAuOFQBdM);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"chunkmatrixutils.c",0x52,"writeChunkWrite:n",1,sVar1);
  }
  if (0 < local_10) {
    sVar1 = local_10 + 1;
    sVar2 = fwrite(_JRdOLJlLABtB_uOFQB,8,sVar1,_JRaERKHBAuOFQBdM);
    if (sVar2 != sVar1) {
      KOBOOLOXSPAIPP(0x10,"chunkmatrixutils.c",0x5b,"writeChunkWrite:_JRdOLJlLABtB_uOFQB",sVar1,
                     sVar2);
    }
    sVar1 = local_10 + 1;
    sVar2 = fwrite(_JRrLlLABtB_uOFQB,8,sVar1,_JRaERKHBAuOFQBdM);
    if (sVar2 != sVar1) {
      KOBOOLOXSPAIPP(0x10,"chunkmatrixutils.c",0x62,"writeChunkWrite:_JRrLlLABtB_uOFQB",sVar1,sVar2)
      ;
    }
    sVar1 = local_10 + 1;
    sVar2 = fwrite(_JRmCCbFyDtB_uOFQB,8,sVar1,_JRaERKHBAuOFQBdM);
    if (sVar2 != sVar1) {
      KOBOOLOXSPAIPP(0x10,"chunkmatrixutils.c",0x6a,"writeChunkWrite:_JRmCCbFyDtB_uOFQB",sVar1,sVar2
                    );
    }
  }
  return;
}


