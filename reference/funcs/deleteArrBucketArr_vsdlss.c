/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteArrBucketArr_vsdlss  addr: 00a1e280  size: 1a */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteArrBucketArr_vsdlss(undefined4 *param_1); */

undefined8 deleteArrBucketArr_vsdlss(undefined4 *param_1)

{
  if (param_1 != (undefined4 *)0x0) {
    deleteTag_vsdlss(*param_1);
    return 0;
  }
  return 0;
}


