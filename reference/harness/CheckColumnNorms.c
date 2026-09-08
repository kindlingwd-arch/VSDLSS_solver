/* harness: CheckColumnNorms addr=008e2440 size=9e5 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* cMatrixFit::CheckColumnNorms() */

void __thiscall cMatrixFit::CheckColumnNorms(cMatrixFit *this)

{
  double *pdVar1;
  vector<std::complex<double>,std::allocator<std::complex<double>>> *this_00;
  double dVar2;
  long *plVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  int iVar6;
  ulong uVar7;
  clock_t cVar8;
  ostream *poVar9;
  double *pdVar10;
  clock_t cVar11;
  undefined8 *puVar12;
  int iVar13;
  long lVar14;
  void *pvVar15;
  long lVar16;
  ulong uVar17;
  ulong *puVar18;
  long lVar19;
  undefined8 *puVar20;
  double dVar21;
  double dVar22;
  void *local_3d8;
  void *local_3d0;
  void *local_3c8;
  undefined8 *local_3b8;
  undefined8 *local_3b0;
  undefined8 *local_3a8;
  void *local_398;
  void *local_390;
  void *local_388;
  undefined8 *local_378;
  undefined8 *local_370;
  undefined8 *local_368;
  ulong *local_358;
  ulong local_350;
  ulong *local_348;
  ulong local_340;
  ulong *local_338;
  long local_328 [3];
  long local_310;
  ulong *local_2d8;
  int local_2d0;
  cMatrixFit local_68;
  undefined8 local_50;
  undefined8 local_48;
  
  FortranComplexMatrix::FortranComplexMatrix
            ((FortranComplexMatrix *)local_328,(long)(*(int *)(this + 0x78) + -1),
             (long)(*(int *)(this + 0x78) + -1));
  uVar7 = (ulong)*(int *)(this + 0x78);
  if (0xfffffffffffffff < uVar7) {
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 008e2e18 to 008e2e1c has its CatchHandler @ 008e2e4f */
    std::__throw_length_error("cannot create std::vector larger than max_size()");
  }
  local_3d8 = (void *)0x0;
  uVar17 = uVar7 * 8;
  local_3d0 = (void *)0x0;
  local_3c8 = (void *)0x0;
  if (uVar7 == 0) {
    pvVar15 = (void *)0x0;
  }
  else {
                    /* try { // try from 008e24b3 to 008e24b7 has its CatchHandler @ 008e2e4f */
    local_3d8 = operator_new(uVar17);
    pvVar15 = (void *)((long)local_3d8 + uVar17);
    local_3c8 = pvVar15;
    if (pvVar15 != local_3d8) {
      memset(local_3d8,0,uVar17);
    }
  }
  uVar7 = *(long *)(this + 0x6f0) - *(long *)(this + 0x6e8);
  local_3d0 = pvVar15;
  if (uVar7 < (ulong)(*(long *)(this + 0x678) - *(long *)(this + 0x670))) {
                    /* try { // try from 008e2c52 to 008e2c56 has its CatchHandler @ 008e2e5f */
    FillFreqsForPassivity(this);
    uVar7 = *(long *)(this + 0x6f0) - *(long *)(this + 0x6e8);
  }
  cVar8 = clock();
                    /* try { // try from 008e251d to 008e254a has its CatchHandler @ 008e2e5f */
  std::__ostream_insert<char,std::char_traits<char>>
            ((ostream *)&std::cout," Check Column Norm: ",0x14);
  iVar6 = (int)((long)uVar7 >> 3);
  poVar9 = (ostream *)std::ostream::operator<<((ostream *)&std::cout,iVar6);
  std::__ostream_insert<char,std::char_traits<char>>(poVar9," grid points.",0xd);
  FUN_008cfba0(poVar9);
  local_358 = (ulong *)0x0;
  iVar13 = *(int *)(this + 0x78);
  lVar19 = (long)iVar13;
  local_350 = 0;
  local_348 = (ulong *)0x0;
  local_340 = 0;
  local_338 = (ulong *)0x0;
  if (lVar19 == 0) {
    local_2d8 = (ulong *)0x0;
    local_2d0 = 0;
    local_340 = 0;
    uVar7 = 0;
  }
  else {
    uVar7 = lVar19 + 0x3fU >> 6;
                    /* try { // try from 008e2c9b to 008e2c9f has its CatchHandler @ 008e2e57 */
    local_358 = operator_new(uVar7 * 8);
    local_338 = local_358 + uVar7;
    lVar16 = lVar19 + 0x3f;
    if (-1 < lVar19) {
      lVar16 = lVar19;
    }
    local_350 = local_350 & 0xffffffff00000000;
    local_2d8 = local_358 + (lVar16 >> 6);
    lVar19 = (ulong)(iVar13 + ((uint)(iVar13 >> 0x1f) >> 0x1a) & 0x3f) -
             ((ulong)(lVar19 >> 0x3f) >> 0x3a);
    local_2d0 = (int)lVar19;
    if (lVar19 < 0) {
      local_2d8 = local_2d8 + -1;
      local_2d0 = local_2d0 + 0x40;
    }
    local_340 = CONCAT44(local_340._4_4_,local_2d0);
    local_348 = local_2d8;
    memset(local_358,0,(long)local_338 - (long)local_358);
    uVar7 = (ulong)*(int *)(this + 0x78);
  }
  if (0xfffffffffffffff < uVar7) {
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 008e2dff to 008e2e03 has its CatchHandler @ 008e2e47 */
    std::__throw_length_error("cannot create std::vector larger than max_size()");
  }
  local_3b8 = (undefined8 *)0x0;
  local_3b0 = (undefined8 *)0x0;
  local_3a8 = (undefined8 *)0x0;
  if (uVar7 == 0) {
LAB_008e2c83:
    local_388 = (void *)0x0;
    local_398 = (void *)0x0;
    pvVar15 = (void *)0x0;
    local_3b0 = local_3a8;
  }
  else {
                    /* try { // try from 008e25fb to 008e25ff has its CatchHandler @ 008e2e47 */
    local_3b8 = operator_new(uVar7 * 8);
    uVar5 = DAT_00ab6790;
    local_3b0 = local_3b8 + uVar7;
    uVar7 = (ulong)*(int *)(this + 0x78);
    for (puVar12 = local_3b8; local_3b0 != puVar12; puVar12 = puVar12 + 1) {
      *puVar12 = uVar5;
    }
    local_3a8 = local_3b0;
    if (0xfffffffffffffff < uVar7) {
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 008e2e09 to 008e2e0d has its CatchHandler @ 008e2e2f */
      std::__throw_length_error("cannot create std::vector larger than max_size()");
    }
    local_398 = (void *)0x0;
    uVar17 = uVar7 * 8;
    local_390 = (void *)0x0;
    local_388 = (void *)0x0;
    if (uVar7 == 0) goto LAB_008e2c83;
                    /* try { // try from 008e2673 to 008e2677 has its CatchHandler @ 008e2e2f */
    local_398 = operator_new(uVar17);
    pvVar15 = (void *)((long)local_398 + uVar17);
    local_388 = pvVar15;
    if (pvVar15 != local_398) {
      memset(local_398,0,uVar17);
    }
  }
  local_390 = pvVar15;
  if (0 < iVar6) {
    lVar19 = 0;
    do {
      dVar21 = *(double *)(*(long *)(this + 0x6e8) + lVar19);
                    /* try { // try from 008e26e2 to 008e29fa has its CatchHandler @ 008e2e27 */
      RationalApproxMatrix::CalculateApproximationMatrix
                (dVar21,(FortranComplexMatrix *)(this + 0x80));
      lVar16 = (long)*(int *)(this + 0x78);
      uVar7 = 0;
      if (0 < lVar16) {
        do {
          lVar14 = 0;
          dVar22 = 0.0;
          pdVar10 = (double *)((local_328[0] + 1) * uVar7 * 0x10 + local_310);
          do {
            dVar2 = *pdVar10;
            pdVar1 = pdVar10 + 1;
            lVar14 = lVar14 + 1;
            pdVar10 = pdVar10 + 2;
            dVar22 = dVar22 + *pdVar1 * *pdVar1 + dVar2 * dVar2;
          } while (lVar14 < lVar16);
          if (_DAT_00ab67f8 < dVar22) {
            local_358[uVar7 >> 6] = local_358[uVar7 >> 6] | 1L << ((byte)uVar7 & 0x3f);
          }
          pdVar10 = (double *)(local_3b8 + uVar7);
          if (*pdVar10 <= dVar22 && dVar22 != *pdVar10) {
            *pdVar10 = dVar22;
            *(double *)((long)local_398 + uVar7 * 8) = dVar21;
          }
          lVar16 = (long)*(int *)(this + 0x78);
          uVar7 = uVar7 + 1;
        } while ((long)uVar7 < lVar16);
      }
      lVar19 = lVar19 + 8;
    } while ((ulong)(iVar6 - 1) * 8 + 8 != lVar19);
  }
  lVar19 = 0;
  if (0 < *(int *)(this + 0x78)) {
    do {
      std::__ostream_insert<char,std::char_traits<char>>
                ((ostream *)&std::cout,"Maximum column norm=",0x14);
      poVar9 = std::ostream::_M_insert<double>((double)local_3b8[lVar19]);
      std::__ostream_insert<char,std::char_traits<char>>(poVar9," at freq=",9);
      poVar9 = std::ostream::_M_insert<double>(*(double *)((long)local_398 + lVar19 * 8));
      std::__ostream_insert<char,std::char_traits<char>>(poVar9," column=",8);
      poVar9 = std::ostream::_M_insert<long>((long)poVar9);
      plVar3 = *(long **)(poVar9 + *(long *)(*(long *)poVar9 + -0x18) + 0xf0);
      if (plVar3 == (long *)0x0) {
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 008e2e0e to 008e2e12 has its CatchHandler @ 008e2e27 */
        std::__throw_bad_cast();
      }
      if ((char)plVar3[7] == '\0') {
        std::ctype<char>::_M_widen_init();
        if (*(code **)(*plVar3 + 0x30) != std::ctype<char>::do_widen) {
          (**(code **)(*plVar3 + 0x30))(plVar3,10);
        }
      }
      std::ostream::put((char)poVar9);
      std::ostream::flush();
      lVar19 = lVar19 + 1;
    } while (lVar19 < *(int *)(this + 0x78));
  }
  cVar11 = clock();
  dVar21 = (double)(cVar11 - cVar8) / DAT_00abc298;
  std::__ostream_insert<char,std::char_traits<char>>
            ((ostream *)&std::cout,"Column norm check took ",0x17);
  poVar9 = std::ostream::_M_insert<double>(dVar21);
  std::__ostream_insert<char,std::char_traits<char>>(poVar9," seconds. ",10);
  FUN_008cfba0(poVar9);
  lVar19 = (local_340 & 0xffffffff) + ((long)local_348 - (long)local_358) * 8;
  uVar7 = 0;
  lVar16 = lVar19 >> 2;
  puVar18 = local_358;
  if (0 < lVar16) {
    do {
      iVar6 = (int)uVar7;
      uVar17 = *puVar18;
      iVar13 = iVar6;
      if (((((1L << ((byte)uVar7 & 0x3f) & uVar17) != 0) ||
           (iVar13 = iVar6 + 1, (1L << ((byte)(iVar6 + 1) & 0x3f) & uVar17) != 0)) ||
          (iVar13 = iVar6 + 2, (1L << ((byte)(iVar6 + 2) & 0x3f) & uVar17) != 0)) ||
         (iVar13 = iVar6 + 3, (1L << ((byte)iVar13 & 0x3f) & uVar17) != 0)) goto LAB_008e29a4;
      uVar7 = (ulong)(iVar6 + 4);
      if (iVar13 == 0x3f) {
        puVar18 = puVar18 + 1;
        uVar7 = 0;
      }
      lVar16 = lVar16 + -1;
    } while (lVar16 != 0);
    lVar19 = ((local_340 & 0xffffffff) + ((long)local_348 - (long)puVar18) * 8) - uVar7;
  }
  if (lVar19 == 2) {
    uVar17 = *puVar18;
LAB_008e2db4:
    iVar13 = (int)uVar7;
    if ((uVar17 >> (uVar7 & 0x3f) & 1) == 0) {
      uVar7 = (ulong)((int)uVar7 + 1);
LAB_008e2dc8:
      iVar13 = (int)uVar7;
      if ((uVar17 >> (uVar7 & 0x3f) & 1) == 0) goto LAB_008e2d53;
    }
  }
  else {
    if (lVar19 != 3) {
      if (lVar19 != 1) goto LAB_008e2d53;
      uVar17 = *puVar18;
      goto LAB_008e2dc8;
    }
    uVar17 = *puVar18;
    iVar13 = (int)uVar7;
    if ((uVar17 >> (uVar7 & 0x3f) & 1) == 0) {
      uVar7 = (ulong)((int)uVar7 + 1);
      goto LAB_008e2db4;
    }
  }
LAB_008e29a4:
  if ((local_348 != puVar18) || ((int)local_340 != iVar13)) {
    uVar7 = (ulong)*(int *)(this + 0x78);
    if (0x555555555555555 < uVar7) {
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 008e2e22 to 008e2e26 has its CatchHandler @ 008e2e27 */
      std::__throw_length_error("cannot create std::vector larger than max_size()");
    }
    local_378 = (undefined8 *)0x0;
    local_370 = (undefined8 *)0x0;
    local_368 = (undefined8 *)0x0;
    if (uVar7 != 0) {
      local_378 = operator_new(uVar7 * 0x18);
      local_368 = local_378 + uVar7 * 3;
      iVar13 = *(int *)(this + 0x78);
      puVar12 = local_378;
      do {
        *puVar12 = 0;
        local_370 = puVar12 + 3;
        puVar12[1] = 0;
        puVar12[2] = 0;
        puVar12 = local_370;
      } while (local_370 != local_368);
      puVar12 = local_368;
      puVar4 = local_378;
      puVar20 = local_368;
      if (0 < iVar13) {
        lVar19 = 0;
        do {
          uVar17 = (ulong)*(int *)(this + 0x7c);
          this_00 = (vector<std::complex<double>,std::allocator<std::complex<double>>> *)
                    (local_378 + lVar19 * 3);
          uVar7 = *(long *)(this_00 + 8) - *(long *)this_00 >> 4;
          if (uVar7 < uVar17) {
                    /* try { // try from 008e2a98 to 008e2b0d has its CatchHandler @ 008e2e37 */
            std::vector<std::complex<double>,std::allocator<std::complex<double>>>::
            _M_default_append(this_00,uVar17 - uVar7);
LAB_008e2a5c:
            iVar13 = *(int *)(this + 0x78);
          }
          else if ((uVar17 < uVar7) &&
                  (lVar16 = uVar17 * 0x10 + *(long *)this_00, *(long *)(this_00 + 8) != lVar16)) {
            *(long *)(this_00 + 8) = lVar16;
            goto LAB_008e2a5c;
          }
          lVar19 = lVar19 + 1;
        } while ((int)lVar19 < iVar13);
        puVar12 = local_370;
        puVar4 = local_378;
        puVar20 = local_370;
        if (0 < iVar13) {
          uVar7 = 0;
          do {
            iVar13 = (int)uVar7;
            if ((1L << ((byte)uVar7 & 0x3f) & local_358[uVar7 >> 6]) != 0) {
              FillOriginalDataForColumn(this,(vector *)&local_378,iVar13);
              cColumnNormEnforcer::cColumnNormEnforcer
                        ((cColumnNormEnforcer *)&local_2d8,(vector *)(this + 0x670),
                         (vector *)&local_378,(vector *)(this + 0x6e8),
                         (RationalApproxMatrix *)(this + 0x80),iVar13);
              local_50 = DAT_00ab7338;
              local_48 = DAT_00ad4f20;
              local_68 = this[0x71e];
                    /* try { // try from 008e2b4c to 008e2b62 has its CatchHandler @ 008e2e3f */
              cColumnNormEnforcer::EnforceColumnNorm
                        ((cColumnNormEnforcer *)&local_2d8,(double)local_3b8[uVar7]);
              cColumnNormEnforcer::SetModifiedColumn
                        ((cColumnNormEnforcer *)&local_2d8,(RationalApproxMatrix *)(this + 0x80),
                         iVar13);
              cColumnNormEnforcer::~cColumnNormEnforcer((cColumnNormEnforcer *)&local_2d8);
            }
            uVar7 = uVar7 + 1;
            puVar12 = local_370;
            puVar4 = local_378;
            puVar20 = local_370;
          } while ((int)uVar7 < *(int *)(this + 0x78));
        }
      }
      for (; puVar4 != puVar20; puVar4 = puVar4 + 3) {
        if ((void *)*puVar4 != (void *)0x0) {
          tc_cfree((void *)*puVar4);
        }
        puVar12 = local_378;
      }
      if (puVar12 != (undefined8 *)0x0) {
        tc_cfree(puVar12);
      }
    }
    if (local_398 != (void *)0x0) {
      tc_cfree(local_398);
    }
    if (local_3b8 != (undefined8 *)0x0) {
      tc_cfree(local_3b8);
    }
    if (local_358 != (ulong *)0x0) {
      tc_cfree(local_358);
    }
    if (local_3d8 != (void *)0x0) {
      tc_cfree(local_3d8);
    }
    FortranComplexMatrix::~FortranComplexMatrix((FortranComplexMatrix *)local_328);
    return;
  }
LAB_008e2d53:
  std::_Vector_base<double,std::allocator<double>>::~_Vector_base
            ((_Vector_base<double,std::allocator<double>> *)&local_398);
  std::_Vector_base<double,std::allocator<double>>::~_Vector_base
            ((_Vector_base<double,std::allocator<double>> *)&local_3b8);
  std::_Bvector_base<std::allocator<bool>>::_M_deallocate
            ((_Bvector_base<std::allocator<bool>> *)&local_358);
  std::_Vector_base<double,std::allocator<double>>::~_Vector_base
            ((_Vector_base<double,std::allocator<double>> *)&local_3d8);
  FortranComplexMatrix::~FortranComplexMatrix((FortranComplexMatrix *)local_328);
  return;
}


