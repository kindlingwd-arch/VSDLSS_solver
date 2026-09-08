/* driver: FKPBOQhLzgKQLhOQXSPAIPP addr=0098afd0 size=b5 */
#include "vsdlss_ref.h"


void FKPBOQhLzgKQLhOQXSPAIPP(void)

{
  long lVar1;
  long *plVar2;
  long lVar3;
  
  lVar1 = CFKAhLzgKhOQpBQROK();
  if (0 < lVar1) {
    KOBOOLOXSPAIPP(1,"jobreloadmanager.c",0xc6);
  }
  plVar2 = &DAT_00e795c0;
  lVar1 = 1;
  do {
    if (*plVar2 == 0) {
      lVar3 = lVar1 * 0x4a0;
      *(undefined8 *)(&DAT_00e79120 + lVar3) = 1;
      strcpy((char *)(lVar3 + 0xe79128),GLzlyJBXSPAIPP);
      *(undefined8 *)(&DAT_00e79528 + lVar3) = 1;
      *(undefined8 *)(&DAT_00e79530 + lVar3) = 0;
      FKFQnCYKAd_cKQOFBPZVnLPhOQ(lVar1);
      return;
    }
    lVar1 = lVar1 + 1;
    plVar2 = plVar2 + 0x94;
  } while (lVar1 != 0x41);
  KOBOOLOXSPAIPP(0x2f,"jobreloadmanager.c",0xd8,"MAX_NUMBER_JOBS");
  return;
}


