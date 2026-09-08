/* VSDLSS function (decompiled by Ghidra) */
/* name: anilFree_vsdlss  addr: 009d2020  size: 5 */
#include "vsdlss_ref.h"

/* signature: void anilFree_vsdlss(void *__ptr); */

void anilFree_vsdlss(void *__ptr)

{
  FreeList *pFVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  ThreadCache *this;
  undefined8 uVar5;
  long lVar6;
  ulong uVar7;
  ulong uVar8;
  ulong uVar9;
  ulong uVar10;
  long in_FS_OFFSET;
  
  if (DAT_00f05ee0 != 0) {
    FUN_00ab4640();
    return;
  }
  uVar8 = (ulong)__ptr >> 0xd;
  uVar9 = uVar8 & 0xffff;
  this = *(ThreadCache **)(PTR_00e33f40 + in_FS_OFFSET + 8);
  uVar7 = *(ulong *)(&DAT_00f23920 + uVar9 * 8) ^ ((ulong)__ptr >> 0x1d) << 0x10;
  uVar10 = uVar7 & 0xffffffff;
  if (0x7f < uVar7) {
    if ((((ulong)__ptr >> 0x30 != 0) || (*(long *)(&DAT_00fa3920 + ((ulong)__ptr >> 0x1f) * 8) == 0)
        ) || (lVar6 = *(long *)(*(long *)(&DAT_00fa3920 + ((ulong)__ptr >> 0x1f) * 8) +
                               (ulong)((uint)uVar8 & 0x3ffff) * 8), lVar6 == 0)) goto LAB_00ab498a;
    uVar10 = (ulong)*(byte *)(lVar6 + 0x2a);
    if (*(byte *)(lVar6 + 0x2a) == 0) {
      FUN_00aa2920(lVar6,__ptr);
      return;
    }
    *(ulong *)(&DAT_00f23920 + uVar9 * 8) = uVar8 ^ uVar9 | uVar10;
  }
  if (this != (ThreadCache *)0x0) {
    iVar2 = *(int *)(this + 0xc00);
    pFVar1 = (FreeList *)(this + uVar10 * 0x20);
    iVar3 = *(int *)(pFVar1 + 8);
    uVar5 = *(undefined8 *)pFVar1;
    *(void **)pFVar1 = __ptr;
    uVar4 = *(uint *)(pFVar1 + 0x10);
    *(undefined8 *)__ptr = uVar5;
    *(uint *)(pFVar1 + 8) = iVar3 + 1U;
    if (uVar4 < iVar3 + 1U) {
      tcmalloc::ThreadCache::ListTooLong(this,pFVar1,(uint)uVar10);
      return;
    }
    iVar3 = *(int *)(pFVar1 + 0x18);
    *(int *)(this + 0xc00) = iVar2 - iVar3;
    if (-1 < iVar2 - iVar3) {
      return;
    }
    tcmalloc::ThreadCache::Scavenge(this);
    return;
  }
  if (tcmalloc::Static::inited_ != '\0') {
    *(undefined8 *)__ptr = 0;
    tcmalloc::CentralFreeList::InsertRange
              ((CentralFreeList *)(&DAT_00f07060 + uVar10 * 0x130),__ptr,__ptr,1);
    return;
  }
LAB_00ab498a:
  if (__ptr == (void *)0x0) {
    return;
  }
  FUN_00aa14a0(__ptr);
  return;
}


