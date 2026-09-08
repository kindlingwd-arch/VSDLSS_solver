/* VSDLSS function (decompiled by Ghidra) */
/* name: renumberElemArrAndWriteChunkFiles_vsdlss  addr: 009e71c0  size: 64 */
#include "vsdlss_ref.h"

/* signature: undefined4 renumberElemArrAndWriteChunkFiles_vsdlss(undefined8 param_1,undefined8 param_2,long param_3,undefined8 param_4,undefined8 param_5); */

undefined4
renumberElemArrAndWriteChunkFiles_vsdlss
          (undefined8 param_1,undefined8 param_2,long param_3,undefined8 param_4,undefined8 param_5)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  
  puVar2 = (undefined4 *)renumberSortAndElimDuplicates();
  uVar1 = *puVar2;
  casiRemove_vsdlss(*(undefined4 *)(param_3 + 4));
  casiRemove_vsdlss(*(undefined4 *)(param_3 + 8));
  casiRemove_vsdlss(*(undefined4 *)(param_3 + 0xc));
  casiRemove_vsdlss(*(undefined4 *)(param_3 + 0x10));
  convertFromNodeToNonZeros_vsdlss(puVar2);
  writeElemArrToChunkFiles(puVar2,param_5);
  deleteElemArr_vsdlss(puVar2);
  return uVar1;
}


