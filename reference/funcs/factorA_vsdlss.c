/* VSDLSS function (decompiled by Ghidra) */
/* name: factorA_vsdlss  addr: 009ef280  size: 272 */
#include "vsdlss_ref.h"

/* signature: void factorA_vsdlss(long param_1,long param_2,long param_3); */

void factorA_vsdlss(long param_1,long param_2,long param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  long lVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  int iVar9;
  int iVar10;
  
  uVar4 = *(undefined4 *)(param_2 + 4);
  iVar1 = *(int *)(param_1 + 8);
  iVar3 = getMaxIVector_vsdlss(*(undefined8 *)(param_2 + 0x18),uVar4);
  lVar5 = newIVector_vsdlss((long)iVar3,0,0);
  uVar6 = createFactorBlock(param_2);
  uVar4 = getMaxIVector_vsdlss(*(undefined8 *)(param_2 + 0x10),uVar4);
  uVar7 = newFactorBlockHeader_vsdlss(uVar4);
  if (0 < iVar1) {
    iVar3 = 1;
    casiTimes_vsdlss(t_work1_vsdlss);
    uVar8 = createFactorChunk(param_1,param_2,1);
    initFactorChunkFromA(param_1,uVar8,uVar6,lVar5);
    casiTimes_vsdlss(t_work2_vsdlss);
    accumDiffTimes_vsdlss(t_work1_vsdlss,t_work2_vsdlss,t_factorIO_vsdlss);
    removeAChunkFile_vsdlss(param_1,1);
LAB_009ef360:
    iVar3 = iVar3 + 1;
    effectWithinChunk(param_1,param_2,uVar8,uVar7,lVar5);
    casiTimes_vsdlss(t_work1_vsdlss);
    writeFactorChunk_vsdlss(uVar8,param_1,param_2);
    casiTimes_vsdlss(t_work2_vsdlss);
    accumDiffTimes_vsdlss(t_work1_vsdlss,t_work2_vsdlss,t_factorIO_vsdlss);
    deleteFactorChunk_vsdlss(uVar8);
    if (iVar3 <= iVar1) {
      casiTimes_vsdlss(t_work1_vsdlss);
      uVar8 = createFactorChunk(param_1,param_2,iVar3);
      initFactorChunkFromA(param_1,uVar8,uVar6,lVar5);
      casiTimes_vsdlss(t_work2_vsdlss);
      accumDiffTimes_vsdlss(t_work1_vsdlss,t_work2_vsdlss,t_factorIO_vsdlss);
      removeAChunkFile_vsdlss(param_1,iVar3);
      iVar9 = 1;
      do {
        while( true ) {
          iVar10 = iVar3;
          if (iVar3 <= iVar9) {
            iVar10 = iVar9;
          }
          iVar2 = iVar3;
          if (iVar9 <= iVar3) {
            iVar2 = iVar9;
          }
          if (*(char *)(*(long *)(*(long *)(param_3 + 0x10) + (long)iVar10 * 8) + (long)iVar2) ==
              '\0') break;
          iVar10 = iVar9 + 1;
          effectOfFromChunkOnToChunk(param_1,param_2,iVar9,iVar3,uVar8,uVar6,lVar5);
          iVar9 = iVar10;
          if (iVar10 == iVar3) goto LAB_009ef360;
        }
        iVar9 = iVar9 + 1;
      } while (iVar9 != iVar3);
      goto LAB_009ef360;
    }
  }
  deleteFactorBlock_vsdlss(uVar6);
  deleteFactorBlock_vsdlss(uVar7);
  if (lVar5 == 0) {
    return;
  }
  casiFree_vsdlss(lVar5);
  return;
}


