/* driver: PBQnNFrLlRIInNFXSPAIPP addr=009a7200 size=30 */
#include "vsdlss_ref.h"


undefined8 PBQnNFrLlRIInNFXSPAIPP(long param_1)

{
  if (param_1 != 0) {
    *(undefined8 *)(param_1 + 8) = 0;
    *(undefined8 *)(param_1 + 0x10) = 0;
    *(undefined8 *)(param_1 + 0x18) = 0;
    *(undefined8 *)(param_1 + 0x20) = 0;
    *(undefined8 *)(param_1 + 0x28) = 0;
  }
  return 0;
}


