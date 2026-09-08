/* driver: Cy_QLOaERKHdFIBcUFPQPXSPAIPP addr=00974ba0 size=48 */
#include "vsdlss_ref.h"


bool Cy_QLOaERKHdFIBcUFPQPXSPAIPP(undefined8 param_1,undefined8 param_2)

{
  FILE *__stream;
  char local_418 [16];
  char local_408 [1032];
  
  DBQdy_QLOaERKHdRIIdKXSPAIPP(param_1,param_2,local_408);
  local_418[0] = 'r';
  local_418[1] = 'b';
  local_418[2] = 0;
  __stream = fopen(local_408,local_418);
  if (__stream != (FILE *)0x0) {
    fclose(__stream);
  }
  return __stream != (FILE *)0x0;
}


