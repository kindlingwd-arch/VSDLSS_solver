/* harness: Column addr=005f3810 size=295 */
#include "vsdlss_ref.h"


/* Column<double, int>::Column(std::__cxx11::string const&, std::__cxx11::string const&) */

void __thiscall Column<double,int>::Column(Column<double,int> *this,string *param_1,string *param_2)

{
  undefined1 *__src;
  int iVar1;
  long *plVar2;
  FILE *__stream;
  ulong uVar3;
  void *__ptr;
  size_t sVar4;
  long *local_78;
  long local_70;
  long local_68;
  long lStack_60;
  long **local_58;
  long *local_50;
  long *local_48 [3];
  
  __src = *(undefined1 **)param_1;
  plVar2 = *(long **)(param_1 + 8);
  local_58 = local_48;
  if ((__src + (long)plVar2 != (undefined1 *)0x0) && (__src == (undefined1 *)0x0)) {
                    /* WARNING: Subroutine does not return */
    std::__throw_logic_error("basic_string::_M_construct null not valid");
  }
  local_78 = plVar2;
  if (plVar2 < (long *)0x10) {
    if (plVar2 == (long *)0x1) {
      local_48[0] = (long *)CONCAT71(local_48[0]._1_7_,*__src);
    }
    else if (plVar2 != (long *)0x0) goto LAB_005f3a53;
  }
  else {
    local_58 = (long **)std::__cxx11::string::_M_create((ulong *)&local_58,(ulong)&local_78);
    local_48[0] = local_78;
LAB_005f3a53:
    memcpy(local_58,__src,(size_t)plVar2);
  }
  *(undefined1 *)((long)local_58 + (long)local_78) = 0;
  local_50 = local_78;
  if (local_78 == (long *)0x3fffffffffffffff) {
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 005f3ab0 to 005f3ab4 has its CatchHandler @ 005f3aec */
    std::__throw_length_error("basic_string::append");
  }
                    /* try { // try from 005f389a to 005f389e has its CatchHandler @ 005f3aec */
  std::__cxx11::string::_M_append((char *)&local_58,0xafee4e);
                    /* try { // try from 005f38aa to 005f38ae has its CatchHandler @ 005f3abf */
  plVar2 = (long *)std::__cxx11::string::_M_append((char *)&local_58,*(ulong *)param_2);
  if ((long *)*plVar2 == plVar2 + 2) {
    local_68 = plVar2[2];
    lStack_60 = plVar2[3];
    local_78 = &local_68;
  }
  else {
    local_68 = plVar2[2];
    local_78 = (long *)*plVar2;
  }
  local_70 = plVar2[1];
  *plVar2 = (long)(plVar2 + 2);
  plVar2[1] = 0;
  *(undefined1 *)(plVar2 + 2) = 0;
  if (local_58 != local_48) {
    tc_cfree(local_58);
  }
                    /* try { // try from 005f390e to 005f3a21 has its CatchHandler @ 005f3ac4 */
  __stream = fopen((char *)local_78,"r");
  if (__stream == (FILE *)0x0) {
    *(undefined4 *)this = 0;
    *(undefined8 *)(this + 0x10) = 0;
    goto joined_r0x005f3a96;
  }
  iVar1 = fseek(__stream,0,2);
  if (iVar1 == 0) {
    uVar3 = ftell(__stream);
    if ((uVar3 == 0xffffffffffffffff) || (iVar1 = fseek(__stream,0,0), iVar1 != 0))
    goto LAB_005f3a12;
  }
  else {
    uVar3 = 0;
LAB_005f3a12:
    asim_fatal("fseek error with file \"%s\"",local_78);
  }
  iVar1 = (int)(uVar3 >> 3);
  *(int *)this = iVar1;
  if (0xfffffffffffffff < (ulong)(long)iVar1) {
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 005f3ae7 to 005f3aeb has its CatchHandler @ 005f3ac4 */
    __cxa_throw_bad_array_new_length();
  }
  __ptr = operator_new((long)iVar1 << 3);
  *(void **)(this + 0x10) = __ptr;
  *(void **)(this + 8) = __ptr;
  sVar4 = fread(__ptr,8,(long)*(int *)this,__stream);
  if ((long)*(int *)this != sVar4) {
    asim_fatal("error reading file \"%s\"",local_78);
  }
  fclose(__stream);
joined_r0x005f3a96:
  if (local_78 != &local_68) {
    tc_cfree(local_78);
  }
  return;
}


