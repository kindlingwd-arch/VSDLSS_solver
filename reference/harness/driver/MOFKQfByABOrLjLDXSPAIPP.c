/* driver: MOFKQfByABOrLjLDXSPAIPP addr=0098a0e0 size=486 */
#include "vsdlss_ref.h"


void MOFKQfByABOrLjLDXSPAIPP
               (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               undefined8 param_5,undefined8 param_6,long param_7,long param_8,undefined8 param_9,
               long param_10,undefined8 param_11,char *param_12,undefined8 param_13,
               undefined8 param_14,undefined8 param_15,undefined8 param_16,undefined8 param_17,
               undefined8 param_18,undefined8 param_19,undefined8 param_20,undefined8 param_21,
               long param_22,undefined8 param_23,undefined8 param_24,undefined8 param_25,
               long param_26,undefined8 param_27,undefined8 param_28,long param_29,
               undefined8 param_30)

{
  FILE *__stream;
  long lVar1;
  long lVar2;
  
  __stream = (FILE *)DBQdFIBnQOXSPAIPP(8);
  fputc(10,__stream);
  fwrite("******************************************************************************\n",1,0x4f,
         __stream);
  MOFKQrLjLDXSPAIPP("   Header Information\n");
  MOFKQrLjLDXSPAIPP("   ------------------\n");
  MOFKQrLjLDXSPAIPP("\tJobname: %s\n",param_2);
  MOFKQrLjLDXSPAIPP("\tNumber of Nodes(Variables): %ld\n",param_3);
  MOFKQrLjLDXSPAIPP("\tNumber of Offdiagonals: %ld\n",param_4);
  MOFKQrLjLDXSPAIPP("\tMultiple Load Case Flag: %ld\n",param_5);
  MOFKQrLjLDXSPAIPP(param_1,"\tAvailable Memory (MB): %g\n");
  MOFKQrLjLDXSPAIPP("\tOut-of-Core Activation Flag: %ld\n",param_6);
  MOFKQrLjLDXSPAIPP("\tNumber of available directories: %ld\n",param_7);
  MOFKQrLjLDXSPAIPP("\tList of available directories\n");
  MOFKQrLjLDXSPAIPP("\t-----------------------------\n");
  lVar1 = 1;
  if (0 < param_7) {
    do {
      lVar2 = lVar1 + 1;
      MOFKQrLjLDXSPAIPP("\tDirectory %ld: %s\n",lVar1,*(undefined8 *)(param_8 + lVar1 * 8));
      lVar1 = lVar2;
    } while (lVar2 <= param_7);
  }
  MOFKQrLjLDXSPAIPP("\t-----------------------------\n");
  MOFKQrLjLDXSPAIPP("\tProblem Size Reduction Indicator: %ld\n",param_9);
  MOFKQrLjLDXSPAIPP("\tPartial Factor Required Flag: %ld\n",param_10);
  if (param_10 != 0) {
    MOFKQrLjLDXSPAIPP("\t\tSize of the Reduced Matrix: %ld\n",param_11);
    MOFKQrLjLDXSPAIPP("\t\tProduce Reduced System Only Flag: %ld\n",param_15);
    MOFKQrLjLDXSPAIPP("\t\tReduced Linear System Variable List File: %s\n",param_16);
  }
  if (param_12 == (char *)0x0) {
    param_12 = "<none>";
  }
  MOFKQrLjLDXSPAIPP("\tLocal Directory: %s\n",param_12);
  MOFKQrLjLDXSPAIPP("\tSolve in Memory Activation Flag: %ld\n",param_13);
  MOFKQrLjLDXSPAIPP("\tOrder Selector: %ld\n",param_14);
  MOFKQrLjLDXSPAIPP("\tMatrix Will Be Read From Files:\n");
  if (param_29 == 0) {
    MOFKQrLjLDXSPAIPP("\t\tDiagonal Vector:\t%s\n\t\tRow Index Vector:\t%s\n\t\tColumn Index Vector:\t%s\n\t\tOffdiagonals Vector:\t%s\n"
                      ,param_17,param_18,param_19,param_20);
    if (param_22 == 0) {
      MOFKQrLjLDXSPAIPP("\tRHS Will Be Read From File: %s\n",param_21);
    }
    else {
      MOFKQrLjLDXSPAIPP("\tRHS is passed in memory via a vector\n");
    }
    MOFKQrLjLDXSPAIPP("\tResult Will Be Returned in File: %s\n",param_23);
  }
  else {
    MOFKQrLjLDXSPAIPP("\tCaled from HMSLSS. Matrix will be passed in memory via a callback.\n");
  }
  MOFKQrLjLDXSPAIPP("\tPrint Statistics Flag: %ld\n",param_24);
  MOFKQrLjLDXSPAIPP("\tPrint Level: %ld\n",param_25);
  MOFKQrLjLDXSPAIPP("\tCalled From Uwrapper: %ld\n",param_26);
  if (param_26 != 0) {
    MOFKQrLjLDXSPAIPP("\tAt End node list is read from file: %s\n",param_27);
    MOFKQrLjLDXSPAIPP("\tRTOyMMBO File Names are read from file: %s\n",param_28);
  }
  MOFKQrLjLDXSPAIPP("\tCalled From hmslss: %ld\n",param_29);
  MOFKQrLjLDXSPAIPP("\thmslssReorderOnlyF: %ld\n",param_30);
  fwrite("******************************************************************************\n",1,0x4f,
         __stream);
  fputc(10,__stream);
  return;
}


