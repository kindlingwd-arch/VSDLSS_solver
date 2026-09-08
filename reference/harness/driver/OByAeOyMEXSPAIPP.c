/* driver: OByAeOyMEXSPAIPP addr=00988b90 size=2f6 */
#include "vsdlss_ref.h"


long OByAeOyMEXSPAIPP(void)

{
  long *plVar1;
  long *plVar2;
  FILE *__stream;
  size_t sVar3;
  long lVar4;
  void *__ptr;
  undefined8 uVar5;
  long lVar6;
  long lVar7;
  long local_d8;
  long local_d0;
  undefined1 local_c8 [8];
  long local_c0;
  undefined8 local_b8;
  long local_b0;
  long local_a8;
  long local_a0;
  long local_98;
  
  __stream = (FILE *)LMBKdFIBpXSPAIPP(0xb);
  sVar3 = fread(&local_d0,8,1,__stream);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"graphutils.c",99,"readIVectorNoNewVec:n",1,sVar3);
  }
  sVar3 = fread(local_c8,8,local_d0 + 1,__stream);
  if (local_d0 + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"graphutils.c",99,"readIVectorNoNewVec:v",local_d0 + 1U,sVar3);
  }
  local_d8 = local_d0;
  if ((local_98 == 0) || (local_b0 != 0)) {
    lVar4 = KBTeOyMEXSPAIPP(local_c0);
    *(undefined8 *)(lVar4 + 8) = local_b8;
    if (local_b0 != 0) {
      uVar5 = OByAgtB_QLOXSPAIPP(__stream,&local_d8);
      *(undefined8 *)(lVar4 + 0x10) = uVar5;
    }
  }
  else {
    KOBOOLOXSPAIPP(1,"graphutils.c",0x6e);
    lVar4 = KBTeOyMEXSPAIPP(local_c0);
    *(undefined8 *)(lVar4 + 8) = local_b8;
  }
  if (local_a8 != 0) {
    uVar5 = OByAgtB_QLOXSPAIPP(__stream,&local_d8);
    *(undefined8 *)(lVar4 + 0x18) = uVar5;
  }
  if (local_a0 != 0) {
    __ptr = (void *)KBTgtB_QLOXSPAIPP(local_c0,0,0);
    lVar7 = 1;
    lVar6 = KBTacADBtB_QLOXSPAIPP(local_c0,1,0);
    *(long *)(lVar4 + 0x20) = lVar6;
    if (0 < local_c0) {
      do {
        sVar3 = fread(&local_d0,8,1,__stream);
        if (sVar3 != 1) {
          KOBOOLOXSPAIPP(0x10,"graphutils.c",0x83,"readIVectorNoNewVec:n",1,sVar3);
        }
        sVar3 = fread(__ptr,8,local_d0 + 1,__stream);
        if (local_d0 + 1U != sVar3) {
          KOBOOLOXSPAIPP(0x10,"graphutils.c",0x83,"readIVectorNoNewVec:v",local_d0 + 1U,sVar3);
        }
        uVar5 = _LMVYOOyVrLacADBIFPQXSPAIPP(__ptr,local_d0);
        *(undefined8 *)(lVar6 + lVar7 * 8) = uVar5;
        lVar7 = lVar7 + 1;
      } while (lVar7 <= local_c0);
    }
    if (__ptr != (void *)0x0) {
      _yPFdOBBXSPAIPP(__ptr);
    }
  }
  if (local_98 != 0) {
    uVar5 = OByAgtB_QLOXSPAIPP(__stream,&local_d8);
    *(undefined8 *)(lVar4 + 0x28) = uVar5;
    lVar6 = KBTgntB_QLOXSPAIPP(local_c0,1,0);
    *(long *)(lVar4 + 0x30) = lVar6;
    if ((0 < local_c0) &&
       (*(undefined8 *)(lVar6 + 8) = *(undefined8 *)(lVar4 + 0x28), local_c0 != 1)) {
      lVar7 = 8;
      lVar6 = 2;
      do {
        plVar1 = (long *)(*(long *)(lVar4 + 0x10) + lVar7);
        plVar2 = (long *)(*(long *)(lVar4 + 0x30) + lVar7);
        lVar7 = lVar7 + 8;
        *(long *)(*(long *)(lVar4 + 0x30) + lVar6 * 8) = *plVar2 + *plVar1 * 8;
        lVar6 = lVar6 + 1;
      } while (lVar6 != local_c0 + 1);
    }
  }
  _yPFdaILPBXSPAIPP(0xb);
  return lVar4;
}


