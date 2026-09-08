/* harness: MatrixEqn addr=005f1330 size=136 */
#include "vsdlss_ref.h"


/* MatrixEqn::MatrixEqn(std::__cxx11::string const&) */

void __thiscall MatrixEqn::MatrixEqn(MatrixEqn *this,string *param_1)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  undefined2 *local_48;
  undefined8 local_40;
  undefined2 local_38;
  undefined1 uStack_36;
  undefined1 uStack_35;
  
  *(undefined **)this = &DAT_00e413c0;
  MapPartitioned3d<double,int>::MapPartitioned3d
            ((MapPartitioned3d<double,int> *)(this + 8),(MatrixPartition *)&DAT_00e413c0,param_1,
             (bool)_options[0x584]);
  local_38 = 0x7372;
  uStack_36 = 0x6c;
  local_40 = 3;
  uStack_35 = 0;
  local_48 = &local_38;
                    /* try { // try from 005f1395 to 005f1399 has its CatchHandler @ 005f147e */
  Column<double,int>::Column((Column<double,int> *)(this + 0x160),param_1,(string *)&local_48);
  if (local_48 != &local_38) {
    tc_cfree(local_48);
  }
  local_38 = 0x6872;
  uStack_36 = 0x73;
  local_40 = 3;
  uStack_35 = 0;
  local_48 = &local_38;
                    /* try { // try from 005f13df to 005f13e3 has its CatchHandler @ 005f1476 */
  Column<double,int>::Column((Column<double,int> *)(this + 0x180),param_1,(string *)&local_48);
  if (local_48 != &local_38) {
    tc_cfree(local_48);
  }
  uVar1 = *(int *)(this + 0x10) + *(int *)(this + 0x14) + *(int *)(this + 0x18);
  lVar3 = (long)(int)uVar1;
  if (uVar1 == *(uint *)(this + 0x160)) {
    uVar1 = *(uint *)(this + 0x180);
    if (uVar1 != *(uint *)(this + 0x160)) {
LAB_005f1454:
      asim_error("matrix A rows (%d) != b rows (%d)",lVar3,(ulong)uVar1);
      return;
    }
  }
  else {
                    /* try { // try from 005f1437 to 005f1462 has its CatchHandler @ 005f146e */
    asim_error("matrix A rows (%d) != x rows (%d)",lVar3);
    uVar1 = *(uint *)(this + 0x180);
    uVar2 = *(int *)(this + 0x10) + *(int *)(this + 0x14) + *(int *)(this + 0x18);
    lVar3 = (long)(int)uVar2;
    if (uVar1 != uVar2) goto LAB_005f1454;
  }
  return;
}


