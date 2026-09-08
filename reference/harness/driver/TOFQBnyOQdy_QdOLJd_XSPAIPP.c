/* driver: TOFQBnyOQdy_QdOLJd_XSPAIPP addr=0096e160 size=18f */
#include "vsdlss_ref.h"


void TOFQBnyOQdy_QdOLJd_XSPAIPP(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  FILE *__s;
  size_t sVar6;
  long lVar7;
  size_t local_48;
  long local_40 [2];
  
  lVar1 = *(long *)(param_2 + 0x50);
  lVar7 = *(long *)(param_2 + 0x10);
  lVar2 = *(long *)(param_2 + 0x58);
  lVar3 = *(long *)(param_2 + 0x48);
  __s = (FILE *)LMBKdFIBuXSPAIPP(0x1f);
  local_40[0] = lVar7 - param_1;
  sVar6 = fwrite(local_40,8,1,__s);
  if (sVar6 != 1) {
    KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xb49,"writePartFactFromFc:lastRow",1,sVar6);
  }
  lVar7 = 1;
  if (0 < local_40[0]) {
    do {
      local_48 = *(size_t *)(lVar3 + lVar7 * 8);
      lVar4 = *(long *)(lVar2 + lVar7 * 8);
      lVar5 = *(long *)(lVar1 + lVar7 * 8);
      sVar6 = fwrite(&local_48,8,1,__s);
      if (sVar6 != 1) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xb4f,"writePartFactFromFc:nz",1,sVar6);
      }
      sVar6 = fwrite((void *)(lVar5 + 8),8,local_48,__s);
      if (sVar6 != local_48) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xb51,"writePartFactFromFc:indVec",local_48,sVar6);
        sVar6 = local_48;
      }
      sVar6 = fwrite((void *)(lVar4 + 8),8,sVar6,__s);
      if (sVar6 != local_48) {
        KOBOOLOXSPAIPP(0x10,"vsdlss1memutils.c",0xb53,"writePartFactFromFc:valVec",local_48,sVar6);
      }
      lVar7 = lVar7 + 1;
    } while (lVar7 <= local_40[0]);
  }
  _yPFdaILPBXSPAIPP(0x1f);
  return;
}


