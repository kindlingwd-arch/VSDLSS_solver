/* harness: solve<Matrix<Column<double,int>>> addr=005f34d0 size=338 */
#include "vsdlss_ref.h"


/* void MapPartitioned3d<double, int>::solve<Matrix<Column<double, int> >
   >(SolverAttributes::SolveCase, Matrix<Column<double, int> >&, Matrix<Column<double, int> >&) */

void __thiscall
MapPartitioned3d<double,int>::solve<Matrix<Column<double,int>>>
          (MapPartitioned3d<double,int> *this,int param_2,long param_3,long param_4)

{
  double *pdVar1;
  long lVar2;
  void *__src;
  long lVar3;
  void *pvVar4;
  int iVar5;
  uint uVar6;
  ulong uVar7;
  int *piVar8;
  ulong uVar9;
  int iVar10;
  int *piVar11;
  int iVar12;
  void *local_78;
  undefined8 local_70;
  int local_68;
  void *local_58;
  undefined8 local_50;
  undefined8 local_48;
  int local_40;
  
  if (param_2 == -1) {
    (**(code **)(**(long **)(this + 0x140) + 8))
              (*(long **)(this + 0x140),0xffffffff,this + 0x18,this + 0x58,this + 0x90,
               *(undefined8 *)(param_3 + 0x10));
    if (*(int *)(this + 0x10) == 0) {
      return;
    }
                    /* WARNING: Could not recover jumptable at 0x005f37dd. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)**(undefined8 **)(this + 0x148))
              (*(undefined8 **)(this + 0x148),0xffffffff,this + 0x100,
               *(undefined8 *)(param_3 + 0x10));
    return;
  }
  iVar5 = *(int *)(this + 0x10);
  iVar12 = param_2;
  if (param_2 == 5) {
    if (iVar5 != 0) {
      iVar12 = 3;
    }
    param_2 = (iVar5 == 0) + 2;
  }
  iVar10 = *(int *)(this + 8) + *(int *)(this + 0xc);
  if (0xfffffffffffffff < (ulong)(long)(iVar5 + iVar10)) {
                    /* WARNING: Subroutine does not return */
    __cxa_throw_bad_array_new_length();
  }
  __src = operator_new((long)(iVar5 + iVar10) << 3);
  pvVar4 = (void *)((long)__src + (long)iVar10 * 8);
  local_50 = *(undefined8 *)(param_3 + 0x10);
  local_48 = *(undefined8 *)(param_4 + 0x10);
  local_58 = __src;
  local_40 = iVar10;
  MatrixPartition<int>::apply<Functor1<double,int>>
            (*(MatrixPartition<int> **)this,(Functor1 *)&local_58);
  if (*(int *)(this + 0x10) != 0) {
    piVar11 = *(int **)(this + 0xe0);
    for (piVar8 = *(int **)(this + 0xd8); piVar11 != piVar8; piVar8 = piVar8 + 4) {
      pdVar1 = (double *)((long)__src + (long)*piVar8 * 8);
      *pdVar1 = *pdVar1 - *(double *)((long)pvVar4 + (long)piVar8[1] * 8) * *(double *)(piVar8 + 2);
    }
  }
  (**(code **)(**(long **)(this + 0x140) + 8))
            (*(long **)(this + 0x140),param_2,this + 0x18,this + 0x58,this + 0x90,__src);
  if (*(int *)(this + 0x10) != 0) {
    local_70 = *(undefined8 *)(param_3 + 0x10);
    local_78 = __src;
    local_68 = iVar10;
    MatrixPartition<int>::apply<Functor2<double,int>>
              (*(MatrixPartition<int> **)this,(Functor2 *)&local_78);
    piVar8 = *(int **)(this + 0xe0);
    for (piVar11 = *(int **)(this + 0xd8); piVar11 != piVar8; piVar11 = piVar11 + 4) {
      pdVar1 = (double *)((long)pvVar4 + (long)piVar11[1] * 8);
      *pdVar1 = *pdVar1 - *(double *)((long)__src + (long)*piVar11 * 8) * *(double *)(piVar11 + 2);
    }
    (**(code **)**(undefined8 **)(this + 0x148))
              (*(undefined8 **)(this + 0x148),iVar12,this + 0x100,pvVar4);
  }
  lVar2 = *(long *)this;
  pvVar4 = *(void **)(param_4 + 0x10);
  lVar3 = *(long *)(lVar2 + 0x18);
  if (lVar3 != 0) {
    uVar9 = 0;
    if (0 < *(int *)(lVar2 + 0x10)) {
      while( true ) {
        uVar7 = uVar9 & 0xffffffff;
        if (lVar3 != 0) {
          uVar7 = (ulong)*(uint *)(lVar3 + uVar9 * 4);
        }
        piVar8 = *(int **)(lVar2 + 8);
        iVar5 = *piVar8;
        uVar6 = (uint)uVar7;
        if (iVar5 <= (int)uVar6) {
          do {
            piVar8 = piVar8 + 4;
            uVar6 = (int)uVar7 - iVar5;
            uVar7 = (ulong)uVar6;
            iVar5 = *piVar8;
          } while (iVar5 <= (int)uVar6);
        }
        *(undefined8 *)((long)pvVar4 + uVar9 * 8) =
             *(undefined8 *)((long)__src + (long)(int)(uVar6 + piVar8[1]) * 8);
        uVar9 = uVar9 + 1;
        if (*(int *)(lVar2 + 0x10) <= (int)uVar9) break;
        lVar3 = *(long *)(lVar2 + 0x18);
      }
    }
    goto LAB_005f3698;
  }
  iVar5 = *(int *)(lVar2 + 4);
  if (iVar5 == 1) {
LAB_005f37eb:
    memcpy(pvVar4,__src,(long)*(int *)(lVar2 + 0x10) << 3);
  }
  else {
    piVar8 = *(int **)(lVar2 + 8);
    if (iVar5 == 2) {
      if (*piVar8 == 0) goto LAB_005f37eb;
    }
    else if (((long)iVar5 & 0xfffffffffffffffU) == 0) goto LAB_005f3698;
    do {
      piVar11 = piVar8 + 4;
      pvVar4 = memcpy(pvVar4,(void *)((long)__src + (long)piVar8[1] * 8),(long)*piVar8 << 3);
      pvVar4 = (void *)((long)pvVar4 + (long)*piVar8 * 8);
      piVar8 = piVar11;
    } while (piVar11 != (int *)((long)*(int *)(lVar2 + 4) * 0x10 + *(long *)(lVar2 + 8)));
  }
LAB_005f3698:
  tc_cfree(__src);
  return;
}


