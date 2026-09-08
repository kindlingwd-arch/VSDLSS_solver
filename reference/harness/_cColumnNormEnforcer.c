/* harness: ~cColumnNormEnforcer addr=008ea530 size=2e4 */
#include "vsdlss_ref.h"


/* cColumnNormEnforcer::~cColumnNormEnforcer() */

void __thiscall cColumnNormEnforcer::~cColumnNormEnforcer(cColumnNormEnforcer *this)

{
  void *pvVar1;
  undefined8 *puVar2;
  void *__ptr;
  undefined8 *puVar3;
  
  cBlockQR::~cBlockQR((cBlockQR *)(this + 0x1f8));
  pvVar1 = *(void **)(this + 0x1e8);
  __ptr = *(void **)(this + 0x1e0);
  if (pvVar1 != __ptr) {
    do {
      if (*(void **)((long)__ptr + 0x10) != (void *)0x0) {
        tc_cfree(*(void **)((long)__ptr + 0x10));
      }
      __ptr = (void *)((long)__ptr + 0x30);
    } while (pvVar1 != __ptr);
    __ptr = *(void **)(this + 0x1e0);
  }
  if (__ptr != (void *)0x0) {
    tc_cfree(__ptr);
  }
  if (*(void **)(this + 0x1c8) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x1c8));
  }
  if (*(void **)(this + 0x1b0) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x1b0));
  }
  if (*(void **)(this + 0x188) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x188));
  }
  if (*(void **)(this + 0x160) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x160));
  }
  if (*(void **)(this + 0x140) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x140));
  }
  if (*(void **)(this + 0x120) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x120));
  }
  puVar2 = *(undefined8 **)(this + 0xf8);
  puVar3 = *(undefined8 **)(this + 0xf0);
  if (puVar2 != puVar3) {
    do {
      if ((void *)*puVar3 != (void *)0x0) {
        tc_cfree((void *)*puVar3);
      }
      puVar3 = puVar3 + 3;
    } while (puVar2 != puVar3);
    puVar3 = *(undefined8 **)(this + 0xf0);
  }
  if (puVar3 != (undefined8 *)0x0) {
    tc_cfree(puVar3);
  }
  if (*(void **)(this + 0xd8) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0xd8));
  }
  if (*(void **)(this + 0xc0) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0xc0));
  }
  if (*(void **)(this + 0xa8) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0xa8));
  }
  if (*(void **)(this + 0x90) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x90));
  }
  if (*(void **)(this + 0x78) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x78));
  }
  puVar2 = *(undefined8 **)(this + 0x68);
  puVar3 = *(undefined8 **)(this + 0x60);
  if (puVar2 != puVar3) {
    do {
      if ((void *)*puVar3 != (void *)0x0) {
        tc_cfree((void *)*puVar3);
      }
      puVar3 = puVar3 + 3;
    } while (puVar2 != puVar3);
    puVar3 = *(undefined8 **)(this + 0x60);
  }
  if (puVar3 != (undefined8 *)0x0) {
    tc_cfree(puVar3);
  }
  if (*(void **)(this + 0x40) != (void *)0x0) {
    tc_cfree(*(void **)(this + 0x40));
  }
  puVar2 = *(undefined8 **)(this + 0x30);
  puVar3 = *(undefined8 **)(this + 0x28);
  if (puVar2 != puVar3) {
    do {
      if ((void *)puVar3[3] != (void *)0x0) {
        tc_cfree((void *)puVar3[3]);
      }
      if ((void *)*puVar3 != (void *)0x0) {
        tc_cfree((void *)*puVar3);
      }
      puVar3 = puVar3 + 8;
    } while (puVar2 != puVar3);
    puVar3 = *(undefined8 **)(this + 0x28);
  }
  if (puVar3 != (undefined8 *)0x0) {
    tc_cfree(puVar3);
  }
  puVar2 = *(undefined8 **)(this + 0x18);
  puVar3 = *(undefined8 **)(this + 0x10);
  if (puVar2 != puVar3) {
    do {
      if ((void *)puVar3[3] != (void *)0x0) {
        tc_cfree((void *)puVar3[3]);
      }
      if ((void *)*puVar3 != (void *)0x0) {
        tc_cfree((void *)*puVar3);
      }
      puVar3 = puVar3 + 8;
    } while (puVar2 != puVar3);
    puVar3 = *(undefined8 **)(this + 0x10);
  }
  if (puVar3 == (undefined8 *)0x0) {
    return;
  }
  tc_cfree(puVar3);
  return;
}


