/* driver: PBQdFIBlyJBXSPAIPP addr=00986dd0 size=4b */
#include "vsdlss_ref.h"


void PBQdFIBlyJBXSPAIPP(long param_1,char *param_2)

{
  if (*(long *)(&DAT_00e3ab50 + param_1 * 0x28) != 3) {
    KOBOOLOXSPAIPP(0x18,"fmanager.c",0x73);
  }
  strcpy(*(char **)(&DAT_00e3ab40 + param_1 * 0x28),param_2);
  return;
}


