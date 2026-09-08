/* harness: solverVsdlss addr=009d7620 size=1ef */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int solverVsdlss(int param_1,long param_2)

{
  char cVar1;
  char *__s;
  bool bVar2;
  int iVar3;
  long lVar4;
  ulong uVar5;
  size_t __n;
  undefined8 uVar6;
  ulong uVar7;
  undefined4 local_43c;
  undefined1 local_438 [1040];
  
  local_43c = 0;
  _DAT_00e96240 = _setjmp((__jmp_buf_tag *)&DAT_00e96260);
  iVar3 = _DAT_00e96240;
  if (_DAT_00e96240 == 0) {
    param_1 = param_1 + -1;
    __s = *(char **)(param_2 + 8);
    if (0 < param_1) {
      uVar6 = 0;
      bVar2 = false;
      uVar7 = 0;
      cVar1 = *__s;
      while (cVar1 == '-') {
        lVar4 = param_2 + 8;
        if (__s[1] != '\0') {
          switch(__s[1]) {
          case 'S':
            SILENT_vsdlss = 1;
            break;
          default:
            goto switchD_009d769b_caseD_54;
          case 'U':
          case 'u':
            local_43c = 1;
            break;
          case 'W':
          case 'w':
            uVar6 = 1;
            break;
          case 'p':
            param_1 = param_1 + -1;
            lVar4 = param_2 + 0x10;
            if (param_1 < 2) goto switchD_009d769b_caseD_54;
            uVar5 = strtol(*(char **)(param_2 + 0x10),(char **)0x0,10);
            uVar7 = uVar5 & 0xffffffff;
            if (5 < (uint)uVar5) goto switchD_009d769b_caseD_54;
            break;
          case 's':
            bVar2 = true;
          }
        }
        param_1 = param_1 + -1;
        __s = *(char **)(lVar4 + 8);
        if (param_1 == 0) goto switchD_009d769b_caseD_54;
        param_2 = lVar4;
        cVar1 = *__s;
      }
      if (param_1 == 1) {
        __n = strlen(__s);
        if (__n < 0x400) {
          memcpy(local_438,__s,__n + 1);
          memcpy(&DAT_00e96340,local_438,__n);
          *(undefined4 *)(&DAT_00e96340 + __n) = 0x474c432e;
          (&DAT_00e96344)[__n] = 0;
          DAT_00e96440 = (FILE *)openFileMainAT_vsdlss(&DAT_00e96340);
          if (bVar2) {
            iVar3 = mainSpice_vsdlss(local_438,uVar7,&local_43c);
          }
          else {
            iVar3 = mainMat_vsdlss(local_438,uVar7,uVar6);
          }
          fclose(DAT_00e96440);
          DAT_00e96440 = (FILE *)0x0;
          return iVar3;
        }
                    /* WARNING: Subroutine does not return */
        fatalError_vsdlss(9999,"main.c",0xb6a,"VSDLSS jobName exceeds buffer length\n");
      }
    }
switchD_009d769b_caseD_54:
    if (SILENT_vsdlss == 0) {
      FUN_009d2030();
    }
    iVar3 = 9999;
  }
  return iVar3;
}


