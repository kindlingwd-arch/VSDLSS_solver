/* driver: Cy_QLOYmKBaERKHXSPAIPP addr=0096cee0 size=5 */
#include "vsdlss_ref.h"


void Cy_QLOYmKBaERKHXSPAIPP(long param_1,long param_2,long param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  
  lVar5 = 1;
  lVar4 = 1;
  lVar1 = *(long *)(param_3 + 0x40);
  lVar3 = *(long *)(param_3 + 0x28) - param_2;
  if (param_1 == 0) {
    lVar3 = *(long *)(param_3 + 0x28);
  }
  if (0 < lVar3) {
    do {
      lVar2 = *(long *)(lVar1 + lVar4 * 8);
      switch(lVar2) {
      default:
        lVar8 = lVar5;
        for (lVar7 = lVar2; 0 < lVar7; lVar7 = lVar7 - lVar6) {
          lVar6 = 6;
          if (lVar7 < 7) {
            lVar6 = lVar7;
          }
          switch(lVar6) {
          default:
            PRz1pLTaLKQOFzdOLJpBPQXSPAIPP();
            break;
          case 2:
            PRz2pLTaLKQOFzdOLJpBPQXSPAIPP();
            break;
          case 3:
            PRz3pLTaLKQOFzdOLJpBPQXSPAIPP();
            break;
          case 4:
            PRz4pLTaLKQOFzdOLJpBPQ();
            break;
          case 5:
            PRz5pLTaLKQOFzdOLJpBPQ();
            break;
          case 6:
            PRz6pLTaLKQOFzdOLJpBPQ(lVar8,param_3);
          }
          lVar8 = lVar8 + lVar6;
        }
        break;
      case 1:
        PRz1pLTaLKQOFzdOLJpBPQXSPAIPP(lVar5,param_3);
        break;
      case 2:
        PRz2pLTaLKQOFzdOLJpBPQXSPAIPP(lVar5,param_3);
        break;
      case 3:
        PRz3pLTaLKQOFzdOLJpBPQXSPAIPP(lVar5,param_3);
        break;
      case 4:
        PRz4pLTaLKQOFzdOLJpBPQ(lVar5,param_3);
        break;
      case 5:
        PRz5pLTaLKQOFzdOLJpBPQ(lVar5,param_3);
        break;
      case 6:
        PRz6pLTaLKQOFzdOLJpBPQ(lVar5,param_3);
      }
      lVar5 = lVar5 + lVar2;
      lVar4 = lVar4 + 1;
    } while (lVar4 <= lVar3);
  }
  return;
}


