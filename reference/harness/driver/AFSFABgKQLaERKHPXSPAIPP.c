/* driver: AFSFABgKQLaERKHPXSPAIPP addr=00978ae0 size=1b2 */
#include "vsdlss_ref.h"


undefined8
AFSFABgKQLaERKHPXSPAIPP
          (undefined8 param_1,undefined8 param_2,undefined8 param_3,ulong *param_4,
          undefined8 *param_5,undefined8 param_6,undefined8 param_7,undefined8 *param_8,
          undefined8 *param_9)

{
  ulong uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  long lVar4;
  undefined8 uVar5;
  undefined8 local_50;
  undefined8 local_48;
  undefined8 local_40 [2];
  
  uVar1 = *param_4;
  uVar2 = KBTgtB_QLOXSPAIPP(uVar1,1,0);
  uVar3 = KBTgtB_QLOXSPAIPP(uVar1,1,0);
  lVar4 = KBTacADBtB_QLOXSPAIPP(uVar1,1,0);
  ABzRDnOFKQXSPAIPP("Start divideIntoChunks\n");
  uVar5 = KBTqQy_HXSPAIPP(~uVar1);
  local_50 = 0;
  local_48 = 1;
  local_40[0] = 0;
  for (; param_5 != (undefined8 *)0x0; param_5 = (undefined8 *)param_5[1]) {
    JLAFCFBAbCP(param_1,*param_5,param_4,param_6,param_7,uVar3,&local_50,&local_48,local_40,uVar2,
                uVar5,lVar4);
  }
  ABIBQBqQy_HXSPAIPP(uVar5);
  DBQgKSBOPBkyMgKnIy_BXSPAIPP(uVar3,uVar1);
  MBOJRQBgtB_QLOgKnIy_BXSPAIPP(uVar3,uVar1,uVar2);
  _EB_HlLABrLaERKHtB_(local_48,uVar2,uVar1);
  ABzRDnOFKQXSPAIPP("Chunks obtained\n");
  uVar5 = _OByQBaERKHgKCL(param_2,param_3,param_7,uVar3,uVar2);
  ABzRDnOFKQXSPAIPP("ChunkInfo created\n");
  if (lVar4 != 0) {
    _yPFdOBBXSPAIPP(lVar4);
  }
  ABzRDnOFKQXSPAIPP("End divideIntoChunks\n");
  *param_8 = uVar3;
  *param_9 = uVar2;
  return uVar5;
}


