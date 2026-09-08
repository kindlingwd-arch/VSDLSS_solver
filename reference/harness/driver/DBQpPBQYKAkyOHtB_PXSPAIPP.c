/* driver: DBQpPBQYKAkyOHtB_PXSPAIPP addr=0096dcd0 size=14b */
#include "vsdlss_ref.h"


void DBQpPBQYKAkyOHtB_PXSPAIPP(long param_1,long param_2,undefined8 *param_3,undefined8 *param_4)

{
  char *pcVar1;
  undefined8 *puVar2;
  undefined1 *puVar3;
  long lVar4;
  long lVar5;
  long local_58;
  long local_50;
  undefined8 local_48;
  undefined8 local_40 [2];
  
  puVar2 = (undefined8 *)KBTgtB_QLOXSPAIPP(param_1,0,0);
  *puVar2 = 0;
  OByAgtB_QLOdlRJXSPAIPP(0x19,param_1,puVar2);
  DBQkFKYKAkyUgtB_QLOXSPAIPP(puVar2,param_1,&local_58,&local_50,&local_48,local_40);
  if (local_58 < 1) {
    KOBOOLOXSPAIPP(0x2d,"vsdlss1memutils.c",0xa9a,local_58,local_48,1,param_2);
  }
  if (param_2 < local_50) {
    KOBOOLOXSPAIPP(0x2d,"vsdlss1memutils.c",0xaa2,local_50,local_40[0],1,param_2);
  }
  puVar3 = (undefined1 *)KBTatB_QLOXSPAIPP(param_2,1,0);
  *puVar3 = 0;
  lVar4 = 1;
  if (0 < param_1) {
    do {
      while( true ) {
        pcVar1 = puVar3 + puVar2[lVar4];
        if (*pcVar1 == '\0') break;
        lVar5 = lVar4 + 1;
        KOBOOLOXSPAIPP(0x2e,"vsdlss1memutils.c",0xab2,puVar2[lVar4],lVar4);
        *pcVar1 = '\x01';
        lVar4 = lVar5;
        if (param_1 < lVar5) goto LAB_0096ddb2;
      }
      lVar4 = lVar4 + 1;
      *pcVar1 = '\x01';
    } while (lVar4 <= param_1);
  }
LAB_0096ddb2:
  *param_3 = puVar2;
  *param_4 = puVar3;
  return;
}


