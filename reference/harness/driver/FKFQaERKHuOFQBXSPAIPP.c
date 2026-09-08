/* driver: FKFQaERKHuOFQBXSPAIPP addr=00974d90 size=d9 */
#include "vsdlss_ref.h"


void FKFQaERKHuOFQBXSPAIPP(undefined8 param_1)

{
  undefined8 *puVar1;
  size_t sVar2;
  
  _JRaERKHqFWBuOFQB = DAT_00e74ee0;
  _JRdOLJlLABtB_uOFQB = (undefined8 *)KBTgtB_QLOXSPAIPP(DAT_00e74ee0,0,0);
  _JRrLlLABtB_uOFQB = (undefined8 *)KBTgtB_QLOXSPAIPP(_JRaERKHqFWBuOFQB,0,0);
  puVar1 = (undefined8 *)KBTdtB_QLOXSPAIPP(0,_JRaERKHqFWBuOFQB,0);
  _JRmCCbFyDtB_uOFQB = puVar1;
  *_JRdOLJlLABtB_uOFQB = 0;
  *_JRrLlLABtB_uOFQB = 0;
  *puVar1 = 0;
  _JRaERKHBAuOFQBdlRJ = param_1;
  _JRaERKHBAuOFQBdM = (FILE *)LMBKdFIBuXSPAIPP(param_1);
  sVar2 = fwrite(&_JRaERKHqFWBuOFQB,8,1,_JRaERKHBAuOFQBdM);
  if (sVar2 != 1) {
    KOBOOLOXSPAIPP(0x10,"chunkmatrixutils.c",0x3e,"initChunkWrite:_JRaERKHqFWBuOFQB",1,sVar2);
  }
  _JRaROOBKQcKQOVuOFQB = 1;
  return;
}


