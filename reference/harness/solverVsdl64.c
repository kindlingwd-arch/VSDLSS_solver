/* harness: solverVsdl64 addr=0095fe30 size=1f6 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int solverVsdl64(int param_1,long param_2)

{
  char cVar1;
  char *__s;
  bool bVar2;
  int iVar3;
  long lVar4;
  size_t __n;
  long lVar5;
  ulong uVar6;
  undefined8 uVar7;
  undefined8 local_440;
  undefined1 local_438 [1040];
  
  local_440 = 0;
  iVar3 = _setjmp((__jmp_buf_tag *)&DAT_00e56160);
  _DAT_00e56140 = (long)iVar3;
  if (_DAT_00e56140 == 0) {
    param_1 = param_1 + -1;
    __s = *(char **)(param_2 + 8);
    if (0 < param_1) {
      uVar7 = 0;
      bVar2 = false;
      uVar6 = 0;
      cVar1 = *__s;
      while (cVar1 == '-') {
        lVar5 = param_2 + 8;
        if (__s[1] != '\0') {
          switch(__s[1]) {
          case 'S':
            qgjclrXSPAIPP = 1;
            break;
          default:
            goto switchD_0095feae_caseD_54;
          case 'U':
          case 'u':
            local_440 = 1;
            break;
          case 'W':
          case 'w':
            uVar7 = 1;
            break;
          case 'p':
            param_1 = param_1 + -1;
            lVar5 = param_2 + 0x10;
            if (param_1 < 2) goto switchD_0095feae_caseD_54;
            lVar4 = strtol(*(char **)(param_2 + 0x10),(char **)0x0,10);
            uVar6 = (ulong)(int)lVar4;
            if (5 < uVar6) goto switchD_0095feae_caseD_54;
            break;
          case 's':
            bVar2 = true;
          }
        }
        param_1 = param_1 + -1;
        __s = *(char **)(lVar5 + 8);
        if (param_1 == 0) goto switchD_0095feae_caseD_54;
        param_2 = lVar5;
        cVar1 = *__s;
      }
      if (param_1 == 1) {
        __n = strlen(__s);
        if (__n < 0x400) {
          memcpy(local_438,__s,__n + 1);
          memcpy(&DAT_00e56240,local_438,__n);
          *(undefined4 *)(&DAT_00e56240 + __n) = 0x474c432e;
          (&DAT_00e56244)[__n] = 0;
          DAT_00e56340 = (FILE *)LMBKdFIBkyFKYrXSPAIPP(&DAT_00e56240);
          if (bVar2) {
            iVar3 = JyFKqMF_BXSPAIPP(local_438,uVar6,&local_440);
          }
          else {
            iVar3 = JyFKkyQXSPAIPP(local_438,uVar6,uVar7);
          }
          fclose(DAT_00e56340);
          DAT_00e56340 = (FILE *)0x0;
          return iVar3;
        }
                    /* WARNING: Subroutine does not return */
        CyQyIcOOLOXSPAIPP(9999,"main.c",0xb6a,"VSDLSS jobName exceeds buffer length\n");
      }
    }
switchD_0095feae_caseD_54:
    if (qgjclrXSPAIPP == 0) {
      FUN_0095a810();
    }
    iVar3 = 9999;
  }
  return iVar3;
}


