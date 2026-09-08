/* harness: createAColumnOfA12_1 addr=00a370a0 size=68 */
#include "vsdlss_ref.h"


int createAColumnOfA12_1(int param_1,int param_2,long param_3,int param_4,undefined4 *param_5)

{
  int iVar1;
  undefined4 uVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  
  lVar5 = (long)param_4;
  iVar1 = (param_1 - param_2) + 1;
  uVar2 = *param_5;
  *(int *)(*(long *)(param_5 + 4) + lVar5 * 4) = iVar1;
  lVar3 = *(long *)(param_5 + 6);
  uVar4 = tagNewFVector_uwrapper(0,uVar2,iVar1,0);
  *(undefined8 *)(lVar3 + lVar5 * 8) = uVar4;
  copyVector_uwrapper(param_3 + -8 + (long)param_2 * 8,iVar1,
                      *(undefined8 *)(*(long *)(param_5 + 6) + lVar5 * 8));
  return iVar1;
}


