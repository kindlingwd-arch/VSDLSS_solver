/* driver: CFIBcUFPQPkyFKXSPAIPP addr=0095abe0 size=27 */
#include "vsdlss_ref.h"


bool CFIBcUFPQPkyFKXSPAIPP(char *param_1)

{
  FILE *__stream;
  
  __stream = fopen(param_1,"rb");
  if (__stream != (FILE *)0x0) {
    fclose(__stream);
  }
  return __stream != (FILE *)0x0;
}


