/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDSetAhtVecsToNull_vsdlss  addr: 00a28000  size: 31 */
#include "vsdlss_ref.h"

/* signature: undefined4 * MLDSetAhtVecsToNull_vsdlss(undefined4 *param_1); */

undefined4 * MLDSetAhtVecsToNull_vsdlss(undefined4 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0;
  *(undefined8 *)(param_1 + 4) = 0;
  *(undefined8 *)(param_1 + 6) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  *(undefined8 *)(param_1 + 10) = 0;
  return param_1;
}


