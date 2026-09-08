/* VSDLSS function (decompiled by Ghidra) */
/* name: initStaticVars_vsdlss  addr: 009dce50  size: 17 */
#include "vsdlss_ref.h"

/* signature: void initStaticVars_vsdlss(void); */

void initStaticVars_vsdlss(void)

{
  initStatic_casimalloc_vsdlss();
  initStatic_celutils_vsdlss();
  initStatic_memmanager_vsdlss();
  return;
}


