/* driver: QLMXSPAIPP addr=009b2860 size=12 */
#include "vsdlss_ref.h"


undefined8 QLMXSPAIPP(undefined8 *param_1)

{
  if ((undefined8 *)param_1[1] != (undefined8 *)param_1[2]) {
    return *(undefined8 *)param_1[1];
  }
  return *param_1;
}


