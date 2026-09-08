/* VSDLSS function (decompiled by Ghidra) */
/* name: printHeaderToLog_vsdlss  addr: 009f55a0  size: 44b */
#include "vsdlss_ref.h"

/* signature: void printHeaderToLog_vsdlss(undefined8 param_1,undefined8 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,undefined4 param_6,int param_7,undefined8 *param_8,undefined4 param_9,int param_10,undefined4 param_11,char *param_12,undefined4 param_13,undefined4 param_14,undefined4 param_15,undefined8 param_16,undefined8 param_17,undefined8 param_18,undefined8 param_19,undefined8 param_20,undefined8 param_21,long param_22,undefined8 param_23,undefined4 param_24,undefined4 param_25,int param_26,undefined8 param_27,undefined8 param_28,int param_29,undefined4 param_30); */

void printHeaderToLog_vsdlss
               (undefined8 param_1,undefined8 param_2,undefined4 param_3,undefined4 param_4,
               undefined4 param_5,undefined4 param_6,int param_7,undefined8 *param_8,
               undefined4 param_9,int param_10,undefined4 param_11,char *param_12,
               undefined4 param_13,undefined4 param_14,undefined4 param_15,undefined8 param_16,
               undefined8 param_17,undefined8 param_18,undefined8 param_19,undefined8 param_20,
               undefined8 param_21,long param_22,undefined8 param_23,undefined4 param_24,
               undefined4 param_25,int param_26,undefined8 param_27,undefined8 param_28,int param_29
               ,undefined4 param_30)

{
  FILE *__stream;
  int iVar1;
  int iVar2;
  
  __stream = (FILE *)getFilePtr_vsdlss(8);
  fputc(10,__stream);
  fwrite("******************************************************************************\n",1,0x4f,
         __stream);
  printToLog_vsdlss("   Header Information\n");
  printToLog_vsdlss("   ------------------\n");
  printToLog_vsdlss("\tJobname: %s\n",param_2);
  printToLog_vsdlss("\tNumber of Nodes(Variables): %d\n",param_3);
  printToLog_vsdlss("\tNumber of Offdiagonals: %d\n",param_4);
  printToLog_vsdlss("\tMultiple Load Case Flag: %d\n",param_5);
  printToLog_vsdlss(param_1,"\tAvailable Memory (MB): %g\n");
  printToLog_vsdlss("\tOut-of-Core Activation Flag: %d\n",param_6);
  printToLog_vsdlss("\tNumber of available directories: %d\n",param_7);
  printToLog_vsdlss("\tList of available directories\n");
  printToLog_vsdlss("\t-----------------------------\n");
  iVar1 = 1;
  if (0 < param_7) {
    do {
      param_8 = param_8 + 1;
      iVar2 = iVar1 + 1;
      printToLog_vsdlss("\tDirectory %d: %s\n",iVar1,*param_8);
      iVar1 = iVar2;
    } while (iVar2 <= param_7);
  }
  printToLog_vsdlss("\t-----------------------------\n");
  printToLog_vsdlss("\tProblem Size Reduction Indicator: %d\n",param_9);
  printToLog_vsdlss("\tPartial Factor Required Flag: %d\n",param_10);
  if (param_10 != 0) {
    printToLog_vsdlss("\t\tSize of the Reduced Matrix: %d\n",param_11);
    printToLog_vsdlss("\t\tProduce Reduced System Only Flag: %d\n",param_15);
    printToLog_vsdlss("\t\tReduced Linear System Variable List File: %s\n",param_16);
  }
  if (param_12 == (char *)0x0) {
    param_12 = "<none>";
  }
  printToLog_vsdlss("\tLocal Directory: %s\n",param_12);
  printToLog_vsdlss("\tSolve in Memory Activation Flag: %d\n",param_13);
  printToLog_vsdlss("\tOrder Selector: %d\n",param_14);
  printToLog_vsdlss("\tMatrix Will Be Read From Files:\n");
  if (param_29 == 0) {
    printToLog_vsdlss("\t\tDiagonal Vector:\t%s\n\t\tRow Index Vector:\t%s\n\t\tColumn Index Vector:\t%s\n\t\tOffdiagonals Vector:\t%s\n"
                      ,param_17,param_18,param_19,param_20);
    if (param_22 == 0) {
      printToLog_vsdlss("\tRHS Will Be Read From File: %s\n",param_21);
    }
    else {
      printToLog_vsdlss("\tRHS is passed in memory via a vector\n");
    }
    printToLog_vsdlss("\tResult Will Be Returned in File: %s\n",param_23);
  }
  else {
    printToLog_vsdlss("\tCaled from HMSLSS. Matrix will be passed in memory via a callback.\n");
  }
  printToLog_vsdlss("\tPrint Statistics Flag: %d\n",param_24);
  printToLog_vsdlss("\tPrint Level: %d\n",param_25);
  printToLog_vsdlss("\tCalled From Uwrapper: %d\n",param_26);
  if (param_26 != 0) {
    printToLog_vsdlss("\tAt End node list is read from file: %s\n",param_27);
    printToLog_vsdlss("\tuwrapper File Names are read from file: %s\n",param_28);
  }
  printToLog_vsdlss("\tCalled From hmslss: %d\n",param_29);
  printToLog_vsdlss("\thmslssReorderOnlyF: %d\n",param_30);
  fwrite("******************************************************************************\n",1,0x4f,
         __stream);
  fputc(10,__stream);
  return;
}


