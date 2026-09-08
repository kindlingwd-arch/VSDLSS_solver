/* driver: yKFIkyIIL_XSPAIPP addr=0095a7f0 size=5 */
#include "vsdlss_ref.h"


void * yKFIkyIIL_XSPAIPP(size_t __size)

{
  ThreadCache *pTVar1;
  ThreadCache *this;
  undefined8 *puVar2;
  uint uVar3;
  void *pvVar4;
  long in_FS_OFFSET;
  
  if ((DAT_00e40660 == 0) &&
     (this = *(ThreadCache **)(PTR_00e33f40 + in_FS_OFFSET), this != (ThreadCache *)0x0)) {
    if (__size < 0x401) {
      uVar3 = (int)__size + 7U >> 3;
    }
    else {
      if (0x40000 < __size) goto LAB_00ab54e8;
      uVar3 = (int)__size + 0x3c7fU >> 7;
    }
    pTVar1 = this + (ulong)(byte)(&DAT_00f061c0)[uVar3] * 0x20;
    puVar2 = *(undefined8 **)pTVar1;
    if (puVar2 != (undefined8 *)0x0) {
      *(undefined8 *)pTVar1 = *puVar2;
      uVar3 = *(int *)(pTVar1 + 8) - 1;
      *(uint *)(pTVar1 + 8) = uVar3;
      if (uVar3 < *(uint *)(pTVar1 + 0xc)) {
        *(uint *)(pTVar1 + 0xc) = uVar3;
      }
      *(int *)(this + 0xc00) = *(int *)(this + 0xc00) + *(int *)(pTVar1 + 0x18);
      return puVar2;
    }
    pvVar4 = (void *)tcmalloc::ThreadCache::FetchFromCentralCache
                               (this,(uint)(byte)(&DAT_00f061c0)[uVar3],*(int *)(pTVar1 + 0x18));
    return pvVar4;
  }
LAB_00ab54e8:
  pvVar4 = (void *)tcmalloc::allocate_full_malloc_oom(__size);
  return pvVar4;
}


