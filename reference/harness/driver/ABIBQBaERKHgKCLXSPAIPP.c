/* driver: ABIBQBaERKHgKCLXSPAIPP addr=00974500 size=155 */
#include "vsdlss_ref.h"


undefined8 ABIBQBaERKHgKCLXSPAIPP(long param_1)

{
  long lVar1;
  long *plVar2;
  long lVar3;
  
  if (param_1 != 0) {
    lVar3 = 1;
    if (0 < *(long *)(param_1 + 0x18)) {
      do {
        plVar2 = (long *)(lVar3 * 8 + *(long *)(param_1 + 0x28));
        if (*plVar2 != 0) {
          _yPFdOBBXSPAIPP();
          plVar2 = (long *)(lVar3 * 8 + *(long *)(param_1 + 0x28));
        }
        lVar3 = lVar3 + 1;
        lVar1 = *(long *)(param_1 + 0x18);
        *plVar2 = 0;
      } while (lVar3 <= lVar1);
    }
    if (*(long *)(param_1 + 0x28) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x28) = 0;
    if (*(long *)(param_1 + 0x30) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x30) = 0;
    if (*(long *)(param_1 + 0x38) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x38) = 0;
    if (*(long *)(param_1 + 0x40) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x40) = 0;
    if (*(long *)(param_1 + 0x48) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x48) = 0;
    if (*(long *)(param_1 + 0x50) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x50) = 0;
    if (*(long *)(param_1 + 0x58) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x58) = 0;
    if (*(long *)(param_1 + 0x60) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x60) = 0;
    if (*(long *)(param_1 + 0x68) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x68) = 0;
    if (*(long *)(param_1 + 0x70) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x70) = 0;
    if (*(long *)(param_1 + 0x78) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x78) = 0;
    _yPFdOBBXSPAIPP(param_1);
  }
  return 0;
}


