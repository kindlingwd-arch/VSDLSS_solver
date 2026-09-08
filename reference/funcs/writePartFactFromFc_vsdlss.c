/* VSDLSS function (decompiled by Ghidra) */
/* name: writePartFactFromFc_vsdlss  addr: 00a1a7c0  size: 192 */
#include "vsdlss_ref.h"

/* signature: void writePartFactFromFc_vsdlss(int param_1,long param_2); */

void writePartFactFromFc_vsdlss(int param_1,long param_2)

{
  int iVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  FILE *__s;
  size_t sVar7;
  long lVar8;
  int local_40;
  int local_3c [3];
  
  lVar2 = *(long *)(param_2 + 0x30);
  iVar1 = *(int *)(param_2 + 8);
  lVar3 = *(long *)(param_2 + 0x38);
  lVar4 = *(long *)(param_2 + 0x28);
  __s = (FILE *)openFileW_vsdlss(0x1f);
  local_3c[0] = iVar1 - param_1;
  sVar7 = fwrite(local_3c,4,1,__s);
  if ((int)sVar7 != 1) {
    nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xb49,"writePartFactFromFc:lastRow",1,sVar7 & 0xffffffff
                  );
  }
  lVar8 = 0;
  if (0 < local_3c[0]) {
    do {
      local_40 = *(int *)(lVar4 + 4 + lVar8 * 4);
      lVar5 = *(long *)(lVar3 + 8 + lVar8 * 8);
      lVar6 = *(long *)(lVar2 + 8 + lVar8 * 8);
      sVar7 = fwrite(&local_40,4,1,__s);
      if ((int)sVar7 != 1) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xb4f,"writePartFactFromFc:nz",1,sVar7 & 0xffffffff)
        ;
      }
      sVar7 = fwrite((void *)(lVar6 + 4),4,(long)local_40,__s);
      iVar1 = (int)sVar7;
      if ((int)sVar7 != local_40) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xb51,"writePartFactFromFc:indVec",local_40,
                       sVar7 & 0xffffffff);
        iVar1 = local_40;
      }
      sVar7 = fwrite((void *)(lVar5 + 8),8,(long)iVar1,__s);
      if ((int)sVar7 != local_40) {
        nrerror_vsdlss(0x10,"vsdlss1memutils.c",0xb53,"writePartFactFromFc:valVec",local_40,
                       sVar7 & 0xffffffff);
      }
      iVar1 = (int)lVar8;
      lVar8 = lVar8 + 1;
    } while (iVar1 + 2 <= local_3c[0]);
  }
  casiFClose_vsdlss(0x1f);
  return;
}


