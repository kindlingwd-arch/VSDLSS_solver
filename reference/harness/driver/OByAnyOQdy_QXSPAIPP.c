/* driver: OByAnyOQdy_QXSPAIPP addr=009a2b60 size=17e */
#include "vsdlss_ref.h"


undefined8 * OByAnyOQdy_QXSPAIPP(void)

{
  undefined8 uVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  FILE *__stream;
  size_t sVar5;
  undefined8 *puVar6;
  undefined8 *puVar7;
  long lVar8;
  long local_48;
  long local_40 [2];
  
  __stream = (FILE *)LMBKdFIBkyFKpXSPAIPP();
  sVar5 = fread(&local_48,8,1,__stream);
  if (sVar5 != 1) {
    KOBOOLOXSPAIPP(0x10,"partfactutils.c",0xd8,"readPartFact:nColsParf",sVar5,1);
  }
  lVar8 = 1;
  puVar6 = (undefined8 *)KBTnyOQdy_QXSPAIPP(local_48);
  uVar1 = *puVar6;
  lVar2 = puVar6[2];
  lVar3 = puVar6[3];
  lVar4 = puVar6[4];
  if (0 < local_48) {
    do {
      sVar5 = fread(local_40,8,1,__stream);
      if (sVar5 != 1) {
        KOBOOLOXSPAIPP(0x10,"partfactutils.c",0xe2,"readPartFact:nz",sVar5,1);
      }
      *(long *)(lVar2 + lVar8 * 8) = local_40[0];
      if (local_40[0] == 0) {
        KOBOOLOXSPAIPP(1,"partfactutils.c",0xe6);
      }
      puVar7 = (undefined8 *)QyDlBTgtB_QLOXSPAIPP(uVar1,local_40[0],0,0);
      *(undefined8 **)(lVar3 + lVar8 * 8) = puVar7;
      *puVar7 = 0;
      OByAgtB_QLOdOLJdMkyFKXSPAIPP(__stream,local_40[0],puVar7 + 1);
      puVar7 = (undefined8 *)QyDlBTbtB_QLOXSPAIPP(0,uVar1,local_40[0],0);
      *(undefined8 **)(lVar4 + lVar8 * 8) = puVar7;
      *puVar7 = 0;
      lVar8 = lVar8 + 1;
      OByAbtB_QLOdOLJdMkyFKXSPAIPP(__stream,local_40[0],puVar7 + 1);
    } while (lVar8 <= local_48);
  }
  fclose(__stream);
  return puVar6;
}


