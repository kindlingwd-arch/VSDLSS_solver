/* driver: OByAgKPXSPAIPP addr=00989ad0 size=183 */
#include "vsdlss_ref.h"


long OByAgKPXSPAIPP(FILE *param_1,void *param_2)

{
  long lVar1;
  long lVar2;
  size_t sVar3;
  long lVar4;
  undefined8 uVar5;
  long lVar6;
  long local_d0;
  undefined1 local_c8 [8];
  long local_c0;
  
  sVar3 = fread(&local_d0,8,1,param_1);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"indistnodesetsutils.c",0x139,"readIVectorNoNewVec:n",1,sVar3);
  }
  sVar3 = fread(local_c8,8,local_d0 + 1,param_1);
  if (local_d0 + 1U != sVar3) {
    KOBOOLOXSPAIPP(0x10,"indistnodesetsutils.c",0x139,"readIVectorNoNewVec:v",local_d0 + 1U,sVar3);
  }
  lVar6 = 1;
  lVar4 = KBTgKAFPQlLABqBQPXSPAIPP(local_c0);
  if (0 < local_c0) {
    do {
      sVar3 = fread(&local_d0,8,1,param_1);
      if (sVar3 != 1) {
        KOBOOLOXSPAIPP(0x10,"indistnodesetsutils.c",0x13e,"readIVectorNoNewVec:n",1,sVar3);
      }
      sVar3 = fread(param_2,8,local_d0 + 1,param_1);
      if (local_d0 + 1U != sVar3) {
        KOBOOLOXSPAIPP(0x10,"indistnodesetsutils.c",0x13e,"readIVectorNoNewVec:v",local_d0 + 1U,
                       sVar3);
      }
      lVar2 = *(long *)(lVar4 + 8);
      lVar1 = lVar6 * 8;
      lVar6 = lVar6 + 1;
      uVar5 = _LMVYOOyVrLacADBIFPQXSPAIPP(param_2,local_d0);
      *(undefined8 *)(lVar2 + lVar1) = uVar5;
    } while (lVar6 <= local_c0);
  }
  return lVar4;
}


