/* harness: EnforceColumnNorm addr=0090ee80 size=5b2 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* cColumnNormEnforcer::EnforceColumnNorm(double) */

void __thiscall cColumnNormEnforcer::EnforceColumnNorm(cColumnNormEnforcer *this,double param_1)

{
  cColumnNormEnforcer *pcVar1;
  double *pdVar2;
  long *plVar3;
  long *plVar4;
  long *plVar5;
  char cVar6;
  clock_t cVar7;
  long *plVar8;
  long lVar9;
  ulong uVar10;
  clock_t cVar11;
  ostream *poVar12;
  ulong uVar13;
  long lVar14;
  uint uVar15;
  undefined8 *puVar16;
  undefined8 *puVar17;
  uint uVar18;
  uint uVar19;
  byte bVar20;
  double dVar21;
  double dVar22;
  double local_80;
  double *local_78;
  double *local_70;
  double *local_68;
  long *local_58;
  long *local_50;
  long *local_48;
  
  bVar20 = 0;
  cVar7 = clock();
  Initialize(this);
  local_80 = 0.0;
  local_78 = (double *)0x0;
  local_70 = (double *)0x0;
  local_68 = (double *)0x0;
  local_78 = operator_new(0xd0);
  local_70 = local_78 + 0x1a;
  *local_78 = 0.0;
  local_78[0x19] = 0.0;
  puVar17 = (undefined8 *)((ulong)(local_78 + 1) & 0xfffffffffffffff8);
  for (uVar13 = (ulong)(((int)local_78 -
                        (int)(undefined8 *)((ulong)(local_78 + 1) & 0xfffffffffffffff8)) + 0xd0U >>
                       3); uVar13 != 0; uVar13 = uVar13 - 1) {
    *puVar17 = 0;
    puVar17 = puVar17 + (ulong)bVar20 * -2 + 1;
  }
  local_58 = (long *)0x0;
  local_50 = (long *)0x0;
  local_48 = (long *)0x0;
  local_68 = local_70;
                    /* try { // try from 0090ef3a to 0090ef3e has its CatchHandler @ 0090f439 */
  local_58 = operator_new(0x270);
  local_48 = local_58 + 0x4e;
  plVar8 = local_58;
  do {
    *plVar8 = 0;
    local_50 = plVar8 + 3;
    plVar8[1] = 0;
    plVar8[2] = 0;
    plVar8 = local_50;
  } while (local_50 != local_48);
  pcVar1 = this + 0x10;
                    /* try { // try from 0090ef84 to 0090f438 has its CatchHandler @ 0090f441 */
  FUN_0090e510(local_58,pcVar1);
  uVar19 = 0x19;
  *local_78 = param_1;
  lVar14 = 8;
  uVar18 = 0;
  while( true ) {
    CalculateCorrection(this);
    LineSearch(this);
    cVar6 = CheckColumnNorm(this,&local_80);
    uVar15 = uVar18 + 1;
    FUN_0090e510(lVar14 * 3 + (long)local_58,pcVar1);
    *(double *)((long)local_78 + lVar14) = local_80;
    if ((cVar6 != '\0') || (local_80 <= DAT_00b11db0)) break;
    plVar8 = local_50;
    if ((uVar19 - 1 == uVar18) && (uVar19 < 0x28)) {
      lVar9 = 1;
      dVar21 = *local_78;
      do {
        pdVar2 = local_78 + lVar9;
        lVar9 = lVar9 + 1;
        dVar22 = *pdVar2;
        if (dVar21 <= *pdVar2) {
          dVar22 = dVar21;
        }
        dVar21 = dVar22;
      } while ((int)lVar9 <= (int)uVar18);
      if (_DAT_00b11df8 < dVar22) {
        uVar13 = 0x29;
        uVar19 = 0x28;
      }
      else {
        uVar13 = (ulong)(int)(uVar19 + 1);
      }
      uVar10 = (long)local_70 - (long)local_78 >> 3;
      if (uVar10 < uVar13) {
        std::vector<double,std::allocator<double>>::_M_default_append
                  ((vector<double,std::allocator<double>> *)&local_78,uVar13 - uVar10);
      }
      else if ((uVar13 < uVar10) && (local_70 != local_78 + uVar13)) {
        local_70 = local_78 + uVar13;
      }
      plVar5 = local_50;
      lVar9 = (long)local_50 - (long)local_58 >> 3;
      uVar10 = lVar9 * -0x5555555555555555;
      if (uVar10 < uVar13) {
        std::
        vector<std::vector<PoleResidueForm,std::allocator<PoleResidueForm>>,std::allocator<std::vector<PoleResidueForm,std::allocator<PoleResidueForm>>>>
        ::_M_default_append((vector<std::vector<PoleResidueForm,std::allocator<PoleResidueForm>>,std::allocator<std::vector<PoleResidueForm,std::allocator<PoleResidueForm>>>>
                             *)&local_58,uVar13 + lVar9 * 0x5555555555555555);
        plVar8 = local_50;
      }
      else {
        plVar8 = local_50;
        if (uVar13 < uVar10) {
          plVar3 = local_58 + uVar13 * 3;
          for (plVar4 = plVar3; plVar5 != plVar4; plVar4 = plVar4 + 3) {
            puVar17 = (undefined8 *)plVar4[1];
            puVar16 = (undefined8 *)*plVar4;
            if (puVar17 != puVar16) {
              do {
                if ((void *)puVar16[3] != (void *)0x0) {
                  tc_cfree((void *)puVar16[3]);
                }
                if ((void *)*puVar16 != (void *)0x0) {
                  tc_cfree((void *)*puVar16);
                }
                puVar16 = puVar16 + 8;
              } while (puVar17 != puVar16);
              puVar16 = (undefined8 *)*plVar4;
            }
            if (puVar16 != (undefined8 *)0x0) {
              tc_cfree(puVar16);
            }
            plVar8 = plVar3;
          }
        }
      }
    }
    local_50 = plVar8;
    if ((int)uVar19 <= (int)uVar15) goto LAB_0090f1f4;
    lVar14 = lVar14 + 8;
    uVar18 = uVar15;
  }
  std::__ostream_insert<char,std::char_traits<char>>
            ((ostream *)&std::cout,"Column norm <=1 has been achieved... ",0x25);
  plVar8 = *(long **)(&DAT_00e40e90 + *(long *)(std::cout + -0x18));
  if (plVar8 == (long *)0x0) {
                    /* WARNING: Subroutine does not return */
    std::__throw_bad_cast();
  }
  if ((char)plVar8[7] == '\0') {
    std::ctype<char>::_M_widen_init();
    if (*(code **)(*plVar8 + 0x30) != std::ctype<char>::do_widen) {
      (**(code **)(*plVar8 + 0x30))(plVar8,10);
    }
  }
  std::ostream::put(-0x60);
  std::ostream::flush();
  uVar19 = uVar15;
LAB_0090f1f4:
  uVar10 = 0;
  dVar21 = *local_78;
  uVar13 = 1;
  do {
    if (local_78[uVar13] < dVar21) {
      uVar10 = uVar13 & 0xffffffff;
      dVar21 = local_78[uVar13];
    }
    uVar13 = uVar13 + 1;
  } while (uVar13 != uVar19 + 1);
  FUN_0090e510(pcVar1,local_58 + (long)(int)uVar10 * 3);
  cVar11 = clock();
  dVar21 = (double)(cVar11 - cVar7) / DAT_00abc298;
  std::__ostream_insert<char,std::char_traits<char>>
            ((ostream *)&std::cout,"Column Norm enforcement took ",0x1d);
  poVar12 = std::ostream::_M_insert<double>(dVar21);
  std::__ostream_insert<char,std::char_traits<char>>(poVar12," seconds. ",10);
  plVar8 = *(long **)(poVar12 + *(long *)(*(long *)poVar12 + -0x18) + 0xf0);
  if (plVar8 == (long *)0x0) {
                    /* WARNING: Subroutine does not return */
    std::__throw_bad_cast();
  }
  if ((char)plVar8[7] == '\0') {
    std::ctype<char>::_M_widen_init();
    if (*(code **)(*plVar8 + 0x30) != std::ctype<char>::do_widen) {
      (**(code **)(*plVar8 + 0x30))(plVar8,10);
    }
  }
  std::ostream::put((char)poVar12);
  std::ostream::flush();
  plVar5 = local_50;
  for (plVar8 = local_58; plVar5 != plVar8; plVar8 = plVar8 + 3) {
    puVar17 = (undefined8 *)plVar8[1];
    puVar16 = (undefined8 *)*plVar8;
    if (puVar17 != puVar16) {
      do {
        if ((void *)puVar16[3] != (void *)0x0) {
          tc_cfree((void *)puVar16[3]);
        }
        if ((void *)*puVar16 != (void *)0x0) {
          tc_cfree((void *)*puVar16);
        }
        puVar16 = puVar16 + 8;
      } while (puVar17 != puVar16);
      puVar16 = (undefined8 *)*plVar8;
    }
    if (puVar16 != (undefined8 *)0x0) {
      tc_cfree(puVar16);
    }
  }
  if (local_58 != (long *)0x0) {
    tc_cfree(local_58);
  }
  if (local_78 != (double *)0x0) {
    tc_cfree(local_78);
  }
  return;
}


