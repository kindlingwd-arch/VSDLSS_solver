/* VSDLSS function (decompiled by Ghidra) */
/* name: snipAndReturnCEdgelistElem_vsdlss  addr: 009e21c0  size: 1f */
#include "vsdlss_ref.h"

/* signature: undefined8 snipAndReturnCEdgelistElem_vsdlss(undefined8 *param_1,long param_2,long param_3,long *param_4); */

undefined8
snipAndReturnCEdgelistElem_vsdlss(undefined8 *param_1,long param_2,long param_3,long *param_4)

{
  if (param_3 != 0) {
    *(undefined8 *)(param_3 + 8) = *(undefined8 *)(param_2 + 8);
    *param_4 = param_2;
    return *(undefined8 *)(param_3 + 8);
  }
  *param_1 = *(undefined8 *)(param_2 + 8);
  *param_4 = param_2;
  return *param_1;
}


