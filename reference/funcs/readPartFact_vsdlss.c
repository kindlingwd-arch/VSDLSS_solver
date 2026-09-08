/* VSDLSS function (decompiled by Ghidra) */
/* name: readPartFact_vsdlss  addr: 00a089b0  size: 177 */
#include "vsdlss_ref.h"

/* signature: undefined4 * readPartFact_vsdlss(void); */

undefined4 * readPartFact_vsdlss(void)

{
  undefined4 uVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  int iVar5;
  FILE *__stream;
  size_t sVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  undefined8 *puVar9;
  long lVar10;
  int local_40;
  int local_3c [3];
  
  __stream = (FILE *)openFileMainR_vsdlss();
  sVar6 = fread(&local_40,4,1,__stream);
  if ((int)sVar6 != 1) {
    nrerror_vsdlss(0x10,"partfactutils.c",0xd8,"readPartFact:nColsParf",sVar6 & 0xffffffff,1);
  }
  lVar10 = 0;
  puVar7 = (undefined4 *)newPartFact_vsdlss(local_40);
  uVar1 = *puVar7;
  lVar2 = *(long *)(puVar7 + 2);
  lVar3 = *(long *)(puVar7 + 4);
  lVar4 = *(long *)(puVar7 + 6);
  if (0 < local_40) {
    do {
      sVar6 = fread(local_3c,4,1,__stream);
      if ((int)sVar6 != 1) {
        nrerror_vsdlss(0x10,"partfactutils.c",0xe2,"readPartFact:nz",sVar6 & 0xffffffff,1);
      }
      *(int *)(lVar2 + 4 + lVar10 * 4) = local_3c[0];
      if (local_3c[0] == 0) {
        nrerror_vsdlss(1,"partfactutils.c",0xe6);
      }
      puVar8 = (undefined4 *)tagNewIVector_vsdlss(uVar1,(long)local_3c[0],0,0);
      *(undefined4 **)(lVar3 + 8 + lVar10 * 8) = puVar8;
      *puVar8 = 0;
      readIVectorFromFpMain_vsdlss(__stream,local_3c[0],puVar8 + 1);
      puVar9 = (undefined8 *)tagNewDVector_vsdlss(0,uVar1,(long)local_3c[0],0);
      *(undefined8 **)(lVar4 + 8 + lVar10 * 8) = puVar9;
      *puVar9 = 0;
      readDVectorFromFpMain_vsdlss(__stream,local_3c[0],puVar9 + 1);
      iVar5 = (int)lVar10;
      lVar10 = lVar10 + 1;
    } while (iVar5 + 2 <= local_40);
  }
  fclose(__stream);
  return puVar7;
}


