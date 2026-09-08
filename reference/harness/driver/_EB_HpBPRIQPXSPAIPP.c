/* driver: _EB_HpBPRIQPXSPAIPP addr=00978570 size=16f */
#include "vsdlss_ref.h"


void _EB_HpBPRIQPXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  FILE *__s;
  char *pcVar1;
  double dVar2;
  double dVar3;
  time_t local_30 [2];
  
  dVar2 = (double)QTLlLOJXSPAIPP(param_3,param_1);
  __s = (FILE *)LMBKdFIBYXSPAIPP(0xe);
  PRztB_QLOPXSPAIPP(param_2,param_3,param_1,param_3);
  dVar3 = (double)QTLlLOJXSPAIPP(param_3,param_1);
  fwrite("******************************************************************************\n",1,0x4f,
         __s);
  fwrite("(C) Copyright 1992-2011 Computational Applications and System Integration Inc.\n    All rights Reserved.\n"
         ,1,0x68,__s);
  fwrite("******************************************************************************\n",1,0x4f,
         __s);
  fputc(10,__s);
  local_30[0] = time((time_t *)0x0);
  pcVar1 = ctime(local_30);
  fprintf(__s,"Job:%s\tat:%s\n",GLzlyJBXSPAIPP,pcVar1);
  fputc(10,__s);
  if (dVar2 <= DAT_00ab71f0) {
    fwrite("\t*** Dense Solve Result is Zero ***\n",1,0x24,__s);
    fprintf(__s,"\tTwo Norm of dense solve result: %g\n",dVar2);
    fprintf(__s,"\tTwo Norm of error vector: %g\n",dVar3);
  }
  else {
    fprintf(__s,"\tRelative Error between dense and sparse results: %g\n",dVar3 / dVar2);
  }
  _yPFdaILPBXSPAIPP(0xe);
  return;
}


